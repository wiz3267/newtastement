#include "debug.h"
#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include "sound.h"
#include "digplay.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "settings.h"
#include <fstream.h>

#include <stdlib.h>
#include "midpak2.h"



#define STROY_24	 	2
#define STROY_PIANO 	1
#define STROY_NATURAL	0

//*********************************************************//
//*********************************************************//
//*********************************************************//

extern "C" extern short far * cdecl far PendingAddress(void);
extern "C" extern short	cdecl far AudioPendingStatus(void);
extern "C" short	cdecl far PostAudioPending(SNDSTRUC far *sndplay);
extern "C" short cdecl far  DigPlay(SNDSTRUC far *sndplay); // 688h -> Play 8 bit digitized sound.
extern "C" short cdecl far SetBackFillMode(short mode); // Turn DMA backfill mode on/off, return code
extern "C"    unsigned short cdecl far ReportDMAC(void); // Report current DMA counter.

//*********************************************************//
//*********************************************************//
//*********************************************************//


static int counter=0;
const int SOUND_LEN=640;
static unsigned char SOUND_BUF1[SOUND_LEN];
static unsigned char SOUND_BUF2[SOUND_LEN];
short IsPlay1=1;
short IsPlay2=1;
SNDSTRUC snd1;
SNDSTRUC snd2;

const double scale=pow(2.0,1/12.0);
const double scale_24=pow(2.0,1/24.0);

static int show_debug_info=0;
extern SETTINGS Settings;
extern SOUND Sound;

const FREQ=22050;

struct RECORD_KEY
{
	unsigned long Time;
	char Scan;
	char Press;
};

const int MAX_REC=256;
int real_rec=0;
RECORD_KEY record[MAX_REC];

//*********************************************************//
//*********************************************************//
//*********************************************************//

struct MUSIC_KEY
{
	char Scan;	//скан-код клавише
	char Note;	//номер ноты (номер полутона)
	float Volume;	//играет
	float VolumeDecrement;	//скорость убывания
	float Freq;	//частота
	char Press;	//было ли нажатие
	//MUSIC_KEY()
	//{
	//	//Press=0;
	//}
};

char BaseFreqChange=0;

int mode_play=0;
int mode_rec=0;
int play_counter=0;

//*********************************************************//
//*********************************************************//
//*********************************************************//


struct FUNCTIONAL_KEY
{
	int ID;
	char * help;
	char ScanCode;
	char ScanCode2;
};

//*********************************************************//
//*********************************************************//
//*********************************************************//


enum{
	F_VOLUME_SUB=0,
	F_VOLUME_ADD,
	F_BASEFREQ_INPUT,
	F_BASEFREQ_SUB,
	F_BASEFREQ_ADD,
	F_DECREMENT_SUB,
	F_DECREMENT_ADD,
	F_TIMER_SUB,
	F_TIMER_ADD,
	F_CHANGE_TABLE,

	F_SHOW_HIDE_DEBUG,
	F_STROY,
	F_HELP,
	F_START_RECORD,
	F_STOP,
	F_PLAY_RECORD,
	F_EXIT
};

//*********************************************************//
//*********************************************************//
//*********************************************************//


FUNCTIONAL_KEY functional_keys[]=
{
//	{F_BASEFREQ_INPUT, "Ввод базовой частоты",KEY_F8},
	{F_HELP, "Помощь",						KEY_F1},
	{F_START_RECORD, "Запись ",				KEY_F2},
	{F_STOP, "Остановка ",		KEY_F3},
	{F_PLAY_RECORD, "Проиграть записанное ",	KEY_F4},
	{F_TIMER_SUB, "-- частоты таймера",	KEY_F5},
	{F_TIMER_ADD, "++ частоты таймера",	KEY_F6},

	{F_BASEFREQ_SUB, "-- базовой частоты",	KEY_PADSUB,KEY_LEFTCTRL},
	{F_BASEFREQ_ADD, "++ базовой частоты",	KEY_PADADD,KEY_LEFTCTRL},

	{F_STROY, "Изменение строя (нат./равн./24 пол)",KEY_F7},
	{F_CHANGE_TABLE, "Изменение таблицы (полутона/пентатоника)",KEY_F8},

	{F_VOLUME_SUB, "-- общей громкости",	KEY_F9},
	{F_VOLUME_ADD, "++ общей громкости",	KEY_F10},

	{F_DECREMENT_SUB, "-- декремента",		KEY_F11},
	{F_DECREMENT_ADD, "++ декремента",		KEY_F12},

	{F_SHOW_HIDE_DEBUG,"Показать/скрыть отладочную информацию",KEY_LEFTSHIFT,KEY_D},
	{F_EXIT, "Выход",KEY_ESC},

	{-1,"",-1}
};

//*********************************************************//
//*********************************************************//
//*********************************************************//


int GetFunctionalKeyScan(int _ID_)
{
	for(int i=0; ;i++)
	{
		int ID=functional_keys[i].ID;

		if (ID==-1) return 0;

		if (ID==_ID_)
		{
			return functional_keys[i].ScanCode;
		}
	}
}

//*********************************************************//
//*********************************************************//
//*********************************************************//


int GetFunctionalKeyScan2(int _ID_)
{
	for(int i=0; ;i++)
	{
		int ID=functional_keys[i].ID;

		if (ID==-1) return 0;

		if (ID==_ID_)
		{
			return functional_keys[i].ScanCode2;
		}
	}
}

//*********************************************************//
//*********************************************************//
//*********************************************************//


int KeybFunctional(int ID)
{
	int scan=GetFunctionalKeyScan(ID);
	if (scan==0) return 0;
	int scan2=GetFunctionalKeyScan2(ID);
	if (scan2==0) return Keyb[scan];

	return Keyb[scan] && Keyb[scan2];
}

//*********************************************************//
//*********************************************************//
//*********************************************************//


void KeybFunctionalClear(int ID)
{
	int scan=GetFunctionalKeyScan(ID);
	if (scan==0) return;
	Keyb.Set(scan,0);
}

//*********************************************************//
//*********************************************************//
//*********************************************************//


void ShowHelpPage()
{
	scr.clear();
	scr.x=0;
	scr.y=0;
	for(int i=0; ;i++)
	{
		if (functional_keys[i].ID==-1) break;
		scr<<KEYBOARD::KEY[functional_keys[i].ScanCode];
		if(functional_keys[i].ScanCode2)
		{
	        scr<<"+"<<KEYBOARD::KEY[functional_keys[i].ScanCode2];
		}
		scr<<" "<<functional_keys[i].help;
		scr<<"\n";
	}
	scr.update();
}

//данные для синтезатора
struct RECORD_DATA
{
	unsigned long Time;	//время события
	char Note;	//относительный номер тона
	char Stroy;	//строй (натуральный,равномерный)
	float StartVolume;  //начальная громкость (0..127)
	float Decrement; //относительная скорость убывания амплитуды (зависит от размера буфера)
	float Freq;	//абсолютная частота тона
};

const int MAX_KEY=64;

//*********************************************************//
//*********************************************************//
//*********************************************************//

//1-3-6-8-10
MUSIC_KEY table0[MAX_KEY]=
{
	{KEY_LQUOTE,0,0,0,0},
	{KEY_1,2,0,0,0},
	{KEY_2,4,0,0,0},
	{KEY_3,5,0,0,0},
	{KEY_4,7,0,0,0},
	{KEY_5,9,0,0,0},
	{KEY_6,11,0,0,0},
	{KEY_7,0+12,0,0,0},
	{KEY_8,2+12,0,0,0},
	{KEY_9,4+12,0,0,0},
	{KEY_0,5+12,0,0,0},
	{12,   7+12,0,0,0},
	{13,   9+12,0,0,0},
	{0x20+11,11+12,0,0,0},
	{14,     0+24,0,0,0},
	{KEY_INSERT,     2+24,0,0,0},
	{KEY_HOME,     4+24,0,0,0},
	{KEY_PAGEUP,     5+24,0,0,0},
	{KEY_NUMLOCK,     7+24,0,0,0},
	{KEY_PADDIV,     9+24,0,0,0},
	{KEY_PADMUL,     11+24,0,0,0},
	{KEY_TAB,     0+36,0,0,0},
	{KEY_Q,     2+36,0,0,0},
	{KEY_W,     4+36,0,0,0},
	{KEY_E,     5+36,0,0,0},

	{KEY_CAPSLOCK, 127,0,0,0},
	{KEY_LEFTSHIFT,127,0,0,0},

	{KEY_ENTER,     127,0,0,0},
	{255,     255,255,255,255}
};

//*********************************************************//
//*********************************************************//
//*********************************************************//


MUSIC_KEY table1[MAX_KEY]=
{
	{KEY_LQUOTE,1,0,0,0},
	{KEY_1,3,0,0,0},
	{KEY_2,6,0,0,0},
	{KEY_3,8,0,0,0},
	{KEY_4,10,0,0,0},
	{KEY_5,1+12,0,0,0},
	{KEY_6,3+12,0,0,0},
	{KEY_7,6+12,0,0,0},
	{KEY_8,8+12,0,0,0},
	{KEY_9,10+12,0,0,0},
	{KEY_0,1+12*2,0,0,0},
	{12,   3+12*2,0,0,0},
	{13,   6+12*2,0,0,0},
	{0x20+11,8+12*2,0,0,0},
	{14,     10+12*2,0,0,0},
	{KEY_INSERT,     1+12*3,0,0,0},
	{KEY_HOME,     3+12*3,0,0,0},
	{KEY_PAGEUP,     6+12*3,0,0,0},
	{KEY_NUMLOCK,     8+12*3,0,0,0},
	{KEY_PADDIV,     10+12*3,0,0,0},
	{KEY_PADMUL,     1+12*4,0,0,0},
	{KEY_TAB,     3+12*4,0,0,0},
	{KEY_Q,     6+12*4,0,0,0},
	{KEY_W,     8+12*4,0,0,0},
	{KEY_E,     10+12*4,0,0,0},

	{KEY_CAPSLOCK, 127,0,0,0},
	{KEY_LEFTSHIFT,127,0,0,0},

	{KEY_ENTER,     127,0,0,0},
	{255,     255,255,255,255}
};

//*********************************************************//
//*********************************************************//
//*********************************************************//

MUSIC_KEY table2[MAX_KEY]=
{
	{KEY_LQUOTE,0,0,0,0},
	{KEY_1,1,0,0,0},
	{KEY_2,2,0,0,0},
	{KEY_3,3,0,0,0},
	{KEY_4,4,0,0,0},
	{KEY_5,5,0,0,0},
	{KEY_6,6,0,0,0},
	{KEY_7,7,0,0,0},
	{KEY_8,8,0,0,0},
	{KEY_9,9,0,0,0},
	{KEY_0,10,0,0,0},
	{12,   11,0,0,0},
	{13,   12,0,0,0},
	{0x20+11,13,0,0,0},
	{14,     14,0,0,0},
	{KEY_INSERT,     15,0,0,0},
	{KEY_HOME,     16,0,0,0},
	{KEY_PAGEUP,     17,0,0,0},
	{KEY_NUMLOCK,     18,0,0,0},
	{KEY_PADDIV,     19,0,0,0},
	{KEY_PADMUL,     20,0,0,0},
	{KEY_TAB,     21,0,0,0},
	{KEY_Q,     22,0,0,0},
	{KEY_W,     23,0,0,0},
	{KEY_E,     24,0,0,0},
	{KEY_R,     25,0,0,0},

	{KEY_T,     26,0,0,0},
	{KEY_Y,     27,0,0,0},
	{KEY_U,     28,0,0,0},
	{KEY_I,     29,0,0,0},
	{KEY_O,     30,0,0,0},
	{KEY_P,     31,0,0,0},

	{KEY_A,     32,0,0,0},
	{KEY_S,     33,0,0,0},
	{KEY_D,     34,0,0,0},
	{KEY_F,     35,0,0,0},
	{KEY_G,     36,0,0,0},
	{KEY_H,     37,0,0,0},

	{KEY_J,     38,0,0,0},
	{KEY_K,     39,0,0,0},
	{KEY_L,     40,0,0,0},

	{KEY_Z,     41,0,0,0},
	{KEY_X,     42,0,0,0},
	{KEY_C,     43,0,0,0},

	{KEY_V,     44,0,0,0},
	{KEY_B,     45,0,0,0},
	{KEY_N,     46,0,0,0},
	{KEY_M,     47,0,0,0},


	{KEY_CAPSLOCK, 127,0,0,0},
	{KEY_LEFTSHIFT,127,0,0,0},

	{KEY_ENTER,     127,0,0,0},

	{255,     255,255,255,255}
};
static MUSIC_KEY *table=table2;

//*********************************************************//
//*********************************************************//
//*********************************************************//

void PlayFreq(int freq, int ampl, int )
{
	int indx=rand()%10;
	table[indx].Freq=freq;
	table[indx].Volume=ampl;
	table[indx].VolumeDecrement=0.004;
}

MUSIC_KEY old_keys[MAX_KEY];
static int trap=390;

//*********************************************************//
//*********************************************************//
//*********************************************************//

void GenerateSound()
{
	if (Settings.EnableSound==0) return;
	int over_load=0;
	SNDSTRUC *snd=&snd1;

	#define PI 3.145926

	float w=2.0*PI/FREQ;

	static int last_t=0;

	double freq;

	static unsigned long I_CNT=0;

	int dmac=SOUND_LEN+1-ReportDMAC();
//				static int dmac=SOUND_LEN+1;//-ReportDMAC();
	int start=0;

	int len=SOUND_LEN/2;

	static int buffer1_fill=0;
	static int buffer2_fill=0;

	static int i=0,p=0,j;


	if ((dmac>len) && (dmac<(len+trap))) start=0;
	else if (dmac>=0 && dmac<trap)start=SOUND_LEN/2;
	else start=0;//32767;

	if (start==0 && buffer1_fill) {return;}
	else if(start==(SOUND_LEN/2) &&buffer2_fill){return;}

	int max_line=60;

	for(i=start,p=1; i<start+len; i++,p++)
	{
		static double t=0;

		/*if (BaseFreqChange)
		{
			//int delta=t-last_value;
			static double abs_delta=fabs(t-last_value);
			if ( abs_delta>1 )
			{
				i=0;
				p=0;
				continue;
			}
			else BaseFreqChange=0;
		}*/


		for(j=0; j<MAX_KEY; j++)
		{
			if (table[j].Volume)
			{

				t+=

				table[j].Volume*sin(w*I_CNT*table[j].Freq);

				//sin(w*I_CNT*table[j].Freq);

				if ((table[j].Volume-=table[j].VolumeDecrement)<0)
				{
					table[j].Volume=0;
				}
			}
		}

		t*=Settings.Volume;

		 I_CNT++;

		 if (t>127)  {t=127; over_load++; }
		 if (t<-127) {t=-127; over_load++;}

		 snd->sound[i]=128+t;



		 if (p<320)
		 {
			if(show_debug_info&1)
			scr.line(p-1,100+last_t*max_line/127,p,100+t*max_line/127,t);
			last_t=t;
		 }


		 }
		if(show_debug_info&1)
		{
		 scr.line(0,100-max_line,320,100-max_line,3);
		 scr.line(0,100+max_line,320,100+max_line,3);
		}

		if (start==0) {buffer1_fill=1;buffer2_fill=0;}
		else if(start==(SOUND_LEN/2)){buffer1_fill=0;buffer2_fill=1;}

		if (over_load)
		{
			scr.color=12;
			//scr.rectangle(0,200-25,320,5,12);
			over_load=0;
		}
}

//*********************************************************//
//*********************************************************//
//*********************************************************//


void PutRec(int scan,unsigned long timer,int pr)
{
	//if(real_rec==0) Timer.Counter=0;
	record[real_rec].Scan=scan;
	record[real_rec].Press=pr;
	record[real_rec].Time=timer;
	real_rec++;
	mode_play=1;
	if (real_rec>=MAX_REC) real_rec=MAX_REC-1;

}

//*********************************************************//
//*********************************************************//
//*********************************************************//

void StartRecord()
{
	real_rec=0;
	mode_play=1;
	play_counter=0;
}
//*********************************************************//
//*********************************************************//
//*********************************************************//
void PutRecord(int scan,unsigned long timer)
{
	PutRec(scan,timer,1);
	PutRec(scan,timer+2,0);
}

//*********************************************************//
//*********************************************************//
//*********************************************************//
#define SOUND_MODULE_FULL 1
#define SOUND_MODULE_SHORT 0
void SoundModule(int param)
{
	if(Settings.EnableSound==0) return;
//	table=table2;

	if(param==SOUND_MODULE_FULL)
	{

	if (KeybFunctional(F_SHOW_HIDE_DEBUG))
	{
		show_debug_info++;
		KeybFunctionalClear(F_SHOW_HIDE_DEBUG);
	}

	if (KeybFunctional(F_CHANGE_TABLE))
	{
		if (table==table0) table=table1;
		else if(table==table1) table=table2;
		else table=table0;
		KeybFunctionalClear(F_CHANGE_TABLE);
	}
	if (KeybFunctional(F_TIMER_SUB))
	{
    	if(Timer.Freq>2)
		Timer.Restart(Timer.Freq-1);
		play_counter=0;
	}

	if (KeybFunctional(F_TIMER_ADD))
	{
		Timer.Restart(Timer.Freq+1);
		play_counter=0;
	}

	if (KeybFunctional(F_STOP))
	{
		mode_play=0;
		for(int z=0;z<MAX_KEY;z++) table[z].Press=0;
	}

	if (KeybFunctional(F_START_RECORD) && !mode_play)
	{
		KeybFunctionalClear(F_START_RECORD);
		if (mode_rec==1)
		{
			mode_rec=0;
		}
		else
		{
			mode_rec=1;
			real_rec=0;
			//Timer.Counter=0;
		}
	}

	if (KeybFunctional(F_PLAY_RECORD) && !mode_play)
	{
		mode_play=1;
		mode_rec=0;
		play_counter=0;
		Timer.Counter=0;
	}
	if (KeybFunctional(F_DECREMENT_SUB))
	{
		Settings.Decrement-=0.001;
		if (Settings.Decrement<0) Settings.Decrement=0;

	}

	if (KeybFunctional(F_DECREMENT_ADD))
	{
		Settings.Decrement+=0.001;
		if (Settings.Decrement>127) Settings.Decrement=127;

	}


	if (KeybFunctional(F_BASEFREQ_SUB))
	{
		Settings.BaseFreq-=1;
		BaseFreqChange=1;
	}

	if (KeybFunctional(F_BASEFREQ_ADD))
	{
		Settings.BaseFreq+=1;
		BaseFreqChange=1;
	}


	if (KeybFunctional(F_STROY))
	{
		KeybFunctionalClear(F_STROY);
		Settings.Stroy++;
		if (Settings.Stroy>=3) Settings.Stroy=0;

	}

	if (KeybFunctional(F_HELP))
	{
		ShowHelpPage();
		Keyb.ReadScan();
	}

	if (KeybFunctional(F_VOLUME_ADD))
	{
		Settings.Volume+=0.01;
		if (Settings.Volume>1) Settings.Volume=1;
	}

	if (KeybFunctional(F_VOLUME_SUB))
	{
		Settings.Volume-=0.01;
		if (Settings.Volume<0) Settings.Volume=0;
	}

	}

	{
			if (mode_play)
			{
				if (record[play_counter].Time<Timer.Counter)
				{
					Keyb.Set(record[play_counter].Scan,record[play_counter].Press);
					play_counter++;
					if (play_counter>=real_rec)
					{
						play_counter=0;
						mode_play=0;
						//Timer.Counter=0;
					}
				}
			}

			for(int z=0; z<MAX_KEY; z++)
			{
				if (Keyb[table[z].Scan]) table[z].Press=TRUE;
				else table[z].Press=FALSE;
			}

			//int KeysChange=0;
			for(z=0; z<MAX_KEY;z++)
			{
				if (old_keys[z].Press!=table[z].Press)
				{
					//KeysChange=1;
					old_keys[z]=table[z];

					if (mode_rec)
					{
						if(real_rec==0) Timer.Counter=0;
						record[real_rec].Scan=table[z].Scan;
						record[real_rec].Press=table[z].Press;
						record[real_rec].Time=Timer.Counter;
						real_rec++;
						if (real_rec>=MAX_REC) real_rec=MAX_REC-1;
					}

				}
			}

			for(int i=0; i<MAX_KEY;i++)
			{
				if (table[i].Press)
				{
					table[i].Volume=127;//Settings.Volume;
					table[i].VolumeDecrement=Settings.Decrement;
					//if (mode_rec || mode_play)
					//{
					//	Keyb.Set(table[i].Scan,0);
					//}
					//if (real_rec==0) Timer.Counter=0;


					//Keyb.Set(table[i][0],0);
					int NoteAdd=0;

					if(Keyb[KEY_CAPSLOCK])
					{
						NoteAdd=+1;
					}

					if(Keyb[KEY_LEFTSHIFT])
					{
						NoteAdd=-1;
					}

					if (Settings.Stroy==STROY_PIANO)
					{
						table[i].Freq=
						Settings.BaseFreq*pow(scale,table[i].Note+NoteAdd);
					}

					if(Settings.Stroy==STROY_NATURAL)
					{
						table[i].Freq=Settings.BaseFreq*(i+1);
					}


				   if (Settings.Stroy==STROY_24)
				   {
					table[i].Freq=Settings.BaseFreq*pow(scale_24,table[i].Note);
				   }

					if (table[i].Note==127) table[i].Freq=0;
				}
				else
				{
					if (table[i].Volume!=0)
					{
						table[i].Volume-=table[i].VolumeDecrement/100.0;

						if (table[i].Volume<0)
						{
							table[i].Volume=0;
						}
					}

				}

				if(param==SOUND_MODULE_FULL)
				{
				int h=30;
				int l=12;

				int x=i*l;
				int y=170-h;
				scr.x=i*l;
				scr.y=170-h-8;
				scr<<i;

				int clr=0;
				if (Settings.Stroy>0 && table==table2)
				{
				int b=1;
				if (Settings.Stroy==2) b=2;
				int d=i%(12*b);
				if (d==1*b || d==3*b || d==6*b || d== 8*b || d==10*b)
				{
					h-=2;
					clr=1;
				}
				}

				scr.rectangle(x,y,
							  l-1,h,
							  table[i].Volume+1+clr);
				}
			}



			short far * pending=NULL;
			pending=PendingAddress();
			short pending_status=AudioPendingStatus();

			if(param==SOUND_MODULE_FULL)
			{
			scr.color=15;
			if(show_debug_info&1)
			{
			scr.x=0;
			scr.y=0;
			scr.color=2;
			scr<<"Volume = "<<Settings.Volume<<"\n";
			scr<<"Decrement = "<<Settings.Decrement<<"\n";
			scr<<"Stroy = "<<Settings.Stroy;

			if(Settings.Stroy==STROY_NATURAL)scr<<" natural";
			if(Settings.Stroy==STROY_PIANO)scr<<" 12 tones";
			if(Settings.Stroy==STROY_24)scr<<" 24 tones";

			if (table==table0) scr<<" Table:Minor/Major\n";
			if (table==table1) scr<<" Table:Pentatonic\n";
			if (table==table2) scr<<" Table:Normal\n";

			scr<<"\nBaseFreq = "<<Settings.BaseFreq<<"\n";
			scr<<"Pending="<<(*pending)<<" "<<pending_status<<" "<<IsPlay1<<" "<<IsPlay2<<"   .";
					scr<<"\nDMAC="<<ReportDMAC()<<"\n";

			scr<<"\nTRAP="<<trap<<"(Z--,X++)\n";
			if (Keyb[KEY_Z]&&trap)trap--;
			if(Keyb[KEY_X])trap++;
			scr<<"FREQ="<<FREQ<<"\n";

			scr<<"\nSOUND_LEN="<<int(SOUND_LEN)<<"\n";
			scr<<"T.C="<<Timer.Counter<<"\n"<<"T.F="<<Timer.Freq<<"\n";

			}

			if (mode_rec)
			{
				if ((Timer.Counter&100)<50)
				{
					scr.color=12;
					scr<<"\nRECORD ";
					if(real_rec==0) scr<<" WAIT";
					else scr<<Timer.Counter<<" "<<real_rec;
				}
			}

			if (mode_play)
			{
				if ((Timer.Counter&100)<50)
				{
					scr.color=13;
					scr<<"\nPLAY "<<Timer.Counter<<" "<<play_counter;
				}
			}

			}

			snd1.sound=SOUND_BUF1;
			snd1.sndlen=SOUND_LEN;
			snd1.IsPlaying=&IsPlay1;
			snd1.frequency=FREQ;

			static int first=0;
			if (!first)
			{
				first=1;
				for(int i=0; i<SOUND_LEN;i++)
				{
					SOUND_BUF1[i]=127;
				}

				SetBackFillMode(1);
				PostAudioPending(&snd1);
				//PostAudioPending(&snd2);
				//DigPlay(&snd1);
			}

			GenerateSound();
			}
}

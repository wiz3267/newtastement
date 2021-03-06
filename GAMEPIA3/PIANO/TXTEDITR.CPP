#include "screen.h"
#include "keyboard.h"
#include "debug.h"
#include "settings.h"
#include <math.h>

#include <conio.h>
#include <string.h>
#include <fstream.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define FIND_FIRST 0
#define FIND_NEXT  1

//fun ////////////////////////////////////////////////
void	fun_key_f1();
void	fun_pad1();
void	fun_pad6();
void	fun_rnd_chapter();
void	fun_code13();
void	fun_show_cursor();
void	fun_pad0();
int		fun_delay();
int		open_text_book();
void	SoundModule(int param);
int		CheckWord(char *w);
void	ToLowerRegister(unsigned char * sym);
void	RestorePalette();
void	SavePalette();
void	fun_show_debug();
void	fun_code8();
void	fun_pad6();
void	fun_key_m_key_4();
void	fun_key_m_key_2();
void	fun_key_m_key_3();
void	fun_key_m_key_1();
void	fun_key_m_key_0();
void	fun_key_pad9();


//extern /////////////////////////////////////////////////////////////
extern 	SETTINGS Settings;
extern int 	BigSymScale;
extern long	filesize;
extern		ifstream ifl;

//global /////////////////////////////////////////////////////////
int 		cr=1;
int			word_count=0;
int			SymbolCount=1;
long		pause=1;
int			SHOW_DEBUG_TIME=20;
int			show_debug=0;//SHOW_DEBUG_TIME;
unsigned char big[]="��������������������������������";
char 		big_len=32;
char		Mode_Printer=0;
#define MODE0_CHAPTER_NORMAL 0
#define MODE1_CHAPTER_RND 1
char		Mode_RandomChangeChapter=MODE0_CHAPTER_NORMAL;
char		code13=0;
const int 	word_max=64;
unsigned char word[word_max];

char 		word_len=0;
unsigned long start_chapter_position=0;
const int	MAX_C=32;
char 		ZAGLAV[MAX_C+1]={0};
int		 	cursor_len;
int			cursor_hei;



//��४���� �� ⥡�, �⠭�!
//��ᯮ�� ��� �����⨢ �� ��� ��譮��!
void VideoEffect()
{
	SavePalette();

	scr.rectangle(0,0,320,200,2);
	scr.update();
	scr.pause(70);
	scr.rectangle(0,0,320,200,1);
	scr.update();


//	for(int i=0; i<70;i++)
	{
			scr.DecrementPalette();
			scr.wait();
			scr.wait();
			scr.wait();

			//if (Keyb[KEY_ESC]) break;
	}

	RestorePalette();

	Keyb.Set(KEY_ESC,0);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


void ColorChange()
{
		static double ugol=0;
		ugol+=0.05;
		int amp=18;
		scr.SetColorRegister(12,63-2*amp+amp*sin(ugol),0,0);

		/*for(int i=1;i<16;i++)
		{
			scr.SetColorRegister(i,63,63,63);
		}*/

}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

//���� �������� �⭮�⥫쭮 ⥪�饣� ���� ��ᠭ��
int find_zaglav()
{
	return 0;
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

long find(char flag=FIND_FIRST)
{
	static char str_find[21];

	//������ 1
	//���� �� ���⥫ﬨ ᫮�� � �ᯮ���⥫ﬨ
	//�ਧ��� �������� - ��ப� ������碠����� '0'..'9' � ����� ����� 13
	//�� �����, �� ���� ��� - ���� ⮫쪮 ����

	char Z[MAX_C+1]={0};

	int C=0;

	scr.color=0;
	if (flag==FIND_FIRST)
	{
		Msg(" ����:                     ");
		scr.InputString(20, str_find, 0);
	}
	else if (flag==FIND_NEXT)
	{
	//Msg(" ����:                      ");
	//scr<<str_find;
	}
	for(int i=0; i<strlen(str_find);i++)
	{
		ToLowerRegister(&str_find[i]);
	}

	if (Keyb[KEY_LEFTSHIFT])	ifl.seekg(filesize*0.8);

	//int find_first=1;

		int find_len=strlen(str_find);

		if (find_len>0)
		{
			int k=0;
			int find=0;

		    long i=ifl.tellg();
			long start_find_position=0;

			for(; i<filesize+1; i++)
			{
				if(Keyb[KEY_ESC])
				{
					scr.color=0;
					Msg(" �⬥���� ���짮��⥫�� ",1);
					scr.update();

					Keyb.ReadScan();
					scr.x=0;
					scr.y=0;
					scr.clear();
					ifl.seekg(0);
					break;
				}
				unsigned char sym;

				if(!ifl.read(&sym,1))
				{
					scr.color=0;
					Msg(" ���⨣��� ����� 䠩�� ",1);
					scr.update();

					Keyb.ReadScan();

					ifl.close();
					open_text_book();

					scr.clear();
					scr.x=0;
					scr.y=0;

					return FALSE;
				}

				if (C>=MAX_C) C=0;


				if(sym==13)
				{
					Z[C]=0;
					if (C>5)
					{

						if (Z[C-1]>='0' && Z[C-1]<='9')
						{
							strcpy(ZAGLAV,Z);
						}
					}
					C=0;
				}
				if(sym<32) sym=32;
				Z[C]=sym;
				C++;


				unsigned char from_file=str_find[k];

				ToLowerRegister(&sym);

				static long cnt=0;
				cnt++;
				if(cnt>1024*20 && flag!=FIND_NEXT)
				{
					cnt=0;
					scr.x=0;
					scr.y=0;
					scr.color=3;
					int proc=100L*(i/1024)/(filesize/1024);
					scr<<proc<<"%";
					scr.update();
				}


				if (sym==from_file)
				{
					if(find==0) start_find_position=ifl.tellg();
					find++;
					k++;
					if (find==find_len)
					{
						ifl.seekg(start_find_position-1);
						int t=1;
						while(1)
						{
						ifl.seekg(start_find_position-t);
						char s;
						ifl.read(&s,1);

						t++;
						if (s==13) {
						if(flag==FIND_FIRST)
						{
						scr.clear();
						scr.color=0;
						Msg(" �ᯥ譮 �������! �����⮢�� ... ",1);
						scr.update();
						scr.pause(70);
						scr.clear();
						scr.update();
						scr.pause(70);
						scr.x=0;
						scr.y=0;
						}
						else
						{
							scr<<"\n\n";
							if(scr.y>(200-8*4))
							{
								scr.x=0;
								scr.y=0;
								scr.clear();
							}

							scr<<ZAGLAV+2<<"\n";
						}

						break;

						}
						}

						break;
					}
				}
				else
				{
					find=0;
					k=0;
				}
			}
		  }

	return TRUE;

}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
int read_word()
{
	word_len=0;

	unsigned char z;
	do{

	ifl.read(&z,1);
	if(z=='*')
	{
		if (Mode_RandomChangeChapter==3)
		{
			Keyb.Set(KEY_PAD0,1);
			VideoEffect();
		}
		else
		{
			Keyb.Set(KEY_PAD1,1);

			if((rand()%2)==0) Keyb.Set(KEY_LEFTCTRL,1);

			VideoEffect();
		}

	}

	if(z==32)
	{
		unsigned char k;
		ifl.read(&k,1);
		long g=ifl.tellg();
		ifl.seekg(g-1);

		if (k>='0'&&k<='9')
		{

		}
		else
		{
			return word_len;
		}
	}

	if(z==13){code13=1;return word_len;}
	if(z<32) continue;

	word[word_len]=z;
	word[word_len+1]=0;
	word_len++;

	if(word_len>=word_max) return word_len;

	}while(1);

}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void NewLine()
{
		scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);
		scr.x=0,scr.y+=cursor_hei;

		if (scr.y>(200-cursor_hei*2))
		{
		   //	for(int v=0;v<cursor_hei;v++)
	   //	{
				scr.ScrollUp(cursor_hei);
				//scr.wait();
				scr.update();
			//}
			scr.y-=cursor_hei;
		}

}

void PrintZaglav()
{
			int x=scr.x;
			int y=scr.y;

			int l=strlen(ZAGLAV)*8;

			scr.x=320-l+1;
			scr.y=1;

			scr.color=16;
			scr.bkcolor=0;
			scr<<ZAGLAV;

			scr.x=320-l;
			scr.y=0;

			scr.color=15;
			scr.bkcolor=0;
			scr<<ZAGLAV;

			scr.x=x;
			scr.y=y;
}

void step_back()
{
		long position=0;
		char sym=0;

		position=ifl.tellg();

		while(1)
		{
			ifl.seekg(position);
			ifl.read(&sym,1);
			if (sym==13) break;
			position--;
		}

		position--;

		while(1)
		{
			ifl.seekg(position);
			ifl.read(&sym,1);
			if (sym==13) break;
			position--;
		}

		ifl.read(&sym,1);
		word_len=0;

		code13=0;
		scr.x=0;
		//pause=0;
		//word_count=0;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void TextEditor()
{
	randomize();

	scr.clear();
	scr.x=0;
	scr.y=0;

	Keyb.Set(KEY_L,0);
	//Keyb.Set(KEY_PAD0,1);

	Keyb.Set(KEY_ENTER,0);
	scr.SetColorRegister(2,0,63,0);

	ifl.seekg(Settings.LastFilePosition);

	do {

	if (Keyb[KEY_LEFTSHIFT] && Keyb[KEY_F])
	{
    	Keyb.Set(KEY_F,0);
		scr.flip^=1;
	}

	SoundModule(0);

	if(!Mode_Printer)
	{
		if(Keyb[KEY_L])
		{
			find(FIND_NEXT);
			Keyb.Set(KEY_L,0);
		}
		if(Keyb[KEY_F])
		{
			find(FIND_FIRST);
			word_count=0;
		}
	}

	char color=1;

	if (Keyb[KEY_PAGEUP]){Keyb.Set(KEY_PAGEUP,0);BigSymScale++;pause=0;}
	if (Keyb[KEY_PAGEDOWN]){Keyb.Set(KEY_PAGEDOWN,0);BigSymScale--;pause=0;}

	if(BigSymScale<1)BigSymScale=1;


	cursor_len=BigSymScale*6;
	cursor_hei=BigSymScale*8;

	if (Keyb[KEY_ESC])
	{
		Keyb.Set(KEY_ESC,0);
		Settings.Save();
		return;
	}

	static int timer=0;
	timer++;


	int DELAY=150;

	if ((timer%DELAY)>(DELAY/2)) cr=0;
	if(Settings.Pause_Symbol==0) cr=0;

	scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,cr);
	PrintZaglav();
	scr.update();

	/*if(Keyb[KEY_1])
	{
		scr.rectangle(0,0,320,200,0);
		scr.update();
		scr.pause(70*60*5);


		while(1)
		{
		scr.rectangle(0,0,320,200,0);
		scr.update();
		scr.pause(70);

		scr.rectangle(0,0,320,200,15);
		scr.update();
		scr.pause(70);

		if (Keyb[KEY_ESC]) break;
		}

	} */

	if(Keyb[KEY_ENTER] && !Mode_Printer)
	{
		if(pause<0) pause=0;
		else pause=-1;
		Keyb.Set(KEY_ENTER,0);
	}

	static delay=Settings.Pause_Symbol;

	int CheckWordFlag=0;

	if (Keyb[KEY_F1]) fun_key_f1();

	if (Keyb[KEY_LEFTCTRL])
	{
		if (Keyb[KEY_LEFT])
		{
			Settings.Pause_Enter--;
			Keyb.Set(KEY_LEFT,0);
			pause=0;
			show_debug=SHOW_DEBUG_TIME;
		}

		if (Keyb[KEY_RIGHT])
		{
			Settings.Pause_Enter++;
			Keyb.Set(KEY_RIGHT,0);
			pause=0;
			show_debug=SHOW_DEBUG_TIME;
		}
		if (Settings.Pause_Enter<=0) Settings.Pause_Enter=1;

		Keyb.Set(KEY_LEFT,0);
		Keyb.Set(KEY_RIGHT,0);

	}
	else
	{
		if (Keyb[KEY_LEFT])
		{
			delay--;
			Keyb.Set(KEY_LEFT,0);
			pause=0;
			show_debug=SHOW_DEBUG_TIME;

		}

		if (Keyb[KEY_RIGHT])
		{
			delay++;
			Keyb.Set(KEY_RIGHT,0);
			pause=0;
			show_debug=SHOW_DEBUG_TIME;
		}

		if (delay<=0) delay=0;

		Settings.Pause_Symbol=delay;

		Keyb.Set(KEY_LEFT,0);
		Keyb.Set(KEY_RIGHT,0);
	}

	if (show_debug)	fun_show_debug();

	if (pause)
	{
		if(Keyb[KEY_PAD1]) { pause=1; timer=0; }

		pause--;
		if(pause==0)
		{
			//scr.clear();
			//scr.x=0;
			//!!!!!!!!!!!!!!1
			//scr.y=24+24+24;
			NewLine();
		}

		SoundModule(0);
		ColorChange();
		scr.wait();
		scr.update();
		continue;
	}

	timer=0;
	static int word_color;
	unsigned char code;

	if (Mode_Printer)
	{
		Keyb.StandartInput();
		//if (kbhit())
		while(1)
		{
			if(!kbhit())
			{
				SoundModule(0);
				continue;
			}
			unsigned char g=getch();

			ofstream ofl;
			ofl.open("out.txt", ios::app);
			if (g==8)
			{
				int pos=ofl.tellp();
				pos-=1;
				ofl.seekp(pos);

                ofl.close();
			}

			if (g>=32)
			{
				ofl.write(&g,1);
				ofl.close();
			}

			if (g==13)
			{
				char cv='\n';
				ofl.write(&cv,1);
				ofl.close();
			}

			word[0]=g;
			word_len=1;

			if (g==13) code13=1;
			break;

		}
		Keyb.SuperInput();
	}
	else
	{
		word_color=26;
		read_word();
		CheckWordFlag=CheckWord(word);
		word_count++;
	}

	if (Keyb[KEY_PAD7]) Mode_Printer=1;
	if (Keyb[KEY_PAD8])	Mode_Printer=0;
	if (Keyb[KEY_M] && Keyb[KEY_4]) fun_key_m_key_4();
	if (Keyb[KEY_M] && Keyb[KEY_2]) fun_key_m_key_2();
	if (Keyb[KEY_M] && Keyb[KEY_3]) fun_key_m_key_3();
	if (Keyb[KEY_M] && Keyb[KEY_1]) fun_key_m_key_1();
	if (Keyb[KEY_M] && Keyb[KEY_0]) fun_key_m_key_0();
	if (Keyb[KEY_PAD9]) fun_key_pad9();

	//�஬���� ���। �� ᫥���饩 �����
	//if(Keyb[KEY_PAD6]) fun_pad6();

	if(Keyb[KEY_PAD4])
	{
		Keyb.Set(KEY_PAD4,0);

		if (Keyb[KEY_LEFTCTRL])
		{
			unsigned long pos=ifl.tellg();
			ifl.seekg(pos-512*8);
			Keyb.Set(KEY_PAD6,1);
		}
		else
		{
			//����� �� ���� ���
			step_back(); word_count=0;
		}
	}

	//��砩��� ����
	//if(Keyb[KEY_PAD1]) fun_pad1();

	code=word[0];


	if(word_len==1 && code=='#'){ Keyb.Set(KEY_PAD0,1);	pause=100; continue;}


	if(Keyb[KEY_PAD6])
	{
		fun_pad6();
	}


	if(Keyb[KEY_PAD1])
	{
		scr.clear();
		scr.update();
		scr.pause(20);


		Keyb.Set(KEY_PAD1,0);
		while(1)
		{
		long kb=filesize/1024L;
		long rand_pos=(rand()%kb) * 1024L;
		if (Keyb[KEY_LEFTCTRL])
		{
			if (rand_pos < filesize*0.8) continue;
		}

		ifl.seekg(rand_pos);
		break;
		}

		long position=ifl.tellg();

		while(1)
		{
			if(Keyb[KEY_ESC]) break;
			position=ifl.tellg();
			int len=read_word();
			if(len<3) continue;
			char e=word[word_len-1];
			if(e>='0'&& e<='9')
			{
				strcpy(ZAGLAV,word);
				//if(ZAGLAV[0]!='�') Keyb.Set(KEY_PAD1,1);

				break;
			}
		}


		char sym=0;
		while(1)
		{
		ifl.seekg(position);
		ifl.read(&sym,1);
		if (sym==13) break;
		position--;
		}

		word_len=0;

		scr.x=0;
		scr.y=0;
		scr.clear();
		pause=0;
		code13=0;
		word_count=0;

	}

	code=word[0];

	if(word_len==1 && code=='#') {pause=-1;continue;}

	if (Mode_RandomChangeChapter && (word_len>5) && word_count>5)
	{
		char e=word[word_len-1];
		if(e>='0'&& e<='9')
		{
			/*if (Keyb.ReadScan() != KEY_SPACE)
			{
			scr.clear();
			scr.update();
			scr.pause(70);
			pause=0;
			continue;
			}*/

			scr.update();
			scr.pause(70);
			if(Mode_RandomChangeChapter==2) Keyb.Set(KEY_PAD0,1);
			else Keyb.Set(KEY_PAD1,1);
		}
	}


	if (Mode_RandomChangeChapter && (word_len>5) && word_count>5)
	{
		//fun_rnd_chapter();
	}

	if((scr.x+BigSymScale*6*word_len)>320)	NewLine();
	if (code==8) fun_code8();

	{
		color=2;
		for(int n=0; n<big_len; n++) if (big[n]==code) color=9;
	}

	if (code>='0' && code<='9') color=5;

	scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);

	if(Keyb[KEY_PAD0]) {   fun_pad0();	continue;	}

	if (word_len > 4) {
		char e=word[word_len-1];
		if(e>='0'&& e<='9') strcpy(ZAGLAV,word);
	}

	if(word[0]=='&' || word[1]=='&') color=2;

	for(int t=0; t<word_len; t++)
	{
		if(word[t]<32) continue;
		PrintZaglav();
		if(CheckWordFlag>0)	color=12;
		if (word[t]=='&')	color=12;
		scr.putBigSym(word[t],color,28);
		if (delay>0) fun_show_cursor();

		for(int i=0; i<delay; i++) if (fun_delay()) break;

		scr.update();


		SymbolCount++;
	}

	if(!Mode_Printer)	scr.putBigSym(' ',color,3);

	if (code13)	fun_code13();

	code13=0;

	SoundModule(0);

	scr.update();

	Settings.LastFilePosition=ifl.tellg();

}while(1);
}

///////////////////////////////////////////////////////////////
//ᮣ��襭��
// InputString - ����� �㭪権
// class STRING - �������� �����
		const int WORDS_GOD_COUNT=5+8+3+6+7+5;
		char *words_God[WORDS_GOD_COUNT]=
		{
		//5
		"����� ",
		"���� ",
		"���� ",
		"���� ",
		"��� ",

		//8
		"��� ",
		"����⢮ ",
		"����⢨� ",
		"����᭮� ",
		"���� ",
		"���⮣� ",
		"���⮩ ",
		"����� ",


		//3
		"��宬 ",
		"��� ",
		"��� ",

		//6
		"��楬 ",
		"��� ",
		"��� ",

		"��� ",
		"��� ",
		"���� ",

		//7
		"������ ",
		"����� ",
		"����� ",

		"����� ",
		"�����",
		"����� ",
		"����� ",

		//5
		"����� ",
		"���஬� ",
		"���ண� ",
		"�ᥤ�ন⥫� ",
		"������ "

		};

		const int WORDS_COUNT=2+14+ 11+2 +12+3;
		char * words[WORDS_COUNT]=
		{
			//2
			"�������᪨� ",
			"�������᪮�� ",

			//14
			"����� ",
			"������ ",
			"���� ",
			"���� ",
			"��� ",
			"��� ",

			"��� ",
			"��� ",
			"��� ",
			"��� ",
			"��� ",
			"��� ",
			"��� ",
			"���� ",

			//11+2
			"������ ",
			"������ ",
			"������ ",
			"����� ",
			"����� ",
			"�����",
			"��� ",
			"����騩 ",

			"���� ",
			"���� ",
			"���� ",
			"���� ",
			"��� ",


			//12
			"����� ",
			"���� ",
			"���� ",
			"���� ",
			"���� ",
			"����� ",
			"���� ",
			"��� ",

			"���� ",
			"���� ",
			"����� ",
			"���� ",

			//3
			"��� ",
			"�� ",
			"�� "


		};

		const int WORDS_IISUS_COUNT=2+5+6+8+5+2;
		char * words_IISUS[WORDS_IISUS_COUNT]=
		{
			//2
			"� ",

			//5
			"����ᮬ ",
			"����� ",
			"����� ",
			"������ ",
			"����� ",

			//6
			"���⮢� ",
			"���⮢� ",
			"���⮬ ",
			"����� ",
			"���� ",
			"���� ",
			"����� ",

			//8

			"��ᯮ�� ",
			"��ᯮ�� ",
			"��ᯮ�� ",
			"��ᯮ�� ",

			"��ᯮ�� ",
			"��ᯮ���� ",
			"��ᯮ��� ",
			"��ᯮ��� ",

			//5
			"�뭮� ",
			"��� ",
			"�뭠 ",
			"�뭥 ",
			"�� ",

			//2
			"����� ",
			"����� "
		};

///////////////////////////////////////////////////////////////
int min(int a, int b)
{
	if (a<b) return a;
	else return b;
}

///////////////////////////////////////////////////////////////
int Check(char * w1, char * w2)
{
	int l1=strlen(w1);
	int l2=strlen(w2);

	int cnt=0;

	int m=min(l1,l2);
	for(int i=0; i<m; i++)
	{
		if(w1[i]==w2[i]) cnt++;
	}

	if ((cnt==l2) || cnt==(l2-1)) return 1;

	else return 0;
}

///////////////////////////////////////////////////////////////
int CheckWord(char *w)
{
	int i=0;
	for(i=0; i<WORDS_GOD_COUNT; i++)	if(Check(w, words_God[i])) return 1;
	for(i=0; i<WORDS_COUNT; i++)		if(Check(w, words[i])) return 2;
	for(i=0; i<WORDS_IISUS_COUNT; i++)	if(Check(w, words_IISUS[i])) return 3;
	return 0;
}
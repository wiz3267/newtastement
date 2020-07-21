#include <fstream.h>

#include "sprite.h"
#include "debug.h"
#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include "sound.h"
#include "math.h"
#include "string.h"
#include "rectable.h"

#include "glass.h"
#include "figure.h"
#include "tetris.h"

#include <stdio.h>


#include "midpak2.h"
#include <stdlib.h>

#include "settings.h"

void graph_draw_num(int x, int y, unsigned long num);
void GetRealTimeClock(unsigned char &hour, unsigned char &min, unsigned char &sec);

unsigned long RunLX=0;

int show_task_list=TRUE;
void music(int n, int seq);
extern int CURRENT_MUSIC;
extern SPRITE SpriteBox;
extern SPRITE SpriteFigure;
extern SPRITE Sprite;
extern SPRITE Background;
extern float BaseSpeed;
extern float BaseAccelerat;
extern RECTABLE Rectable;



//загрузка спрайтов
void load_sprites()
{
	SPRITE::IMAGE im;
	im.Attribute = 0;

	//загружаем задний план
	{
		im.Index=0;
		Background.LoadSpriteform("graph\\backgrnd.sp0", im);
		im.Index=1;
		Background.LoadSpriteform("graph\\backgrnd.sp1", im);
		im.Index=2;
		Background.LoadSpriteform("graph\\backgrnd.sp2", im);
		im.Index=3;
		Background.LoadSpriteform("graph\\backgrnd.sp3", im);
		im.Index=4;
		Background.LoadSpriteform("graph\\backgrnd.sp4", im);
		im.Index=5;
		Background.LoadSpriteform("graph\\backgrnd.sp5", im);
		im.Index=6;
		Background.LoadSpriteform("graph\\backgrnd.sp6", im);
		im.Index=7;
		Background.LoadSpriteform("graph\\backgrnd.sp7", im);
		im.Index=8;
		Background.LoadSpriteform("graph\\backgrnd.sp8", im);
		im.Index=9;
		Background.LoadSpriteform("graph\\backgrnd.sp9", im);
	}

	//загружаем квадратики
	for(int i=1; i<8; i++)
	{
		im.Attribute = 0;
		im.Index = i;
		SpriteBox.LoadSpriteform( "graph\\brick.sp1", im);
		SpriteBox.ModifyColor(i, (i-1)*16);
	}

	//загружаем указатель
	{
		im.Attribute = SCREEN::TRANSP;
		im.Index = SPRITE_POINTER;
		Sprite.LoadSpriteform( "graph\\pointer.sp0", im);
	}

	im.Index=SPRITE_BORDER;
	SpriteBox.LoadSpriteform("graph\\border.sp0",im);

	im.Index=SPRITE_AUTORNAME;
	Sprite.LoadSpriteform("graph\\autor.sp0",im);

	//загружаем фигурки
	{
		im.Index=0;
		SpriteFigure.LoadSpriteform("graph\\box.sp0", im);

		im.Index=1;
		SpriteFigure.LoadSpriteform("graph\\box.sp1", im);

		im.Index=2;
		SpriteFigure.LoadSpriteform("graph\\box.sp2", im);

		im.Index=3;
		SpriteFigure.LoadSpriteform("graph\\box.sp3", im);


		im.Index=4;

		SpriteFigure.LoadSpriteform("graph\\box.sp4", im);

		im.Index=5;

		SpriteFigure.LoadSpriteform("graph\\box.sp5", im);
		im.Index=6;

		SpriteFigure.LoadSpriteform("graph\\box.sp6", im);
		im.Index=7;

		SpriteFigure.LoadSpriteform("graph\\box.sp7", im);

		im.Index=8;
		SpriteFigure.LoadSpriteform("graph\\box1.sp0", im);

		im.Index=9;
		SpriteFigure.LoadSpriteform("graph\\box1.sp1", im);

		im.Index=10;
		SpriteFigure.LoadSpriteform("graph\\box1.sp2", im);

		im.Index=11;
		SpriteFigure.LoadSpriteform("graph\\box1.sp3", im);

		im.Index=12;
		SpriteFigure.LoadSpriteform("graph\\box1.sp4", im);

		im.Index=13;
		SpriteFigure.LoadSpriteform("graph\\box1.sp5", im);

		im.Index=14;
		SpriteFigure.LoadSpriteform("graph\\box1.sp6", im);

		im.Index=15;
		SpriteFigure.LoadSpriteform("graph\\box1.sp7", im);

		im.Index=16;
		SpriteFigure.LoadSpriteform("graph\\box2.sp0", im);

		im.Index=17;
		SpriteFigure.LoadSpriteform("graph\\box2.sp1", im);

		im.Index=18;
		SpriteFigure.LoadSpriteform("graph\\box2.sp2", im);

		im.Index=19;
		SpriteFigure.LoadSpriteform("graph\\box2.sp3", im);

		im.Index=20;
		SpriteFigure.LoadSpriteform("graph\\box2.sp4", im);

		im.Index=21;
		SpriteFigure.LoadSpriteform("graph\\box2.sp5", im);

		im.Index=22;
		SpriteFigure.LoadSpriteform("graph\\box2.sp6", im);

		im.Index=23;
		SpriteFigure.LoadSpriteform("graph\\box2.sp7", im);

		im.Index=24;
		SpriteFigure.LoadSpriteform("graph\\box3.sp0", im);

		im.Index=25;
		SpriteFigure.LoadSpriteform("graph\\box3.sp1", im);

		im.Index=26;
		SpriteFigure.LoadSpriteform("graph\\box3.sp2", im);

		im.Index=27;
		SpriteFigure.LoadSpriteform("graph\\box3.sp3", im);

	}

	im.Index=SPRITE_TEST;
	im.Attribute=0;
	Sprite.LoadSpriteform("graph\\test.sp0", im);

	im.Index=SPRITE_STAR;
	im.Attribute=0;
	Sprite.LoadSpriteform("graph\\star.sp0", im);


}

extern int 		BigSymScale;
int 			StepRunLX=1;
extern 			SETTINGS Settings;
int 			text_loop=0;
unsigned char	str_find[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int 			end_of_file=0;
static char 	ZAGLAVIE[40]={0};
const int 		FILE_BUF_SIZE=70;
char 			file_buf[FILE_BUF_SIZE];
ifstream 		ifl;
long 			filesize=0;
int 			METHOD_NEXT=1;
int 			METHOD_BACK=0;

long FindZaglavie(long start_pos, int method)
{
	return 0;
	int step_k=-1;

	if (method==METHOD_BACK)
	{
		step_k=-1;
	}

	if (method==METHOD_NEXT)
	{
		step_k=1;
	}

	for(long k=start_pos; ; k+=step_k)
	{
		if (k<=0) break;
		if (k>=filesize) break;

		char S;
		ifl.seekg(k);
		ifl.read(&S,1);

		if (S!='(') continue;

		char V;
		ZAGLAVIE[0]='(';

		for(int b=1; b<39; b++)
		{
			ifl.read(&V, 1);
			ZAGLAVIE[b]=V;
			ZAGLAVIE[b+1]=0;

			if (V==')') break;
		}

		break;
	}

	return k;
}

char rus1[]="ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
char rus2[]="йцукенгшщзхъфывапролджэячсмитьбю";
int rus_size=sizeof(rus1);
									 
//		Fun(sym0, sym1, sym2, sym3, "Отец", &t, &x, &_x, &_step_x, 13, &y);

int Fun2(char * AllText, char * Compare, int Len,
	int * t, int * _x, int * _step_x, char color, int *y )
{
	int c=0;
	char src;
	char syms;

	for(int i=0; i<Len; i++)
	{
		src = Compare[i];
		syms = AllText[i];

		if (src==' ')
		{

			if (		   syms == ' '
						|| syms == ','
						|| syms == '.'
						|| syms == '!'
						|| syms == '?'
						|| syms == ';'
						|| syms == ':'
						|| syms == '('
						|| syms == ')'
						|| syms == '{'
						|| syms == '}'
						|| syms == '|'
						|| syms == '\\'
						|| syms == '/'
						|| syms == '<'
						|| syms == '>'
						|| syms == '\"'

			  )
			{
				c++;
				continue;
			}
		 }
		if (src == syms) c++;
	}

	if (c==Len)
	{
		for(int i=0; i<Len; i++)
		{
			src=Compare[i];
			syms=AllText[i];
			char clr=color;

			if (src == ' ')
			{
				clr=15;
			}

			scr.locateXY(*_x + *_step_x * i, *y);
			scr.putBigSym(syms, clr, 0);

		}

		*t += Len-1 ;

		return 1;

	}
	return 0;
}

void ToLowerRegister(unsigned char * sym)
{
	for(int i=0; i<rus_size; i++)
	{
		if (rus1[i] == *sym)
		{
			*sym=rus2[i];
			break;
		}
	}
}

int need_find=FALSE;

int open_text_book()
{
	ifl.open("NOVIZAVT\\all_newz.txt", ifl.binary);
	if (!ifl) return FALSE;
	else return TRUE;
}

int DrawRunLine(int y)
{
	static int first_open=0;

	if (!first_open)
	{
		filesize=0;

		if (!open_text_book()) return 0;

		first_open=1;

		ios::seek_dir sd = ios::end;
		ifl.seekg(0, sd);
		filesize=ifl.tellg();
		ifl.seekg(0);
	}

	#define SEEK_NONE 0
	#define SEEK_NEXT 1
	#define SEEK_PREV 2
	#define SEEK_APPLY 3
	#define SEEK_EXIT 4

	int seek=SEEK_NONE;

	{
		static int screen_off=1;
		if (screen_off)
		{
			screen_off=0;
			Keyb.Set(KEY_ENTER,1);
		}

	}

	if (Keyb[KEY_LEFTCTRL] || Keyb[KEY_RIGHTCTRL])
	{
		/*if (Keyb[KEY_F1])
		{
			ifl.seekg(16384,ios::seek_dir(0));

			do {
			int begin=FALSE,end=FALSE;
			char str[80];
			int str_index=0;
			scr.clear();
			scr.x=0;
			scr.y=0;
			for(int p=0; p<80;p++) str[p]=0;
			do {
			unsigned char sym;
			if (!ifl.read(&sym,1)) break;

			if (sym=='(')
			{
				begin=TRUE;
				ifl.read(&sym,1);
			}
			if (sym==')' || (str_index>75))
			{
				scr<<str<<"\n";
				begin=FALSE;
				str_index=0;
				scr.update();
			}
			if (begin) {
				str[str_index]=sym;
				str[str_index+1]=0;
				str_index++;

			}
			if (scr.y>200) break;
			} while(1);

			if (Keyb.ReadScan()==KEY_ESC) break;
			} while(1);
		}
		*/
		if (Keyb[KEY_LEFTSHIFT] && Keyb[KEY_F])
		{
			scr.flip^=1;
			Keyb.Set(KEY_F,0);
			RunLX=1;
		}

		if (Keyb[KEY_PAD0]) RunLX=1;
		if (Keyb[KEY_PAD1]) { Keyb.Set(KEY_PAD1,0); RunLX=0;		}
		if (Keyb[KEY_PAD4]) { Keyb.Set(KEY_PAD4,0); seek=SEEK_PREV; }
		if (Keyb[KEY_PAD6]) { Keyb.Set(KEY_PAD6,0); seek=SEEK_NEXT;	}
		if (Keyb[KEY_PAD7]) { Keyb.Set(KEY_PAD7,0); text_loop^=1;	}
		if (Keyb[KEY_PAD8]) { Keyb.Set(KEY_PAD8,0); need_find^=1;	}
		if (Keyb[KEY_LEFT])  { StepRunLX--; Keyb.Set(KEY_LEFT,0); }
		if (Keyb[KEY_RIGHT]) { StepRunLX++; Keyb.Set(KEY_RIGHT,0);}



		if (Keyb[KEY_PAGEUP])
		{
			if (BigSymScale<10)
			{
				BigSymScale++;
				Settings.BigSymScale=BigSymScale;
				Settings.Save();
			}

			Keyb.Set(KEY_PAGEUP,0);
		}

		if (Keyb[KEY_PAGEDOWN])
		{
		if (BigSymScale>1)
		{
			BigSymScale--;
			Settings.BigSymScale=BigSymScale;
			Settings.Save();
		}

		Keyb.Set(KEY_PAGEDOWN,0);
	}
	}

	int find_first=0;

	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_F])
	{
		Msg("Найти:                     ");
		scr.InputString(20, str_find, 0);
		Keyb.Set(KEY_L,1);
		Keyb.Set(KEY_LEFTCTRL,1);
		find_first=1;
		Keyb.Set(KEY_ENTER,0);
	}

	static first_find=0;

//	if (first_find)
//	{
//		strcpy(str_find, "иоан.2");
//		RunLX=0;
//	}
	if ( (Keyb[KEY_LEFTCTRL] && Keyb[KEY_L]) || first_find)
	{
		first_find=0;
		if (end_of_file)
		{
			end_of_file=0;
			RunLX=0;
		}

		Keyb.Set(KEY_L,0);

		int find_len=strlen(str_find);

		if (find_len>0)
		{
			if (!find_first)
			{
			char stext[128];
			strcpy(stext, "Поиск: <");
			strcat(stext, str_find);
			strcat(stext, "> ");
			Msg(stext);
			scr.update();
			}

			unsigned long i=RunLX/scr.StepX/BigSymScale;
			long old_RunLX=RunLX;
			long seek_old=i;
			ifl.seekg(i);

			int k=0;
			int find=0;

			for(; i<filesize; i++)
			{
				Keyb.Set(KEY_LEFTCTRL,0);
				if(Keyb[KEY_ESC])
				{
					Keyb.Set(KEY_ESC,0);
					RunLX=old_RunLX;
					ifl.seekg(seek_old);
					break;
				}
				unsigned char sym;
				ifl.read(&sym,1);

				unsigned char from_file=str_find[k];

				ToLowerRegister(&sym);
				ToLowerRegister(&from_file);

				if (sym==from_file)
				{
					find++;
					k++;
					if (find==find_len)
					{
						RunLX=(i-find_len-30)*BigSymScale*scr.StepX;
						break;
					}
				}
				else
				{
					find=0;
					k=0;
				}
			}

			if (find != find_len)
			{
				Msg(" ------- НЕ НАЙДЕНО ------- ",3);
				end_of_file=1;
				scr.update();




				Keyb.ReadScan();
			}
			else
			{
				//FindZaglavie(i, METHOD_NEXT);
			}
		  }
	}

	if (BigSymScale<1) BigSymScale=1;
	if (BigSymScale>10) BigSymScale=10;

//	int nofile=1;

	unsigned long position=0;

	position=RunLX/scr.StepX/BigSymScale;

	if (position > (filesize-64))
	{
		position=0;
		RunLX=0;
	}

	if (seek==SEEK_PREV)
	{
		position=FindZaglavie(position, METHOD_BACK);
		position=FindZaglavie(position-10, METHOD_BACK);
		RunLX=position*scr.StepX*BigSymScale;
	}

	//поиск заглавия
	if (seek==SEEK_NEXT)
	{
		position=FindZaglavie(position, METHOD_NEXT);
		RunLX=position*scr.StepX*BigSymScale;
	}

	seek=SEEK_NONE;

	static long last_position=0;

	position=RunLX/scr.StepX/BigSymScale;
	if (last_position != position)
	{
		ifl.seekg(position);
		ifl.read(file_buf,FILE_BUF_SIZE);
		last_position=position;
	}

	static int find_zagl=0;

	if (find_zagl) find_zagl--;
	else if (file_buf[0]=='(')
	{
		FindZaglavie(position+1, METHOD_NEXT);
		find_zagl=100;
	}

	for(int i=0; i<FILE_BUF_SIZE; i++)
	{
		char a=file_buf[i];

		if (a==13 || a==10 || a==9) a=32;
		if (a=='*') RunLX=0;

		file_buf[i]=a;
	}
	if (RunLX==0)
	{
		randomize();

		unsigned long kb=filesize/1024L;

		RunLX=1024L*random(kb)*BigSymScale*scr.StepX;
		seek=SEEK_NEXT;
	}


	static int basecolor=RunLX;

	int _step_x=BigSymScale*scr.StepX;
//	int _max_x=320+BigSymScale*scr.StepX;

	//int len=FILE_BUF_SIZE;

	int offset=10;
	char *file_src=file_buf+offset;

//	int need_flash_color=TRUE;

	for(int t=-offset+1; t<(FILE_BUF_SIZE-offset); t++)
	{
		int _x=_step_x*t -(RunLX%(BigSymScale*scr.StepX));

		if (_x > 320) break;

		const int WORDS_GOD_COUNT=9+3+6+7+5;
		char *words_God[WORDS_GOD_COUNT]=
		{
		//9
		"Богом ",
		"Боге ",
		"Богу ",
		"Бога ",
		"Бог ",

		"Сии ",
		"Царство ",
		"Небесное ",
		"Свете ",

		//3
		"Духом ",
		"Духа ",
		"Дух ",

		//6
		"Отцем ",
		"Отце ",
		"Отец ",

		"Отче ",
		"Отца ",
		"Отцу ",

		//7
		"Божием ",
		"Божия ",
		"Божий ",

		"Божие ",
		"Божее",
		"Божии ",
		"Божию ",

		//5
		"Который ",
		"Которому ",
		"Которого ",
		"Вседержитель ",
		"Небесный "


		};

		const int WORDS_COUNT=2+14+11+10+3;
		char * words[WORDS_COUNT]=
		{
			//2
			"Человеческий ",
			"Человеческого ",


			//14
			"Моего ",
			"Твоего ",
			"Твои ",
			"Твой ",
			"Мои ",
			"Наши ",

			"Его ",
			"Мой ",
			"Моя ",
			"Ним ",
			"Нем ",
			"Мое ",
			"Мне ",
			"Мною ",

			//11
			"Своего ",
			"Своему ",
			"Своими ",
			"Своим ",
			"Своих ",
			"Своей",

			"Себя ",
			"Себе ",
			"Свой ",
			"Свои ",
			"Сам ",


			//10
			"Твоим ",
			"Тебя ",
			"Тебе ",
			"Твое ",
			"Твоя ",
			"Него ",

			"Нему ",
			"Меня ",
			"Моими ",
			"Моим ",

			//3
			"Ему ",
			"Он ",
			"Ты "


		};

		const int WORDS_IISUS_COUNT=2+5+6+8+5+2;
		char * words_IISUS[WORDS_IISUS_COUNT]=
		{
			//2
			"Я ",

			//5
			"Иисусом ",
			"Иисусе ",
			"Иисуса ",
			"Иисусу ",
			"Иисус ",

			//6
			"Христову ",
			"Христово ",
			"Христом ",
			"Христос ",
			"Христе ",
			"Христа ",
			"Христу ",

			//8

			"Господи ",
			"Господь ",
			"Господа ",
			"Господе ",

			"Господу ",
			"Господнем ",
			"Господни ",
			"Господом ",

			//5
			"Сыном ",
			"Сыну ",
			"Сына ",
			"Сыне ",
			"Сын ",

			//2
			"Слова ",
			"Слово "
		};

		scr.SetColorRegister(12, 63,0,0);

/*
Я		Ты		Он		Отец	Дух		Бог		Христос	Твое	кто ?
Меня	Тебя	Его		Отца	Духа	Бога	Христа	Твоя	кого ?
Мне		Тебе	Ему		Отцу	Духу	Богу	Христу	Твоему	кому ?
														винить кого ?
И Р Д В Т П
*/
		int ok=0;

		static float ugol=0;
		ugol+=0.003;

		if (ugol>100*3.14) ugol=0;


		need_find=FALSE;

		if (need_find)
		{
			for(int k=0; k<WORDS_GOD_COUNT; k++)
			{
				ok=Fun2(&file_src[t], words_God[k],
				strlen(words_God[k]), &t, &_x, &_step_x, 13, &y);
				if (ok) break;
			}

		if (!ok)
		{
			for(k=0; k<WORDS_COUNT; k++)
			{
				ok=Fun2(&file_src[t], words[k],strlen(words[k]),
				 &t, &_x, &_step_x, 17, &y);

				if (ok) break;
			}
		}

		if (!ok)
		{
			for(k=0; k<WORDS_IISUS_COUNT; k++)
			{
				ok=Fun2(&file_src[t],
				 words_IISUS[k],strlen(words_IISUS[k]),
				 &t,  &_x, &_step_x, 14, &y);

				if (ok) break;
			}
		}

		_x=_step_x*t -(RunLX%(BigSymScale*scr.StepX));

		scr.locateXY(_x, y);
		scr.putBigSym(file_src[t], scr.color,0);

		if (ok)
		{
			char _r=40+23*sin(ugol);
			scr.SetColorRegister(13, _r,0,0);
			scr.SetColorRegister(14, _r,_r,0);
			scr.SetColorRegister(17, 63-_r,_r,0);
		}
		}

		_x=_step_x*t -(RunLX%(BigSymScale*scr.StepX));

		scr.locateXY(_x, y);
		scr.putBigSym(file_src[t], scr.color,0);

	}

	//СДЕЛАТЬ ЗАКЛАДКИ
	//АКТИВИЗИРУЮТСЯ ПО КНОПКАМ ЦИФРОВЫМ
	//ИЛИ ОДНА КНОПКА СЛЕДУЮЩАЯ ЗАКЛАДКА
	scr.y=y-8;
	scr.x=0;
//  scr.color=15;

	scr<<ZAGLAVIE<<" ";//<<position<<"       ";


	unsigned char hour,min,sec;
	GetRealTimeClock(hour, min, sec);

	/*
	int list_count=7;
	int hour_list[7]={13,13,13,14, 15, 16,17},
		 min_list[7]={20,40,50,20, 10, 20,30};

	ifstream file;
	file.open("task.txt");

	char list[7][64]={0};

	for(int ii=0; ii<7; ii++)
	{
		file>>hour_list[ii];
		file>>min_list[ii];
		file>>list[ii];
		for(int vb=0; vb<strlen(list[ii]); vb++)
		{
			if (list[ii][vb]=='.') list[ii][vb]=' ';
		}
	}

	file.close();
	*/

/*	char * list[7]={
	"6-23-52 Гриневич Наталья Борисовна",
	"9-75-57 Попов Юрий",
	"9-45-67 Волков Владимир Николаевич",
	"5-84-21 Коробов Саша",
	"7-32-02 Ковшова Любовь Петровна",
	"7-07-72 Волков Андрей Владимирович",
	"5-37-69 Иванов Виталик"
	 };
*/
/*
	scr.x=0;
	scr.y=0;
	scr<<int(hour)<<":"<<int(min)<<":"<<int(sec);

	char txt[12]={0};
	sprintf(txt,"%02i:%02i:%02i", int(hour), int(min),int(sec));

	for(int v=0;v<strlen(txt);v++)
	scr.putBigSym(txt[v], 12,0);

	if (show_task_list)
	{

	for(int k=0; k<list_count; k++)
	{
		scr.x=0;
		scr.y=50+k*10;
		int delta=hour*60+min-min_list[k]-hour_list[k]*60;
		if (delta<0) delta*=-1;
		if (delta<5)
		{
			static kk=0;
			kk++;
			scr.color=kk;
		}
		else scr.color=15;

		char ss[64];
		sprintf(ss, "%02i:%02i ",int(hour_list[k]),int(min_list[k]));

		//scr<<ss<<list[k];
	}
	}
*/
	RunLX+=StepRunLX;

	return 1;
}
void SoundModule(int);
//выводна экран мену
int TETRIS::menu()
{
	scr.update();

	scr.bkcolor=0;

	int x=14,y=7,N=10;
	float t=0,t1=0;

/*	char far * txt[]={
	 "1 PLAYER",
	 "2 PLAYERS",
	 "REDEFINE KEYS",
	 "HI-SCORE",
	 "EXIT",
	  };*/
	char far * txt[]={
	 "ОДИН ИГРОК",
	 "ДВА ИГРОКА",
	 "ВЫБОР КЛАВИШ",
	 "НАСТРОЙКИ 1",
	 "НАСТРОЙКИ 2",
	 "ТАБЛИЦА РЕКОРДОВ",
	 "ВЫХОД ",
	 "РЕДАКТОР ШРИФТА",
	 "ПОМОЩЬ",
	 "НОВЫЙ ЗАВЕТ",
	  };


	int nm=0;
	int px=x-2;	//координаты указателя

	while (1)
	{
		SoundModule(0);

		SCREEN &Screen=scr;

		Screen.setcolor(15);
		Screen.clear();

		char *t5="T E T Р I С - НОВЫЙ ЗАВЕТ";
		scr.color=62;
		scr.y=4*8;
		scr.x=(320-(strlen(t5)*scr.StepX))/2;
		scr<<t5;

		unsigned char hour, min, sec;
		GetRealTimeClock(hour, min, sec);
		char buf[32];

		sprintf(buf,"%02i:%02i:%02i", hour, min, sec);

		scr.color=2;
		scr<<cur(15,2)<<buf;

		static unsigned char clr=65, clr_2=15;

		int STEP_Y=12;

		//вывод строк меню
		{
		for (int k=0; k<N; k++)
		{
			//если строку надо показать выделенной
			if (k == nm)
			{
				int x1=px*8+20*sin(t1)+12;
				int y1=k*STEP_Y+y*8+5*cos(t1);

				Screen.locateXY(x1+1,y1+1);
				Screen.setcolor(1);
				Screen.putText(txt[k]);

				Screen.locateXY(x1,y1);
				Screen.setcolor(clr);
				//Screen.locateXY(px*8+8,k*2*8+y*8);
			}
			else
			{
				Screen.locateXY(px*8+8,k*STEP_Y+y*8);
				Screen.setcolor(clr_2);
			}
			Screen.putText(txt[k]);

			scr.color=15;
			//graph_draw_num(16, 8, clr);
			//graph_draw_num(16, 16, clr_2);

			/*if (Keyb[KEY_C])
			{
				clr++;
				Keyb.Set(KEY_C,0);
			}

			if (Keyb[KEY_X])
			{
				clr_2++;
				Keyb.Set(KEY_X,0);
			} */
		}
	}

	if ( Keyb[KEY_UP])
	{
		Keyb.Set(KEY_UP,0);
		nm--;

		if (nm<0) nm=N-1;

		//Sound.play(SND_MENUSEL);
	}

	if ( Keyb[KEY_DOWN])
	{
		Keyb.Set(KEY_DOWN,0);
		nm++;
		if (nm>=N) nm=0;
		//Sound.play(SND_MENUSEL);
	}

	//вывод указателя
	{
		//Sprite.Show(px*8-15+10*sin(t),nm*STEP_Y+y*8, SPRITE_POINTER);
        scr.rectangle(px*8-15+10*sin(t), nm*STEP_Y+y*8, 5,5, 19);
		const pi=3.1415926;
		t+=0.25;
		t1+=0.1;
		if (t>=pi*4) t-=2*pi;
	}

	//показываем рамку из звезд
	//Star.Animate();

	//рисуем рамку
	//draw_border();

	if (DrawRunLine(200-BigSymScale*8) == 0)
	{
		Rectable.Show(0);
	}

	//показываем строку автора
	//Sprite.Show(90,170-8, SPRITE_AUTORNAME);
	//Sprite.Show(320-48-24,200-24-24, SPRITE_TEST);

	//ожидаем разрешения таймера отобразить картинку
	{
		//while (!::Timer.Counter){}
		//::Timer.Counter=0;
	}

	//показываем изображение
	scr.wait();
	scr.update();

	if ( (SequenceStatus() != SEQ_PLAYING) )
	{
		music(CURRENT_MUSIC,0);
	}

	if (Keyb[KEY_ENTER]) break;
	}//конец цикла while

	Keyb.Set(KEY_ENTER,2);

	return nm;
}


/*	char *RunLine1="                                                                                  "
	"Молитва Оптинских Старцев. Господи, дай мне с душевным спокойствием "
	"встретить все, что принесет мне наступающий день. во всех словах "
	"и делах моих руководи моими мыслями и чувствами. какие бы я не получал "
	"известия в течении дня научи меня принять их с душевным спокойствием "
	"и с твердым убеждением, что на все Твоя Святая воля. во всех непредвиденных "
	"случаях не дай мне забыть, что все ниспослано Тобою. дай мне силы перенести "
	"утомление наступающего дня и всех событий его. научи меня прямо и разумно "
	"действовать с каждым членом моей семьи, никого не смущая и не огорчая. "
	"научи меня молиться, верить, терпеть, прощать и любить. Аминь. (Молитва Оптинских Старцев) "

	"От Матфея, глава 6.     5 И когда молишься, не будь, как лицемеры, "
	"которые любят в синагогах и на углах улиц останавливаясь молится, "
	"чтобы показаться пред людьми. Истинно говорю вам, что они уже получают "
	"награду свою. 6 Ты же, когда молишься, войди в комнату твою и, "
	"затворив дверь твою, помолись Отцу твоему, Который втайне; и Отец твой, "
	"видящий тайное, воздаст тебе явно. 7 А молясь не говорите лишнего, как "
	"язычники, ибо они думают, что в многословии своем будут услышаны; "
	"8 Не уподобляйтесь им; ибо знает Отец ваш, в чем вы имеете нужду, "
	"прежде вашего прошения у Него. 9 Молитесь же так: Отче наш, сущий на "
	"небесах! да святится имя Твое; 10 Да приидет Царствие Твое; да будет "
	"воля Твоя и на земле, как на небе; 11 Хлеб наш насущный дай нам на "
	"сей день; 12 И прости нам долги наши, как и мы прощаем должникам нашим; "
	"13 И не введи нас в искушение, но избавь от лукавого; ибо Твое есть Царство "
	"и сила и слава во веки. Аминь. 14 Ибо, если вы будете прощать людям "
	"согрешения их, то простит и вам Отец ваш Небесный; 15 А если не будете "
	"прощать людям согрешения их, то и Отец ваш не простит вам согрешений ваших. "
	"16 Также, когда поститесь, не будьте унылы, как лицемеры; ибо они "
	"принимают на себя мрачные лица, чтобы показаться людям постящимся. "
	"Истинно говорю вам, что они уже получают награду свою. "
	"17 А ты, когда постишься, помажь голову твою и умой лице твое, "
	"18 Чтобы явится постящимся не пред людьми, но пред Отцем твоим, "
	"Который втайне; и Отец твой, видящий тайное, воздаст тебе явно. "
*/

/*	char *RunLine2="                                                                                  "
	"ЭТУ ИГРУ Я ПОСВЕЩАЮ СВОЕМУ ОТЦУ - КОТОРЫЙ ОЧЕНЬ МНОГО СДЕЛАЛ "
	"ДЛЯ МЕНЯ. НЕ СМОТРЯ НА ТО, ЧТО ОТНОШЕНИЯ МОИ С МОИМ ПАПОЙ НЕ ВСЕГДА "
	"БЫВАЛИ ИДЕАЛЬНЫМИ, МЫ ВСЕГДА МИРИЛИСЬ И ВСЕ ПРОЩАЛИ ДРУГ ДРУГУ! "
	"НУ А КАК ЖЕ ЕЩЕ МОЖЕТЬ БЫТЬ ИНАЧЕ ? СЕЙЧАС МЫ В ОЧЕНЬ ХОРОШИХ "
	"ОТНОШЕНИЯХ, ПРАКТИЧЕСКИ НЕ ССОРИМСЯ, А ЕСЛИ ВДРУГ И ПОВЗДОРИМ - "
	"ТО ОЧЕНЬ БЫСТРО МИРИМСЯ.          НЕ КАК РАНЬШЕ, КОГДА МЫ МОГЛИ МНОГИМИ "
	"ДНЯМИ ХРАНИТЬ ОБИДУ И ЗЛОБУ ДРУГ НА ДРУГА. ТЕПЕРЬ ВСЕ ИНАЧЕ. "
	"ОТЕЦ ВНОВЬ МЕНЯ СЧИТАЕТ СВОИМ СЫНОМ, А Я СЧИТАЮ ЕГО СВОИМ ОТЦОМ. "
	"ВСЕ ТЕПЕРЬ БУДЕТ У НАС ХОРОШО!                                ";
*/

	char *RunLine3="                                                                                  "
	" ИГРА 'ТЕТРИС' - ОЧЕНЬ УВЛЕКАТЕЛЬНАЯ И ЖИВАЯ ИГРА.  ЧТОБЫ ПОБИТЬ "
	"РЕКОРД, НАДО НЕ ТОЛЬКО ДУМАТЬ, НО И ОБЛАДАТЬ ДОСТАТОЧНОЙ "
	"ЛОВКОСТЬЮ РУК.       СУТЬ ИГРЫ ПРОСТА.    В СТАКАН ПАДАЮТ РАЗНЫЕ "
	"ФИГУРКИ, КОТОРЫЕ МОЖНО ПОВОРАЧИВАТЬ НА 90 ГРАДУСОВ. НУЖНО "
	"ТАК ИХ УЛОЖИТЬ В СТАКАН, ЧТОБЫ НА ОДНОЙ ЛИНИИ НЕ ОСТАЛОСЬ НИ "
	"ОДНОГО ПРОПУСКА - ТОГДА ЭТА ЛИНИЯ ПРОПАДАЕТ, А СОДЕРЖИМОЕ СТАКАНА "
	"УПЛОТНЯЕТСЯ. ЗА ОДНУ ЛИНИЮ ДАЮТ 100 ОЧКОВ, ЗА ДВЕ - 300, "
	"ЗА ТРИ - 700, ЗА ЧЕТЫРЕ - 1500. С УВЕЛИЧЕНИЕМ ЧИСЛА НАБРАННЫХ "
	"ОЧКОВ УВЕЛИЧИВАЕТСЯ СКОРОСТЬ ИГРЫ, И НА ПОСЛЕДНЕЙ, ДЕВЯТОЙ СКОРОСТИ "
	"ДОЛГО ПРОДЕРЖАТЬСЯ МОЖЕТ ТОЛЬКО НАСТОЯШИЙ МАСТЕР. ЖЕЛАЮ УДАЧИ !"
	"            (c) 1997-2010 ЕВГЕНИЙ ЛАПШИН БОЛЬШЕ ИЗВЕСТНЫЙ КАК WIZ  "
	"                                                                        "
	;

		/*

		"Ваша цель - выбить все кирпичи. Для этого надо отбивать шарик битой"
		", стараясь его не потерять. Из некоторых кирпичей могут выпадать призы - "
		" ловите их. Желаю удачи !";

		"                                               "


*/

/*

Представьте, что время остановилось. Все часы на планете Земля вышли из
строя и так и остались в состоянии на одной отметке.

часы невозможно подвести или отвести,
часы невозможно сломать,
часы невозможно ни остановить, ни запустить

часы остановились.

*/
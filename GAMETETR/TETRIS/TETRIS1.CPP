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

void graph_draw_num(int x, int y, int num);

unsigned long RunLX=0;

void music(int n, int seq);
extern int CURRENT_MUSIC;
extern SPRITE SpriteBox;
extern SPRITE SpriteFigure;
extern SPRITE Sprite;
extern SPRITE Background;
extern float BaseSpeed;
extern float BaseAccelerat;
extern RECTABLE Rectable;

void GetRealTimeClock(unsigned char &hour, unsigned char &min, unsigned char &sec);

//§ £ąć§Ŗ  įÆą ©ā®¢
void load_sprites()
{
	SPRITE::IMAGE im;
	im.Attribute = 0;

	//§ £ąć¦ „¬ § ¤­Ø© Æ« ­
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

	//§ £ąć¦ „¬ Ŗ¢ ¤ą āØŖØ
	for(int i=1; i<8; i++)
	{
		im.Attribute = 0;
		im.Index = i;
		SpriteBox.LoadSpriteform( "graph\\brick.sp1", im);
		SpriteBox.ModifyColor(i, (i-1)*16);
	}

	//§ £ąć¦ „¬ ćŖ § ā„«ģ
	{
		im.Attribute = SCREEN::TRANSP;
		im.Index = SPRITE_POINTER;
		Sprite.LoadSpriteform( "graph\\pointer.sp0", im);
	}

	im.Index=SPRITE_BORDER;
	SpriteBox.LoadSpriteform("graph\\border.sp0",im);

	im.Index=SPRITE_AUTORNAME;
	Sprite.LoadSpriteform("graph\\autor.sp0",im);

	//§ £ąć¦ „¬ äØ£ćąŖØ
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

extern int BigSymScale;
int StepRunLX=1;

extern SETTINGS Settings;

unsigned char str_find[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int DrawRunLine(int y)
{

//	int STEP=6;

	// 320/8=40
	// 320/7=
	// 320/6=
	// 50*6=300
	// 51*6=306
	// 52*6=312
	// 53*6=318

	const int FILE_BUF_SIZE=55;

	char file_buf[FILE_BUF_SIZE];

	int len=FILE_BUF_SIZE;

	ifstream ifl;
	ifl.open("NOVIZAVT\\all_newz.txt", ifl.binary);
	if (!ifl) return 0;

	ios::seek_dir sd = ios::end;
	ifl.seekg(0, sd);
	unsigned long filesize=ifl.tellg();

	#define SEEK_NONE 0
	#define SEEK_NEXT 1
	#define SEEK_PREV 2
	#define SEEK_APPLY 3
	#define SEEK_EXIT 4

	int seek=SEEK_NONE;

	if (Keyb[KEY_PAD6])
	{
		Keyb.Set(KEY_PAD6,0);
		seek=SEEK_NEXT;
	}

	if (Keyb[KEY_PAD4])
	{
		Keyb.Set(KEY_PAD4,0);
		seek=SEEK_PREV;
	}

	if (Keyb[KEY_PAGEUP])
	{
		if (BigSymScale<10)
		{
			BigSymScale++;
			Settings.BigSymScale=BigSymScale;
			Settings.Save();
		}
		Keyb.Set(KEY_PAGEUP,0);
		RunLX=0;
	}

	if (Keyb[KEY_PAD1])
	{
		Keyb.Set(KEY_PAD1,0);
		RunLX=0;
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
		RunLX=0;
	}

	if (Keyb[KEY_PAD0]) RunLX=1;


	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_F])
	{
		Msg(" ©āØ:                     ");
		scr.InputString(20, str_find, 0);
		Keyb.Set(KEY_L,1);
		Keyb.Set(KEY_LEFTCTRL,1);
	}

	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_L])
	{
		Keyb.Set(KEY_L,0);
		int find_len=strlen(str_find);
	  if (find_len>0)
	  {
		char stext[128];
		strcpy(stext, " ®ØįŖ: <");
		strcat(stext, str_find);
		strcat(stext, "> ");
		Msg(stext);
		scr.update();

		//ifl.seekg(0);
		unsigned long i=RunLX/scr.StepX/BigSymScale;
		ifl.seekg(i);

		int k=0;
		int find=0;
		for(; i<filesize; i++)
		{
			unsigned char sym;
			ifl.read(&sym,1);

			if (sym==str_find[k])
			{
				find++;
				k++;
				if (find==find_len)
				{
					RunLX=(i-find_len-10)*BigSymScale*scr.StepX;
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
			Msg(" -------   ------- ",3);
			scr.update();
			Keyb.ReadScan();
		}
	  }
	}


	if (BigSymScale<1) BigSymScale=1;
	if (BigSymScale>10) BigSymScale=10;

	char ZAGLAVIE[40]={0};

	int nofile=1;

	unsigned long position=0;

	if (RunLX==0)
	{
		randomize();

		unsigned long kb=filesize/1024L;

		RunLX=1024L*random(kb)*BigSymScale*scr.StepX;
	}

	position=RunLX/scr.StepX/BigSymScale;
	ifl.seekg(position);

	//Æ®ØįŖ § £« ¢Øļ
	for(unsigned long k=position; k<filesize; k++)
	{
		char S;
		ifl.read(&S,1);

		if (S!='(') continue;

		char V;
		ZAGLAVIE[0]='(';
		for(int b=1; b<40; b++)
		{
			ifl.read(&V, 1);
			ZAGLAVIE[b]=V;
			ZAGLAVIE[39]=0;

			if (V==')')
			{
				ZAGLAVIE[b+1]=0;
				if (seek==SEEK_NEXT)
				{
					unsigned long pos=k+strlen(ZAGLAVIE)+1;
					RunLX=pos*BigSymScale*scr.StepX;

					seek=SEEK_NONE;
				}

				break;
			}
		}

		if (V==')') break;

	}
	position=RunLX/scr.StepX/BigSymScale;
	ifl.seekg(position);
	ifl.read(file_buf,FILE_BUF_SIZE);
	ifl.close();

	for(int i=0; i<FILE_BUF_SIZE; i++)
	{
		char a=file_buf[i];

		if (a==13 || a==10 || a==9) a=32;

		file_buf[i]=a;
	}

	nofile=0;

	if (nofile) return 0;

	static int basecolor=RunLX;

//	scr.line(0,y-1,320,y-1,9);

	for(int x=0,t=0; x<320+BigSymScale*scr.StepX; x+=BigSymScale*scr.StepX,t++)
	{
		scr.locateXY(x-(RunLX%(BigSymScale*scr.StepX)), y);

		unsigned char sym=file_buf[t];

		if (t<len)
		{
			scr.putBigSym(sym, 15,0);
		}
	}

	scr.y=y-8;
	scr.x=0;

	ZAGLAVIE[39]=0;
	scr<<ZAGLAVIE;

	RunLX+=StepRunLX;

	return 1;
}

//¢ė¢®¤ ­  ķŖą ­ ¬„­ć
int TETRIS::menu()
{
	scr.update();

	scr.bkcolor=0;

	::Timer.Restart(50);

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
	 " ",
	 " ",
	 " ",
	 " 1",
	 " 2",
	 " ",
	 " ",
	 " ",
	 "",
	 " ",
	  };


	int nm=0;
	int px=x-2;	//Ŗ®®ą¤Ø­ āė ćŖ § ā„«ļ

	while (1)
	{
		SCREEN &Screen=scr;

		Screen.setcolor(15);
		Screen.clear();

		char *t5="T E T  I  -  ";
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

		//¢ė¢®¤ įāą®Ŗ ¬„­ī
		{
		for (int k=0; k<N; k++)
		{
			//„į«Ø įāą®Ŗć ­ ¤® Æ®Ŗ § āģ ¢ė¤„«„­­®©
			if (k == nm)
			{
				Screen.locateXY(px*8+20*sin(t1)+12,k*STEP_Y+y*8);

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

			if (Keyb[KEY_C])
			{
				clr++;
				Keyb.Set(KEY_C,0);
			}

			if (Keyb[KEY_X])
			{
				clr_2++;
				Keyb.Set(KEY_X,0);
			}
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

	//¢ė¢®¤ ćŖ § ā„«ļ
	{
		//Sprite.Show(px*8-15+10*sin(t),nm*STEP_Y+y*8, SPRITE_POINTER);
        scr.rectangle(px*8-15+10*sin(t), nm*STEP_Y+y*8, 5,5, 19);
		const pi=3.1415926;
		t+=0.25;
		t1+=0.20;
		if (t>=pi*4) t-=2*pi;
	}

	//Æ®Ŗ §ė¢ „¬ ą ¬Ŗć Ø§ §¢„§¤
	//Star.Animate();

	//ąØįć„¬ ą ¬Ŗć
	//draw_border();

	if (DrawRunLine(200-BigSymScale*8) == 0)
	{
		Rectable.Show(0);
	}

	//Æ®Ŗ §ė¢ „¬ įāą®Ŗć  ¢ā®ą 
	//Sprite.Show(90,170-8, SPRITE_AUTORNAME);
	//Sprite.Show(320-48-24,200-24-24, SPRITE_TEST);

	//®¦Ø¤ „¬ ą §ą„č„­Øļ ā ©¬„ą  ®ā®”ą §Øāģ Ŗ ąāØ­Ŗć
	{
		while (!::Timer.Counter){}
		::Timer.Counter=0;
	}

	//Æ®Ŗ §ė¢ „¬ Ø§®”ą ¦„­Ø„
	scr.update();

	if ( (SequenceStatus() != SEQ_PLAYING) )
	{
		music(CURRENT_MUSIC,0);
	}

	if (Keyb[KEY_ENTER]) break;
	}//Ŗ®­„ę ęØŖ«  while

	Keyb.Set(KEY_ENTER,2);

	return nm;
}


/*	char *RunLine="                                                                                  "

	"ā  āä„ļ, £« ¢  6.     5  Ŗ®£¤  ¬®«Øčģįļ, ­„ ”ć¤ģ, Ŗ Ŗ «Øę„¬„ąė, "
	"Ŗ®ā®ąė„ «ī”ļā ¢ įØ­ £®£ å Ø ­  ć£« å ć«Øę ®įā ­ ¢«Ø¢ ļįģ ¬®«Øāįļ, "
	"ēā®”ė Æ®Ŗ § āģįļ Æą„¤ «ī¤ģ¬Ø. įāØ­­® £®¢®ąī ¢ ¬, ēā® ®­Ø ć¦„ Æ®«ćē īā "
	"­ £ą ¤ć į¢®ī. 6 ė ¦„, Ŗ®£¤  ¬®«Øčģįļ, ¢®©¤Ø ¢ Ŗ®¬­ āć ā¢®ī Ø, "
	"§ ā¢®ąØ¢ ¤¢„ąģ ā¢®ī, Æ®¬®«Øįģ āęć ā¢®„¬ć, ®ā®ąė© ¢ā ©­„; Ø ā„ę ā¢®©, "
	"¢Ø¤ļéØ© ā ©­®„, ¢®§¤ įā ā„”„ ļ¢­®. 7  ¬®«ļįģ ­„ £®¢®ąØā„ «Øč­„£®, Ŗ Ŗ "
	"ļ§ėē­ØŖØ, Ø”® ®­Ø ¤ć¬ īā, ēā® ¢ ¬­®£®į«®¢ØØ į¢®„¬ ”ć¤ćā ćį«ėč ­ė; "
	"8 „ ćÆ®¤®”«ļ©ā„įģ Ø¬; Ø”® §­ „ā ā„ę ¢ č, ¢ ē„¬ ¢ė Ø¬„„ā„ ­ć¦¤ć, "
	"Æą„¦¤„ ¢ č„£® Æą®č„­Øļ ć „£®. 9 ®«Øā„įģ ¦„ ā Ŗ: āē„ ­ č, įćéØ© ­  "
	"­„”„į å! ¤  į¢ļāØāįļ Ø¬ļ ¢®„; 10   ÆąØØ¤„ā  ąįā¢Ø„ ¢®„; ¤  ”ć¤„ā "
	"¢®«ļ ¢®ļ Ø ­  §„¬«„, Ŗ Ŗ ­  ­„”„; 11 «„” ­ č ­ įćé­ė© ¤ © ­ ¬ ­  "
	"į„© ¤„­ģ; 12  Æą®įāØ ­ ¬ ¤®«£Ø ­ čØ, Ŗ Ŗ Ø ¬ė Æą®é „¬ ¤®«¦­ØŖ ¬ ­ čØ¬; "
	"13  ­„ ¢¢„¤Ø ­ į ¢ ØįŖćč„­Ø„, ­® Ø§” ¢ģ ®ā «ćŖ ¢®£®; Ø”® ¢®„ „įāģ  ąįā¢® "
	"Ø įØ«  Ø į« ¢  ¢® ¢„ŖØ. ¬Ø­ģ. 14 ”®, „į«Ø ¢ė ”ć¤„ā„ Æą®é āģ «ī¤ļ¬ "
	"į®£ą„č„­Øļ Øå, ā® Æą®įāØā Ø ¢ ¬ ā„ę ¢ č „”„į­ė©; 15  „į«Ø ­„ ”ć¤„ā„ "
	"Æą®é āģ «ī¤ļ¬ į®£ą„č„­Øļ Øå, ā® Ø ā„ę ¢ č ­„ Æą®įāØā ¢ ¬ į®£ą„č„­Ø© ¢ čØå. "
	"16  Ŗ¦„, Ŗ®£¤  Æ®įāØā„įģ, ­„ ”ć¤ģā„ ć­ė«ė, Ŗ Ŗ «Øę„¬„ąė; Ø”® ®­Ø "
	"ÆąØ­Ø¬ īā ­  į„”ļ ¬ą ē­ė„ «Øę , ēā®”ė Æ®Ŗ § āģįļ «ī¤ļ¬ Æ®įāļéØ¬įļ. "
	"įāØ­­® £®¢®ąī ¢ ¬, ēā® ®­Ø ć¦„ Æ®«ćē īā ­ £ą ¤ć į¢®ī. "
	"17  āė, Ŗ®£¤  Æ®įāØčģįļ, Æ®¬ ¦ģ £®«®¢ć ā¢®ī Ø ć¬®© «Øę„ ā¢®„, "
	"18 ā®”ė ļ¢Øāįļ Æ®įāļéØ¬įļ ­„ Æą„¤ «ī¤ģ¬Ø, ­® Æą„¤ āę„¬ ā¢®Ø¬, "
	"®ā®ąė© ¢ā ©­„; Ø ā„ę ā¢®©, ¢Ø¤ļéØ© ā ©­®„, ¢®§¤ įā ā„”„ ļ¢­®. "
	"                                                              "


/*	"      -     "
	" .    ,         "
	" ,        ! "
	"        ?      "
	",   ,      - "
	"   .            ,     "
	"       .   . "
	"     ,      . "
	"     !                                          "
*/

/*	"  '' -     .    "
	",    ,     "
	" .         .        "
	",     90 .  "
	"    ,        "
	"  -    ,    "
	".     100 ,   - 300, "
	"  - 700,   - 1500.     "
	"   ,   ,   "
	"     .   !"
	"            (c) 1997-2010      WIZ  "
	"                                                                        "
	;

		/*

		" č  ę„«ģ - ¢ė”Øāģ ¢į„ ŖØąÆØēØ. «ļ ķā®£® ­ ¤® ®ā”Ø¢ āģ č ąØŖ ”Øā®©"
		", įā ą ļįģ „£® ­„ Æ®ā„ąļāģ. § ­„Ŗ®ā®ąėå ŖØąÆØē„© ¬®£ćā ¢ėÆ ¤ āģ ÆąØ§ė - "
		" «®¢Øā„ Øå. „« ī ć¤ ēØ !";

		"                                               "


*/
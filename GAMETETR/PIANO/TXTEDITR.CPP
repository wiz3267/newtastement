#include "screen.h"
#include "keyboard.h"
#include <conio.h>
#include <string.h>
#include <fstream.h>

#define TRUE 1
#define FALSE 0

extern int BigSymScale;

unsigned char big[]="‰–“Š…ƒ˜™‡•š”›‚€‹„†Ÿ—‘Œˆ’œ";
char big_len=32;

extern ifstream ifl;

char code13=0;

const int word_max=64;
unsigned char word[word_max];
char word_len=0;
//char word_index=0;
void SoundModule(int param);
int read_word()
{
	word_len=0;

	unsigned char z;
	do{

	ifl.read(&z,1);

	if(z==32)return word_len;
	if(z==13){code13=1;return word_len;}
	if(z<32) continue;

	word[word_len]=z;
	word[word_len+1]=0;
	word_len++;

	if(word_len>=word_max) return word_len;

	}while(1);

}

unsigned char get_sym()
{
	unsigned char z;
	ifl.read(&z,1);
	return z;
}

int cursor_len;
int cursor_hei;

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

void TextEditor()
{
	scr.clear();
	scr.x=0;
	scr.y=0;
//	Keyb.StandartInput();
	Keyb.Set(KEY_L,0);
	static int pause=1;
	Keyb.Set(KEY_ENTER,0);
	do {

	SoundModule(0);

	char color=1;

	if (Keyb[KEY_PAGEUP]){Keyb.Set(KEY_PAGEUP,0);BigSymScale++;pause=0;}
	if (Keyb[KEY_PAGEDOWN]){Keyb.Set(KEY_PAGEDOWN,0);BigSymScale--;pause=0;}

	if(BigSymScale<1)BigSymScale=1;


	cursor_len=BigSymScale*6;
	cursor_hei=BigSymScale*8;

	if (Keyb[KEY_ESC])
	{
		Keyb.Set(KEY_ESC,0);
		return;
	}

	static int timer=0;
	timer++;

	int cr=1;
	if ((timer%50)>25) cr=0;

	scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,cr);
	scr.update();

	if(Keyb[KEY_ENTER])
	{
		if(pause<0)pause=0;
		else pause=-1;
		Keyb.Set(KEY_ENTER,0);
	}

	static delay=5;

	if (Keyb[KEY_LEFT]) {delay--;Keyb.Set(KEY_LEFT,0);pause=0;}
	if (Keyb[KEY_RIGHT]) {delay++;Keyb.Set(KEY_RIGHT,0);pause=0;}

	if (delay<=0) delay=1;


	if (pause)
	{
		pause--;
		if(pause==0)NewLine();

		SoundModule(0);
		scr.wait();
		scr.update();
		continue;
	}
	timer=0;
	unsigned char code;//get_sym();
	read_word();
	code=word[0];

	if((scr.x+BigSymScale*6*word_len)>320)
	{
		NewLine();
	}

	if (code==8)
	{
		scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);

		int len=BigSymScale*6;
		scr.x-=len;
		scr.rectangle(scr.x,scr.y,len,len,0);
		scr.update();
	}

	{
		color=2;

		for(int n=0; n<big_len; n++)
		{

			if (big[n]==code) color=12;
		}
	}

	if (code>='0' && code<='9') color=5;

	scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);

	if(Keyb[KEY_PAD0])
	{
		scr.x=0;
		scr.y=0;
		scr.clear();
		ifl.seekg(0);
	}

	for(int t=0;t<word_len;t++)
	{
		scr.putBigSym(word[t],color,3);
		scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,cr);
		scr.update();
		scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);

		for(int i=0;i<delay;i++)
		{
			if(Keyb[KEY_ENTER])break;
			SoundModule(0);
			scr.wait();
		}

		scr.update();
	}
	scr.putBigSym(' ',color,3);

	if (code13)
	{
		NewLine();
		pause=100;
	}

	code13=0;
	SoundModule(0);
	scr.update();

}while(1);
}

//á®£« è¥­¨ï
// InputString - ¨¬¥­  äã­ªæ¨©
// class STRING - ­ §¢ ­¨¥ ª« áá 

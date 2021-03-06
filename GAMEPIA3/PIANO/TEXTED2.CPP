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

//fun /////////////////////////////////////////////////////////////
void	SoundModule(int param);
void	ColorChange();
int		CheckWord(char *w);
void	ToLowerRegister(unsigned char * sym);
void	RestorePalette();
void	SavePalette();
int		open_text_book();
void	NewLine();
void	VideoEffect();
void	step_back();
int 	read_word();

//extern glob ////////////////////////////////////////////
extern unsigned char word[];
extern int			cr;
extern int		 	cursor_len;
extern int			cursor_hei;
extern int			word_count;
extern SETTINGS 	Settings;
extern int 			BigSymScale;
extern long			filesize;
extern ifstream 	ifl;
extern long			pause;
extern int			SymbolCount;

//��४���� �� ⥡�, �⠭�!
//��ᯮ�� ��� �����⨢ �� ��� ��譮��!
//extern ////////////////////////////////
extern int			SHOW_DEBUG_TIME;
extern int			show_debug;//SHOW_DEBUG_TIME;
//extern unsigned char big[]="��������������������������������";
extern char 	big_len;
extern char		Mode_Printer;
extern char		Mode_RandomChangeChapter;
extern char		code13;
//const int 	word_max=64;
//extern unsigned char word[word_max];

extern char 		word_len;
extern unsigned long start_chapter_position;
//const int	MAX_C=32;
extern char 		ZAGLAV[33+1];

void fun_show_debug()
{
		show_debug--;
		int x=scr.x;
		int y=scr.y;
		scr.x=0;
		scr.y=0;

		scr.color=15;
		scr.bkcolor=16;
		scr<<"Pause_Enter="<<Settings.Pause_Enter;
		scr<<" Pause_Symbol="<<Settings.Pause_Symbol;
		scr<<"  ";
		scr.update();

		scr.x=x;
		scr.y=y;
}


int fun_delay()
{
	int f=0;
			if(Keyb[KEY_ENTER]) f=1;
			if(Keyb[KEY_PAD0]) f=1;
			if(Keyb[KEY_PAD1]) f=1;
			if(Keyb[KEY_PAD2]) f=1;
			if(Keyb[KEY_PAD3]) f=1;
			if(Keyb[KEY_PAD4]) f=1;
			if(Keyb[KEY_PAD5]) f=1;
			if(Keyb[KEY_PAD6]) f=1;
			if(Keyb[KEY_PAD7]) f=1;
			if(Keyb[KEY_PAD8]) f=1;
			if(Keyb[KEY_PAD9]) f=1;
			if(Keyb[KEY_LEFTCTRL]) f=1;
			if(Keyb[KEY_LEFT]||Keyb[KEY_RIGHT]) f=1;

			SoundModule(0);
			ColorChange();

			scr.wait();

			return f;
}
void fun_show_cursor()
{
		{
			scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,cr);
			scr.update();
			scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);
		}

}


void fun_code13()
{
		NewLine();
		if (!Mode_Printer) pause=SymbolCount*(Settings.Pause_Enter-1);
		else pause=0;

		SymbolCount=0;

}

void fun_pad0()
{

		Keyb.Set(KEY_PAD0,0);
		scr.x=0;
		scr.y=0;
		scr.clear();
		scr.update();
		ifl.seekg(0);
		code13=0;
		strcpy(ZAGLAV,"");
}

void fun_code8()
{
		if (scr.x>0)
		{
			scr.rectangle(scr.x,scr.y,cursor_len, cursor_hei,0);

			int len=BigSymScale*6;
			scr.x-=len;
			scr.rectangle(scr.x,scr.y,len,len,0);
			scr.update();
	}
}
void fun_pad9()
{
		Keyb.Set(KEY_PAD9,0);
		for(int i=0; i<10; i++)
		{
			step_back();
			pause=0;
			word_count=0;
		}
}


void fun_pad6()
{
		Keyb.Set(KEY_PAD6,0);
		long position=0;
		char sym=0;
		while(1)
		{
			if(Keyb[KEY_ESC]) break;

			position=ifl.tellg();

			int len=read_word();
			if(len<3) continue;
			char e=word[word_len-1];
			if(e>='0'&& e<='9') break;
		}

		while(1)
		{
		ifl.seekg(position);
		ifl.read(&sym,1);
		if (sym==13) break;
		position--;
		}

		ifl.read(&sym,1);
		word_len=0;

		scr.x=0; scr.y=0;

		scr.clear();

		code13=0;
		pause=0;
		word_count=0;

}



void fun_key_m_key_0()
{
		scr.color=15;
		Msg(" ��⠭����� ०�� 0 (��᫥����⥫�� �����) ");
		Mode_RandomChangeChapter=0;
		scr.update();
		Keyb.ReadScan();
		scr.clear();
		scr.x=0;
		scr.y=0;

}

void fun_key_m_key_1()
{
		scr.color=15;
		Msg(" ��⠭����� ०�� 1 (��砩�� �����) ");
		Mode_RandomChangeChapter=1;
		scr.update();
		Keyb.ReadScan();
		scr.clear();
		scr.x=0;
		scr.y=0;
}

void fun_key_m_key_3()
{
		scr.color=15;
		Msg(" ��⠭����� ०�� 3 (��᫥ * �� ��砫�) ");
		Mode_RandomChangeChapter=3;
		scr.update();
		Keyb.ReadScan();
		scr.clear();
		scr.x=0;
		scr.y=0;
}

void fun_key_m_key_2()
{
		scr.color=15;
		Msg(" ��⠭����� ०�� 2 (��᫥ ����� �� ��砫�) ");
		Mode_RandomChangeChapter=2;
		scr.update();
		Keyb.ReadScan();
		scr.clear();
		scr.x=0;
		scr.y=0;
}
void fun_key_m_key_4()
{
		scr.color=15;
		Msg(" ��⠭����� ०�� 4 (����� �����) ");
		Mode_RandomChangeChapter=4;
		scr.update();
		Keyb.ReadScan();
		scr.clear();
		scr.x=0;
		scr.y=0;
}


void fun_key_pad9()
{
		Keyb.Set(KEY_PAD9,0);
		for(int i=0; i<10; i++)
		{
			step_back();
			pause=0;
			word_count=0;
		}
}

void fun_key_f1()
{
		scr.x=0;
		scr.y=0;
		scr.clear();
		scr.color=15;
		scr<<
		"<F1> ������\n"
        "<LSHIFT>+<F> ��ઠ�쭮\n"
		"<F> ����\n"
		"<LEFT>, <RIGHT> �������� ᪮���� �㪢\n"
		"<LEFTCTRL> + <LEFT>,<RIGHT> �������� ���� ��᫥ �����\n"
		"<ESC> ��室\n"
		"<ENTER> ��㧠\n"
		"<PAD0> ��砫� 䠩��\n"
		"<PAD1> ��砩��� ����\n"
		"<PAD4> �।��騩 ��� \n"
		"<CTRL+PAD4> ��६�⪠ ����� \n"
		"<PAD6> ᫥����� �����\n"
		"<PAD7> ��३� � ०�� ����\n"
		"<PAD8> ��� �� ०��� ����\n"
		"<PAD9> �� ��砫� �����\n"
		"<LEFTCTRL+PAD1> ��砩��� ���� �� ������ �����\n"
		"<M>+<0> ०�� 0 (⥪�� ��᫥����⥫��)\n"
		"<M>+<1> ०�� 1 (��᫥ ����� ����� ���᪮� �� ��砩��� �����)\n"
		"<M>+<2> ०�� 2 (��᫥ ����� �� ��砫�)\n"
		"<M>+<3> ०�� 3 (��᫥ * �� ��砫�)\n"
		"<M>+<4> ०�� 4 (����� �����)\n"


		;
		scr.update();
		Keyb.ReadScan();

		scr.clear();
}





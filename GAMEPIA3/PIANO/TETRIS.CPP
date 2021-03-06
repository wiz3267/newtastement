//��� "TET�IC". ﭢ��� 1998 - ���� 2011 �.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fstream.h>

#include "figure.h"
#include "glass.h"
#include "tetris.h"

#include "mouse.h"
#include "screen.h"
#include "sound.h"
#include "keyboard.h"
#include "timer.h"
#include "sprite.h"
#include "rectable.h"
#include "settings.h"
#include "fontedit.h"

//=================䠩�� ��㪮��� �ࠩ��஢=================
#include "midpak2.h"
#include "doscall2.h"
#include "loader2.h"

extern int show_task_list;
extern unsigned long RunLX;

SETTINGS Settings;

void Draw_Clock();
void graph_draw_symbol(int x, int y, char sym);
void graph_draw_num(int x, int y, unsigned long num);

unsigned long MaxScore;		//���ᨬ��쭮� �᫮ �窮�, ���࠭��� � ���
void GetRealTimeClock(unsigned char &hour, unsigned char &min, unsigned char &sec);

void DrawRunLine(int y);
float BaseSpeed=1, BaseAccelerat=1;

int CURRENT_MUSIC=0;

void PutRec(int scan,unsigned long timer,int pr);
void StartRecord();
RECTABLE Rectable;
int SoundVolume=100;
int KeyDownPress=0;

TETRIS Tetris[2];

extern SPRITE SpriteBox;

void music(int n, int seq);

// *********** �������� ��६���� ***********
	//ᮧ���� �।��।������ ��ꥪ�
	SCREEN  scr;
	MOUSE	Mouse(&scr);
	SOUND	Sound;
	SPRITE	Sprite(&scr,5);
	SPRITE	Background(&scr);		//ᮤ�ন� ������ 䮭�
	MOVEFIGURE Movefig(&SpriteBox);	//��६���騥�� 䨣���
// *********************************************

//trunc - �����頥� �஡��� ����
template <class T>
T trunc(T arg)
{
	return arg-int(arg);
}

void load_sounds()
{
	Sound.load("sound\\pop.raw");
	Sound.load("sound\\xplode2.raw");
}

//============== redfine_keys - �����祭�� ������ ==============
void redefine_keys()
{
	int i;
	int x=14,s=2,k=9,t=6,repeat=1;

	SCREEN &Screen=scr;

	while (repeat)
	{
		Screen.clear();

		for (i=0; i<2; i++)
		{
			Screen.setcolor(9);
			Screen.locate(x,2+i*k);

			Screen.putText("����� ");
			Screen.putNumb(i+1,1);

			Screen.setcolor(15);
			Screen.locate(x-t,s+i*k+2);
			Screen.putText("�����   ");
			Screen.update();
			Screen.putText( Keyb.KEY[Tetris[i].Left=Keyb.ReadScan()] );
//			Sound.play(SND_EXPLODE);

			Screen.locate(x-t, s+i*k+3);
			Screen.putText("������  ");
			Screen.update();
			Screen.putText( Keyb.KEY[Tetris[i].Right= Keyb.ReadScan() ] );
//			Sound.play(SND_EXPLODE);

			Screen.locate(x-t,s+i*k+4);
			Screen.putText("�����   ");
			Screen.update();
			Screen.putText( Keyb.KEY[Tetris[i].Up = Keyb.ReadScan() ] );

			Screen.locate(x-t,s+i*k+5);
			Screen.putText("����    ");
			Screen.update();
			Screen.putText( Keyb.KEY[Tetris[i].Down= Keyb.ReadScan() ] );

			Screen.locate(x-t,s+i*k+6);
			Screen.putText("�����   ");
			Screen.update();
			Screen.putText( Keyb.KEY[Tetris[i].Pause= Keyb.ReadScan() ] );

		}

		Screen.setcolor(143+15*2);

		if (MsgText("��୮ ?", "��",  "���",0, 19) == 1)
		{
			continue;
		}
		else
		{
			return;
		}
	}

//	Sound.play(SND_EXORCISM, NOSTOP);
}

//�������� TETRISa
TETRIS::~TETRIS()
{
}

void generate_picture();
//ᠬ� ���
extern SPRITE SpriteBox;

void TetrisHelp();

void SoundModule(int param);
void PlayNote(int note, int volume, int len);
void PutRecord(int scan,unsigned long timer);
void TETRIS::game()
{

	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_1])
	{
		int len=Glass.Len;
		int hei=Glass.Hei;
		if (len>5) len--;
		Glass.Make(len,hei);

		Keyb.Set(KEY_1,0);
	}
	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_2])
	{
		int len=Glass.Len;
		int hei=Glass.Hei;
		if (len<40) len++;
		Glass.Make(len,hei);

		Keyb.Set(KEY_2,0);
	}

	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_3])
	{
		int len=Glass.Len;
		int hei=Glass.Hei;
		if (hei>5) hei--;
		Glass.Make(len,hei);

		Keyb.Set(KEY_3,0);
	}

	if (Keyb[KEY_LEFTCTRL] && Keyb[KEY_4])
	{
		int len=Glass.Len;
		int hei=Glass.Hei;
		if (hei<40) hei++;
		Glass.Make(len,hei);

		Keyb.Set(KEY_4,0);
	}



	if (Keyb[KEY_F1])
	{
		TetrisHelp();
	}

	generate_picture();

	scr.border(0,0,320,200,1);
		//㬥��蠥� ����প� ��� �����樨 ����� 䨣���
		if (NewfigureDelay) NewfigureDelay--;

		scr.setcolor(11);
		Brick.MakeSupervisor(&SpriteBox);

		//FIGURE &fg=Figure;

		//(������)
		//�᫨ ���� ������� ����� 䨣��� � ��� ������� ��௨祩
		//� ��� ����প�
		int N=Settings.MinimumBrickCount;	//�᫨ �����, �� ���� ����

		//scr.color=15;
		//scr.bkcolor=2;
		//scr<<cur(0,0);
		//scr<<N;

		if (GameMode & GAMEMODE_NEWFIGURE && (Brick.Counter<N)

		  && !NewfigureDelay)
		{
				NewFigureGenerate();

		}

		//*********** I. ����஥��� ����ࠦ���� *************

		static int show_pl_name=1;
		if (Keyb[KEY_F7])
		{
			show_pl_name^=1;
			Keyb.Set(KEY_F7,0);
		}
		if (show_pl_name)
		{
			//�����뢠�� ��� ��ப�
			scr.locateXY(Glass.x+8*1, Glass.y+Glass.BoxHei*Glass.Hei+1);
			scr.color=15;
			scr.bkcolor=0;
			//scr<<PlayerName;
			//scr<<"����� "<<PlayerNumber;

		}

		//�᫨ ०�� ���� �� ������� 䨣��� � ��� ������ 䨣��
		if (~GameMode & GAMEMODE_NEWFIGURE && !Movefig.Counter)
		{
			//�����뢠�� 䨣���
			Figure.Show();
		}

		//�����뢠�� ������ ����
		//Glass.ShowBackground(1);//Speed);

		//�����뢠�� ࠧ����騥�� ��௨�
		Brick.Animate();

		//�����뢠�� �⠪��
		Glass.Show();

	if (Keyb[KEY_PAGEUP] && Keyb[KEY_LEFTSHIFT])
	{
		Msg("������ �᫮ �窮� 1-�:       ",5);
		Tetris[0].Score=scr.InputNumber(5);

		Msg("������ �᫮ �窮� 2-��:       ",5);
		Tetris[1].Score=scr.InputNumber(5);
	}


	if (Score<35000)
	{
		if (Score>30000)
		{
			Background.Scroll(9, SPRITE::SCROLL_RIGHT);
			Background.Scroll(9, SPRITE::SCROLL_UP);
		}

		if (Score>25000)
		{
			Background.Scroll(9, SPRITE::SCROLL_RIGHT);
		}
		else	if (Score>20000)
		{
			Background.Scroll(9, SPRITE::SCROLL_UP);
		}
	}

		static int show_info=1;
	//�����뢠�� ��� ����� ���ࠫ� �窮�
	{
		int maxhei=21*8;
		int hei;

		//��।��塞 ४�� �� ����⪠ �����, ����� ����� ������
		for(int t=Rectable.MAX_RECORD-1; t>=0; t--)
		{
			if (Rectable.record[t].score>Score) break;

		}

		//�᫨ �窮� ���ࠫ� �����, 祬 ���ᨬ�쭮� �᫮
		if (Score>=MaxScore)
		{
			MaxScore=Score;
			//�᫨ �� �� ��⠭������ ४��
			if (!SetRecord)
			{
				SetRecord=1;

				//��頥� �⠪��
				/*for(int i=0; i<Glass.Hei; i++)
				for(int j=0; j<Glass.Len; j++)
				{

					//����
					ANIM ob;
					ob.x=Glass.x+8+j*8;
					ob.y=Glass.y+i*8;
					ob.SpriteIndex=Glass.Read(j,i);
					ob.Acceleraty=0.1*random(10);
					ob.Speedx=(0.1*random(50))*BaseSpeed;
					ob.Speedy=-2-0.1*random(80);
					if (random(2)) ob.Speedx*=-1;

					if (ob.SpriteIndex)
						Brick+=ob;

					Glass.Write(j,i,0);
				  }
				  */
			}
		}

		char str[]="HI SCORE";

		//�᫨ ४��
		if (Score>MaxScore)
		{
			for(int i=0; i<strlen(str); i++)
			{
				scr.locateXY(Glass.x+100+8, Glass.y+20+float(maxhei-40)/strlen(str)*i);
				scr.setcolor(1+random(15));
				scr<<str[i];
			}
		}
//		scr.setcolor(k);

		//!!!㢥��祭�� ᪮��� !!!
		//Speed=Score/4000;
		Speed=0;//Score/8000;
		//if (Speed>9) Speed=9;

		//㢥��稢���(�᫨ ��������) �᫮ �窮�
		int dec=5;
		if (relScore-dec>=0)
		{
			relScore-=dec;
			Score+=dec;
		}
		else if (relScore)
		{
			Score+=relScore;
			relScore=0;
		}

		scr.locateXY(Glass.x,Glass.y-16-8);

		if (show_info)
		{
        int dx=8;
		//�뢮� �६��� ����
		{
			unsigned int Tim=GameTimer/70;
			int min=Tim/60,sec=Tim%60;

			scr.color=22;
			scr.bkcolor=0;

			//scr<<"�६� ";
			//scr<<min<<':';

			scr.color=11;
			int x=Glass.x+dx,y=Glass.y-15-8;
			if (min<10)
			{
			graph_draw_num(x, y, 0);
			graph_draw_num(x+5, y, min);
			}
			else graph_draw_num(x, y, min);

			if (sec<10)
			{
			graph_draw_num(x+15, y, 0);
			graph_draw_num(x+15+5, y, sec);
			}
			else
			{
				graph_draw_num(x+15, y, sec);
			}

			graph_draw_symbol(x+10, y, ':');

			//scr.border(0,0,320,200,2);
			//Glass.x+9*8+dx,Glass.y-8-4+dy
			//if (sec<10) scr<<'0';
			//scr<<sec;
		}

		scr.locateXY(Glass.x,Glass.y-16);
		scr.bkcolor=0;
		scr.color=23;


		int olbk=scr.bkcolor;
		int olcl=scr.color;
		if (relScore)
		{
			static int time_score=0;
			time_score++;

			//if (time_score & 64)
			//{
			//	scr.bkcolor=9;
			//	scr.color=15;
			//}
			//else
			//	{
			//	scr.bkcolor=15;
			//	scr.color=9;
			//}
		}

		//
		//scr<<"�窨 "<<(unsigned long)Score;
		//scr.locateXY(Glass.x,Glass.y-8);
		scr.color=13;
		graph_draw_num(Glass.x+dx, Glass.y-16, Score);

		scr.bkcolor=olbk;
		scr.color=olcl;

//		scr.setcolor(12);
		//scr<<"����� "<<(unsigned long)Lines;
		scr.color=14;
		graph_draw_num(Glass.x+dx, Glass.y-8, Lines);

		//᪮���� ����
		int dy=-3;
		scr.setcolor(15);
		scr.bkcolor=0;
		//scr.border(Glass.x+9*8-4+1+dx,Glass.y-8-8+dy+1,13,13,3);
		//scr.locateXY(Glass.x+9*8+dx,Glass.y-8-4+dy);
		//scr<<Speed;
        scr.color=26;
		graph_draw_num(Glass.x+9*8+dx,Glass.y-8-4+dy-8, Speed);


		}
	}

		if (Keyb[KEY_F2])
		{
			Settings.BoxView++;
			Keyb.Set(KEY_F2,0);
			if (Settings.BoxView>5) Settings.BoxView=0;
		}

		if (Keyb[KEY_F9])
		{
			show_info^=1;
			Keyb.Set(KEY_F9,0);
		}

		//�᫨ ०�� ���� 'GAME OVER' � ������ ������ ����
		if (GameMode & GAMEMODE_GAMEOVER && Keyb[Down])
		{
			GameTimer=0;	//���뢠�� ⠩���

			GameMode^=GAMEMODE_GAMEOVER;

			Glass.Clear();

			Score=0;

			relScore=0;

			Lines=0;

			SetRecord=0;
		}

		//�᫨ ���� �뢮���� ������� 'GAME OVER'
		if (GameMode & GAMEMODE_GAMEOVER)
		{
			char * s="���� ��������";
			scr.setcolor(12);
			int len=Glass.Len*Glass.BoxLen;
			int len2=strlen(s)*scr.StepX;
			int z=(len-len2)/2;
			scr.locateXY(Glass.x+z+8, Glass.y+80);
			scr.bkcolor=16;
			scr.setcolor(15);
			scr<<s;
			scr.bkcolor=0;
			return;
		}


		//for(int i=0; i<256; i++)
		{
		  //	scr.line_down(i,0,23, i);
		}

		if (Keyb[KEY_F1] && Keyb[KEY_LEFTCTRL])
		{
			Keyb.Set(KEY_F1,0);
			Keyb.Set(KEY_LEFTCTRL,0);

			NewFigureGenerate();

		}

		if (Keyb[Pause]==1)
		{
			Keyb.Set(Pause,2);
			GameMode^=GAMEMODE_PAUSE;
		}

		//�᫨ ���� �뢮���� ������� '��㧠'
		if (GameMode & GAMEMODE_PAUSE)
		{
			scr.locateXY(Glass.x+16-8, Glass.y+80);
			scr.bkcolor=0;
			scr.setcolor(::Timer.GetTotalCounter()/10);
			scr<<"  �����";
			scr.bkcolor=0;

			return;
		}
		else
		{
			GameTimer+=BaseSpeed;
		}

		NextFigureShow();


	//*********** II. ������ **************

		char GlassUpdate=FALSE;
		//�᫨ �� ��௨� 㯠��, ���� �� ᤢ����� ����
		if ( (GameMode & GAMEMODE_NEWFIGURE) && Brick.Counter<N)
		{
			for(int i=0; i<Glass.Hei; i++)
			{
				int yes=1;
				for(int j=0; j<Glass.Len; j++)
				{
					if ( Glass.Read(j,i) )
					{
						yes=0;
						break;
					}
				}

				//�᫨ ���� ����� �����
				if (yes)
				{

				 int k=0; 	//������� �� �� ᠬ�� ����
				  //ᤢ����� �� ����
				  for(int y=i; y>=1; y--)
				  for(int x=0; x<Glass.Len; x++)
				  {
					int m=Glass.Read(x,y-1);
					if (m) {GlassUpdate=TRUE; k=1;}
					Glass.Write(x,y, m);
				  }

				  //����塞 �㫥��� ��ப�
				  for(int x=0; x<Glass.Len; x++)
				   Glass.Write(x,0,0);


				}

			}


		}
		if (GlassUpdate)
		{
			PlayNote(5,SoundVolume,200);
			PlayNote(4,SoundVolume,200);
			PlayNote(2,SoundVolume,200);
			PlayNote(1,SoundVolume,200);
		}


	//�����騥 ������
	/*if (!random(20))
	{
		ANIM ob;
		ob.x=random(320);
		ob.y=random(200);
		ob.SpriteIndex=SPRITE_TEST;
		ob.AlwaysAnimate=1;
		Brick+=ob;
	}*/

	 //�᫨ �� ���� ������� ����� 䨣��� - ����� ������� ��ன
	 if (~GameMode & GAMEMODE_NEWFIGURE)
	 {

		//��ࠥ� 䨣���
		Figure.Show(0);


//		int count_busy=0;
		int count_free=0;
		//�஢�ઠ ᢮������ �����
		for(int j=0; j<Glass.Hei; j++)
		{
			for(int x=0; x<Glass.Len; x++)
			{
				if (Glass.Read(x,j)) break;
			}
			if (x==Glass.Len) count_free++;
			else break;
		}

		//scr.locate(0,6);
		//scr<<"free="<<count_free;

		if (PlayerNumber==0)
		{
		if ( (count_free <7) && (CURRENT_MUSIC!=0))
		{
			CURRENT_MUSIC=0;
			MidiStop();
		}

		if ( (count_free >12) && (CURRENT_MUSIC != 4))
		{
			CURRENT_MUSIC=4;
			MidiStop();
		}
		}
		float Speedy=(0.03+Speed/120.0)*BaseSpeed;

		if (Keyb[Down] ==1) Speedy=0.5*BaseSpeed;

		if (Keyb[Down] ==1 && Keyb[KEY_RIGHTSHIFT])
		{
			KeyDownPress=1;
			Speedy=1;
		}

		if (Speedy>1) Speedy=1;

		//��⠥��� ������� 䨣��� ����
		{
			//�᫨ ��� ������ 䨣��
			if (!Movefig.Counter)
				Figure.y+=Speedy;

			//�᫨ ��������� ���� �����
			if (!Figure.Check())
			{
				//�����뢠�� 䨣���
				Figure.y--;
				Figure.Show();

				//���� ������� �� ������� ����� 䨣���
				GameMode|=GAMEMODE_NEWFIGURE;

				NewfigureDelay=8/BaseSpeed;


		//���뢠�� ����� ( �� ���� ᤥ���� ��ᨢ�� ! )
		{
			int t=0;
			//int m=random(256);
			int line_full=0;

			for(int i=0; i<Glass.Hei; i++)
			{
				int yes=1;
				for(int j=0; j<Glass.Len; j++)
				{
					if ( !Glass.Read(j,i) )
					{
						yes=0;
						break;
					}
				}

				//�᫨ ���� ����� �����
				if (yes)
				{
				  Lines++;
				  line_full++;
				  //����
				  for(int x=0; x<Glass.Len; x++)
				  //for(int x=0; x>0; x++)
				  {
					ANIM ob;

					ob.x=Glass.x+8+x*Glass.BoxLen;
					ob.y=Glass.y+i*Glass.BoxHei;

					ob.SpriteIndex=Glass.Read(x,i);
					ob.AlwaysAnimate=1;
					ob.Color=ob.SpriteIndex;

					ob.BoxLen=Glass.BoxLen;
					ob.BoxHei=Glass.BoxHei;

					ob.Speedy=(line_full * -0.1 -0.01*random(20))*BaseSpeed;
					ob.Speedx=(0.2+0.1*(x-5)*(line_full))*BaseSpeed;

					ob.Acceleraty=-(0.01+0.001*random(50))*BaseAccelerat;

					Brick+=ob;

				  }


				  //����塞 ��� �����
				  for(x=0; x<Glass.Len; x++)
				  {
					Glass.Write(x,i, 0);
				  }

					t++;
					//Sound.play(SND_XPLODE, NOSTOP);
					//PlayNote(1,10,200);
					//PlayNote(2,10,200);
					//PlayNote(4,10,200);
					//PlayNote(5,10,200);

				}

			}
			if (line_full)
			{
					//PlayNote(1,SoundVolume,200);
					PlayNote(3,SoundVolume,200);
					//void PlayNote(int note, int volume, int len);
					//PlayNote(4,SoundVolume,200);
					//PlayNote(5,SoundVolume,200);
			}
			else
			{
				unsigned long timer=Timer.Counter;
				StartRecord();
				PutRec(KEY_1,timer+0,1);
				PutRec(KEY_2,timer+1,1);
				PutRec(KEY_1,timer+30,0);
				PutRec(KEY_2,timer+30+10,0);

				//PutRecord(KEY_1,timer+20);

				//PlayNote(4,SoundVolume,200);

			}
			int glen=Glass.Len;
			int SCORE_SCALE=2;

			switch(t)
			{

				case 1:
				relScore += 10*glen*SCORE_SCALE;
				break;

				case 2:
				relScore += 30*glen*SCORE_SCALE;
				break;

				case 3:
				relScore += 70*glen*SCORE_SCALE;
				break;

				case 4:
				relScore += 150*glen*SCORE_SCALE;
				break;
			}
		}


			}
		}

		float Speed_x=(0.2+Speed/80.0)*BaseSpeed;

		if (Speed_x>1) Speed_x=1;

		//Speed_x=1;
		if (Keyb[KEY_LEFTSHIFT]) Speed_x=1;

		if ( Keyb[Left] == 1 &&	~GameMode & GAMEMODE_NEWFIGURE	)
		{
			if (Keyb[KEY_LEFTSHIFT])
			{
				Keyb.Set(Left,0);
			}

			//Keyb.Set(Left,0);

			//fg.Show(0);

			Figure.x -= Speed_x;
			//fg.x=int(fg.x);
			//fg.x--;

			if (Figure.x<0) Figure.x=0;

			if ( !Figure.Check() )
			{
				Figure.x+=Speed_x;
				//fg.x++;
				Figure.Show();
			}
		}

		/*scr.locate(0,2);
		scr<<fg.x;
		scr.locate(0,3);
		scr<<"sx=" << Speed_x;
		scr.locate(0,4);
		scr<<"sy=" << Speedy;
		scr.locate(0,5);
		scr<<"bs=" << BaseSpeed;
		*/

		// <RIGHT> ��⠥��� ��������� ��ࠢ�
		if (Keyb[Right] == 1 && ~GameMode & GAMEMODE_NEWFIGURE)
		{
			if (Keyb[KEY_LEFTSHIFT])
			{
				Keyb.Set(Right,0);
            }

			Figure.Show(0);

			Figure.x += Speed_x;

			//fg.x=int(fg.x);
			//fg.x++;

			if ( !Figure.Check() )
			{
				Figure.x -= Speed_x;
				//fg.x--;
				Figure.Show();
			}
		}

		//�᫨ �� ����� ����� � ������
		if (!Keyb[Left] && !Keyb[Right])
		{
			LRup=1;

			//float drob=fg.x-int(fg.x);

			//fg.x=int(fg.x);

			//if (drob>0.0 && drob<0.3) fg.x++;
		}


		/*if (Keyb[Up] && Keyb[Right] && Keyb[Left])
		{
				//��頥� �⠪��
				for(int i=0; i<Glass.Hei; i++)
				for(int j=0; j<Glass.Len; j++)
				{

					//����
					for(int z=0; z<3;z++)
					{
					ANIM ob;
					ob.x=Glass.x+8+j*8;
					ob.y=Glass.y+i*8;
					ob.SpriteIndex=Glass.Read(j,i);
					if (random(10)) ob.Acceleraty=0.1;
					else ob.Acceleraty=0.3;
					ob.Speedx=(0.1*random(100))*BaseSpeed;
					ob.Speedy=-1-0.1*random(100);
					if (random(2)) ob.Speedx*=-1;

					if (ob.SpriteIndex)
						Brick+=ob;
					}

					Glass.Write(j,i,0);
				  }
		}
		*/

		// <UP> ��⠥��� �������� ��� 䨣���
		if (Keyb[Up] == 1 && ~GameMode & GAMEMODE_NEWFIGURE)
		{
			Keyb.Set(Up,2);

			Figure.Show(0);	//��ࠥ� 䨣���

			Figure.Aspect++;	//���塞 �� ���

			int d=0;	//�� ᪮�쪮 ᬥ�⨫� 䨣���

			//�᫨ 䨣�� �� ����頥���
			if (!Figure.Check())
			{
				for(int i=0; i<3; i++)
				{
					d++;
					Figure.x--;
					if (Figure.Check())
					{
						break;
					}
				}
				if (i==3)
				{
					Figure.Aspect--;
					Figure.x+=d;
				}

			}
			//���� �����뢠�� 䨣���
			else
			{
				Figure.Show();
			}
		}
	 }

	 //static char r[256];
	 //static char g[256];
	 //static char b[256];

	 static char r_add[256];
	 static char g_add[256];
	 static char b_add[256];

	 static first_rgb=1;
	 if (first_rgb)
	 {
		first_rgb=0;
		for(int i=0; i<256; i++)
		{
			r_add[i]=1;
			g_add[i]=1;
			b_add[i]=-1;

		}
	 }
/*
	 {
		 for(int i=1; i<256; i++)
		 {

			 scr.GetColorRegister(i, &r[i], &g[i], &b[i]);
		 }

		 for(i=1; i<256; i++)
		 {
			 r[i] += r_add[i];
			 g[i] += g_add[i];
			 b[i] += b_add[i];

			 if (r[i]>62) r_add[i]=-1;
			 else if (r[i]==0) r_add[i]=1;

			 if (g[i]>62) g_add[i]=-1;
			 else if (g[i]==0) g_add[i]=1;

			 if (b[i]>62) b_add[i]=-1;
			 else if (b[i]==0) b_add[i]=1;


			 scr.SetColorRegister(i, r[i], g[i], b[i]);
		 }
	 }
*/

}

char	*song[10];
long	song_len[10];

void music(int n, int seq)
{
	if (Settings.EnableMusic==0) return;

	RegisterXmidi(song[n],song_len[n]); // register the song.
	//MidiStop();
	PlaySequence(seq);
	SetRelativeVolume(Settings.MusicVolume,0);

}

void load_music()
{
	char *music[]={
			"music\\0.xmi",
			"music\\1.xmi",
			"music\\2.xmi",
			"music\\3.xmi",
			"music\\4.xmi",
			"music\\5.xmi",
			"music\\6.xmi",
	};

	for(int i=0; i<MAX_MUSIC_NUMBER; i++)
		song[i] = fload( music[i],  &song_len[i]); // Load song into memory.
}

void ProgrammEditor()
{
	ifstream ifl;

	ifl.open("TETRIS.CPP");
	if (!ifl) return;

	scr.clear(9);
    scr.rectangle(0,0,320,200, 9);
	scr.x=0;
	scr.y=0;

	scr.bkcolor=9;
	scr.color=15;

	while (!ifl.eof())
	{
		unsigned char buf[512];

		ifl.getline(buf,510);
		for(int i=0; i<strlen(buf); i++)
		{
			if (buf[i]<32) buf[i]=32;
		}

		for(i=0; i<strlen(buf); i++)
		{
			scr<<char(buf[i]);
			if (scr.x> (320-8) ) {scr.x=0, scr.y+=8;}
		}
        scr<<"\n";

		scr.update();
		if (scr.y>180)
		{
			if (Keyb.ReadScan(3276700L) == KEY_ESC) break;
			scr.x=0;
			scr.y=0;
			scr.rectangle(0,0,320,200, 9);
			//scr.clear(9);
		}
	}

	ifl.close();
}

int TetrisStart();
extern int BigSymScale;
extern int StepRunLX;

void DrawPalette()
{
	scr.clear();



	int len=8,hei=8;

	for(int x=0; x<16; x++)
	for(int y=0; y<16; y++)
	{
		scr.rectangle(x*len,y*hei, len, hei,x*16+y);
		scr.update();
	}

	while(1)
	{
		if (Keyb[KEY_SPACE]) return;

		if (Keyb[KEY_R])
		{
			for(int i=1; i<256;i++)
			{
				scr.SetColorRegister(i, rand()%63,rand()%63,rand()%63);
			}
		}
		scr.pause(1);
	}



}

void main()
{
	scr.graphmode();

/*	int step=2;
	int c=15;

	for(int x=0; x<320; x+=step)
	{
		scr.line(x,0,x,200,c);
	}

	for(int y=0; y<200; y+=step)
	{
		scr.line(0,y,320,y,c);
	}
*/

	//scr.update();
	//Keyb.ReadScan();

	MOVEOBJ::Lim.Set(-24,-80,320+8,208);
	//����㦠�� �����
	{
		load_sprites();
		load_sounds();
		load_music();
		Rectable.Load();
		Settings.Load();
	}

	BigSymScale=Settings.BigSymScale;
	//RunLX=Settings.RunLX;
	RunLX=1;//Settings.RunLX;
	StepRunLX=Settings.StepRunLX;

	music(0,0);

	if ( TetrisStart() == 0) return;

	Settings.RunLX=RunLX;
	Settings.StepRunLX=StepRunLX;
	Settings.Save();

	Keyb.Clear();

	randomize();

	Tetris[0].Load("tetris0.ttr");
	Tetris[0].Glass.Load("tetris0.gls");


	Tetris[1].Load("tetris1.ttr");
	Tetris[1].Glass.Load("tetris1.gls");

	MaxScore=Rectable.GetMaxScore();

	int dx=10;

	Tetris[0].Figure.Glass=&Tetris[0].Glass;

	Tetris[0].Figure.x=2;
	Tetris[0].Figure.y=2;
	Tetris[0].Figure.Number=random(4);
	Tetris[0].Figure.Color=1+random(14);

	Tetris[0].Pause=KEY_P;
	Tetris[0].PlayerNumber=0;

	//���樠�����㥬 ��ࢮ�� ��ப�
	{
		randomize();
		TETRIS &tr=Tetris[1];
		tr.Figure.Glass=&tr.Glass;

		tr.Figure.x=2;
		tr.Figure.y=2;
		tr.Figure.Number=random(4);
		tr.Figure.Color=1+random(14);

		tr.Up=KEY_S;
		tr.Down=KEY_X;
		tr.Left=KEY_Z;
		tr.Right=KEY_C;
		tr.PlayerNumber=2;
		tr.Pause=KEY_A;
		tr.PlayerNumber=1;

//		tr.GameMode = TETRIS::GAMEMODE_GAMEOVER;
	}

restart:

	int k;

	strcpy(Tetris[0].PlayerName, "1 ��ப");
	strcpy(Tetris[1].PlayerName, "2 ��ப");

	show_task_list=FALSE;

	do
	{

		k=Tetris[0].menu();

		if (k==2) redefine_keys();

		if (k==3) Tetris[0].Settings1();
		if (k==4) Tetris[0].Settings2();

		if (k==5)
		{
			Rectable.Show(0);
			scr.update();
			while (!Keyb[KEY_ESC])
			{
			}
		}

		if (k==6)
		{
			Tetris[0].Figure.Show(0);
			Tetris[0].Glass.Save("tetris0.gls");
			Tetris[0].Save("tetris0.ttr");

			Tetris[1].Figure.Show(0);
			Tetris[1].Glass.Save("tetris1.gls");
			Tetris[1].Save("tetris1.ttr");

			Settings.RunLX=RunLX;
			Settings.StepRunLX=StepRunLX;
			Settings.Save();
			return;
		}

		if (k==7)
		{
			FontEditor(0);
			//ProgrammEditor();
		}

		if (k==8) TetrisHelp();

		if (k==9)
		{
			show_task_list=TRUE;
			TetrisStart();

			show_task_list=FALSE;
		}

	}while (k>1);

	//����᪠�� ⠩��� �� ����� 70 ��


	//�����塞 ��᫥����⥫쭮��� ��砩��� �ᥫ
	{
		randomize();
	}

	int GAME=1;

	MidiStop();

	if (k==0 || k==1)
	{
		MidiStop();
		music(CURRENT_MUSIC,0);
	}

	if (k==1)
	{
		scr.clear();

		Keyb.Clear();

		static int first_run=1;

		if (first_run)
		{
			first_run=0;

			//if (Tetris[0].PlayerName[0]==0)
			{
				//Msg(" 1 ����� ���:                  ",9);
				//scr.InputString(15, Tetris[0].PlayerName);
			}

			//if (Tetris[1].PlayerName[0]==0)
			{
				//Msg(" 2 ����� ���:                  ",9);
				//scr.InputString(15, Tetris[1].PlayerName);
			}
		}
	}

	while ( GAME)
	{

		//���⪠ ��࠭�
		scr.clear();
		SoundModule(0);
		//०�� ���� ��ப��
		if (k==1)
		{

			Tetris[0].Glass.x=24*8-dx+16-4;
			Tetris[0].Glass.y=24;

			if (Tetris[0].Glass.BoxHei>8) Tetris[0].Glass.y=0;

			Tetris[1].Glass.x=2;
			Tetris[1].Glass.y=24;

			if (Tetris[1].Glass.BoxHei>8) Tetris[1].Glass.y=0;

			Tetris[0].game();
			Tetris[1].game();

			static int Show_Score_Counter=0, compare=0;

			long a=Tetris[0].Score, b=Tetris[1].Score;

			// 100 0

			if (compare>0 && a<b
					||
				compare<0 && a>b
					||
				!compare && a!=b)
			{
				Show_Score_Counter=180;
				compare=a-b;
			}

		}

		if (k==0)
		{
			Tetris[0].Glass.x=15*8-dx;
			Tetris[0].Glass.y=24;

			if (Tetris[0].Glass.BoxHei>8) Tetris[0].Glass.y=0;

			Tetris[0].game();
		}

		//int first_record=0;

	if (Keyb[KEY_F8])
	{
		DrawPalette();
	}

	if (Keyb[KEY_F11])
	{
		Settings.ShowTableRecord^=1;
		Settings.Save();
		Keyb.Set(KEY_F11,0);
	}


	int x_record=0;
	if (k==0)
	{
		x_record=2;
	}
	else
	{
		x_record=100-8+30;
	}

	if (Settings.ShowTableRecord)
	{

	  RECTABLE rectable_new=Rectable;

	  rectable_new.PutNewRecordNameColor(Tetris[0].PlayerName, Tetris[0].Score,9);
	  rectable_new.PutNewRecordNameColor(Tetris[1].PlayerName, Tetris[1].Score,12);

		for(int i=0; i<rectable_new.MAX_RECORD-1; i++)
		{
			scr.bkcolor=rectable_new.record[i].color;

			scr.locateXY(x_record,(i+1)*8+23);

			if (rectable_new.record[i].color!=0)
			{
				scr.color=15;
			}
			else scr.color=25;

			if (i<9)
			{
				scr<<i+1;
			}
			else
			{
				char c[2]={'A'+i-9,0};
				scr<<c;
			}

			scr<<" ";


			int flag_end=0;

			for(int j=0; j<5; j++)
			{
				char a=rectable_new.record[i].name[j];

				if (a==0) flag_end=1;

				if (!flag_end)	scr<<a;
				else scr<<" ";

			}

			scr<<" ";

			//if rectable_new.record[i].score
			scr.putNumb(rectable_new.record[i].score);
		}
	}

	//scr.locate(0,5);
	//scr<<CURRENT_MUSIC;

	if (Keyb[KEY_F10])
	{
		Settings.ShowRealTime^=1;
		Settings.Save();
		Keyb.Set(KEY_F10,0);
	}

	if (Settings.ShowRealTime)
	{

		unsigned char hour, min, sec;
		GetRealTimeClock(hour, min, sec);
		char buf[32];

		sprintf(buf,"%02i:%02i:%02i", hour, min, sec);

		scr.color=9;
		scr.locateXY(x_record+20, 184);
		scr<<buf;
	}


		if (Keyb[KEY_TAB])
		{
			dx=dx++ % 200;
			static int x=0;

			x++;
			if (x>4) x=0;

			//scr.pause(100);
		}

		if ( (SequenceStatus() != SEQ_PLAYING) )
		{
			music(CURRENT_MUSIC,0);
		}

		//Mouse.Draw();
		//scr.setcolor(7);
		//scr<<cur(8,24)<<"(C) 1997 Lapshin Ewgeny";

		Movefig.Animate();	//�����뢠�� ������騥�� 䨣���

		//Sprite.Show(130,194, SPRITE_AUTORNAME);

		//����塞 ����� ������� ᪮����
/*		if (Timer.Counter && Timer.Freq>19)
		{
			Timer.Restart(Timer.Freq-1);

			float n=70.0/Timer.Freq;

			BaseSpeed=n;
			BaseAccelerat=n*n;

			//��������� 11 ������� 2010
			//BaseSpeed=1;

			scr<<cur(0,0)<<"?";
		}
*/

		//scr.border(0,0,320,200,15);
		if (Keyb[KEY_F12])
		{
			Settings.ShowRunLine^=1;
			Settings.Save();
			Keyb.Set(KEY_F12,0);
		}

		if (Settings.ShowRunLine)
		{
			BigSymScale=Settings.BigSymScale;
			scr.color=15;
			DrawRunLine(200-BigSymScale*8);
		}

		if (Keyb[KEY_F4])
		{
				Msg("MinumumBrickCount:    ",2);
				Settings.MinimumBrickCount=scr.InputNumber(2);
				Settings.Save();
		}


		//�᫨ ���� ��㧠
		if (Keyb[KEY_PAUSE])
		{
			Msg(" PAUSE ",1);
			Keyb.ReadScan();
			//�⮡� �� �뫮 ������� �ࠡ��뢠��� ᭨����� �����
			//Timer.Counter=0;
		}

		if (Keyb[KEY_F2])
		{
			if (CURRENT_MUSIC++ >7) CURRENT_MUSIC=0;
			Keyb.Set(KEY_F2,0);

			MidiStop();

		}

		if (Keyb[KEY_ESC])
		{
			//scr.setcolor(14);
			//if (MsgText("��室 ?", "��", "���", 1) == 0)
			{
				GAME=0;
			}
		}

		//ᨭ�஭����㥬�� �� ⠩����

		//Timer.Counter=0;

		scr.wait();
		scr.update();
	}


	goto restart;
}
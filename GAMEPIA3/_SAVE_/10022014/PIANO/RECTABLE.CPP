#include <conio.h>
#include <stdio.h>
#include <fstream.h>

#include "screen.h"
#include "debug.h"
#include "keyboard.h"
#include "rectable.h"
#include "string.h"
#include "stdlib.h"
#include "timer.h"

//***********************************************************************
//********* RECTABLE.CPP - �ணࠬ�� ��� ࠡ��� � ⠡��楩 ४�म� *****


//�������� ⠡����
void RECTABLE::Show(int newnum)
{
	//int clr=scr.color;
	//int bk=scr.bkcolor;

	for(long j=0; j<16*20ul; j++)
	{
		scr.clear();
		//�뢮� ⠡���� �� ��࠭
		int x=(40-16-NAME_MAXLEN)/2-1,
		y=28;//+(25-MAX_RECORD*2)/2;

/*		scr.rectangle(x*8-8,y*8-8,
			(11+NAME_MAXLEN)*8+8*7,
			(MAX_RECORD*2+2)*8,
			143+16*4-5);*/

		//int hei=(MAX_RECORD*2)*8, len=(11+NAME_MAXLEN)*8+8*7;
		//st_x=x*8-8, st_y=y*8-8+1;

		//for(int i=0; i<hei; i++)
		// scr.line(st_x,st_y+i, st_x+len,st_y+i, 144+i%16);

		scr.color=15;

		for (int i=0; i<MAX_RECORD; i++)
		{
			//��㥬 ᨭ��, �᫨ �� ��ப� ⮣� ४�ठ
				int hei=8, len=(18+NAME_MAXLEN)*8,
				st_x=x*8-8, st_y=(y)+i*8-6-1;

			if (i==newnum)
			{
				for(int z=0; z<hei; z++)
				 scr.line(st_x,st_y+z, st_x+len,st_y+z, 144+16*3+z%16);
/*
				scr.rectangle(
					x*8-8,
					(y+i*2)*8-5,
					(18+NAME_MAXLEN)*8+2,
					14,
					144+16*3+3+j%13);*/
			}
			scr.locateXY(st_x,st_y);
			if (i<9)
			{
			scr.putNumb(i+1,1);
			}
			else
			{
			char c[]={'A'+i-9,0};
			scr<<c;
			}
			scr.putSymbol(' ');
			record[i].name[NAME_MAXLEN-1]=0;

			char zx[8];

			scr<<record[i].name;
			scr.locateXY(st_x+NAME_MAXLEN*8+4, st_y);

			//�᫨ �����, � 5 ������
			if (record[i].score<65535L)
				scr.putNumb(record[i].score,5);
			//���� ᪮�쪮 ����
			else
				scr.putNumb(record[i].score);

			scr<<" | ";
			scr.putNumb(record[i].lines,3);

			scr<<" ";

			int t=record[i].time;

			sprintf(zx,"%02i:%02i", t/60, t%60);

			scr<<zx;
		}
		scr.update();

		while (!::Timer.Counter){}
		::Timer.Counter=0;

		if (Keyb[KEY_ESC]) break;
	}
}

//����� ���ଠ�� � ��᪠
void RECTABLE::Load()
{
	for(int i=0; i<MAX_RECORD; i++)
	{
		record[i].color=0;
		record[i].lines=0;
		record[i].time=0;
		record[i].score=0;
		strcpy(record[i].name, "");
	}


	ifstream ifl;

	//��⠥��� ������ 䠩� � ⠡��楩
	ifl.open("records.dat", ios::binary);

	char *name[]={ "���३", "����", "���", "Wiz",
	"����", "����"};

	//�᫨ ⠡��� �����, ����塞 �, �� ����
	if (!ifl)
	{
		for(int i=0; i<MAX_RECORD; i++)
		{
			strcpy(record[i].name, name[random(6)]);
			record[i].score=(22-i)*500;
			record[i].lines=record[i].score/150;
			record[i].time=(MAX_RECORD-i+1)*78;
			record[i].color=0;
		}
		//��࠭塞 �� �� ��᪥
		Save();
		return;
	}
	//���� �⠥� ⠡���� � ������
	else
	{
		int k=sizeof(RECORD)*MAX_RECORD;
		ifl.read( (char far*) record, k );
		//����뢠�� 䠩�
		ifl.close();
	}


}

//������� ⠡���� �� ���
void RECTABLE::Save()
{
	ofstream ofl;

	//��⠥��� ������ 䠩� � ⠡��楩
	ofl.open("records.dat", ios::binary);

	//�᫨ ⠡��� �����, ����塞 �, �� ����
	if (!ofl)
	{
		Debug("RECTABLE::Save - can't save table to disk");
		return;
	}
	//���� ��襬 ⠡���� �� ���
	else
	{
		ofl.write( (char*) record, sizeof(RECORD)*MAX_RECORD );

		//����뢠�� 䠩�
		ofl.close();
	}

}

//���������
RECTABLE::RECTABLE()
{
	//���뢠�� ⠡���� � ��᪠
	//Load();
}

//��������
RECTABLE::~RECTABLE()
{
	//��࠭塞 ���ଠ�� �� ��᪥
	//Save();
}

unsigned long RECTABLE::GetMaxScore()
{
	return record[0].score;
}

//============== ��������� �������� ���� �窨 =============
void RECTABLE::PutNewRecordNameColor(char *name, unsigned long newscore, char Color)
{
	//㧭���, ��襫 �� ४�� � ⠡����
	if (newscore>record[MAX_RECORD-1].score)
	{
		strcpy(record[MAX_RECORD].name, name);
		record[MAX_RECORD].score=newscore;
		record[MAX_RECORD].color=Color;
		//record[MAX_RECORD].lines=lines;
		//record[MAX_RECORD].time=_time;

		//���஢�� ���ᨢ� �� �����⠭��
		for (int i=0; i<=MAX_RECORD; i++)
		{
			for (int k=0; k<=MAX_RECORD-1; k++)
			{
				if (record[k].score<record[k+1].score)
				{
					RECORD tmp=record[k];
					record[k]=record[k+1];
					record[k+1]=tmp;
				}
			}
		}
	}
}


//============== ��������� �������� ���� �窨 =============
char * RECTABLE::PutNewRecord(char * _name, unsigned long newscore, unsigned long lines, unsigned int _time)
{
	//㧭���, ��襫 �� ४�� � ⠡����
	if (newscore>record[MAX_RECORD-1].score)
	{

		Msg("��� ���:                 ",9);

		char player_name[NAME_MAXLEN];

		scr.InputString(
			NAME_MAXLEN, player_name);

		if (strlen(player_name)>0)
		{
			strcpy(record[MAX_RECORD].name, player_name);
		}
		else
		{
			strcpy(record[MAX_RECORD].name, _name);
		}

		record[MAX_RECORD].score=newscore;
		record[MAX_RECORD].lines=lines;
		record[MAX_RECORD].time=_time;

		scr.setbkcolor(0);
		scr.update();

		int newnum;

		//��।���� �����, ��� ����� ४�� ��襫 � ⠡����
		for (int i=0; i<MAX_RECORD; i++)
		{
			if (record[i].score<newscore)
			{
				newnum=i;
				break;
			}
		}

		//���஢�� ���ᨢ� �� �����⠭��
		for (i=0; i<=MAX_RECORD; i++)
		{
			for (int k=0; k<=MAX_RECORD-1; k++)
			{
				if (record[k].score<record[k+1].score)
				{
					RECORD tmp=record[k];
					record[k]=record[k+1];
					record[k+1]=tmp;
				}
			}
		}

		Show(newnum);
		scr.update();

		//�����뢠��
		Save();


		//scr.pause(140);

		//�⬥�塞 �⠭����� ��ࠡ��稪 ���뢠��� ����������
		Keyb.SetInputMode(KEYBOARD_NOOLDINTERRUPT);

		//���� ������ ������
		Keyb.ReadScan();

		return record[newnum].name;
	}

	return NULL;
}


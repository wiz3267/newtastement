#ifndef __TETRIS_H
#define __TETRIS_H

#include "moveobj.h"

enum {
	FGSHOW_X=150,
	FGSHOW_Y=100
};

void load_sprites();

enum {
	SPRITE_BORDER=0x200,
	SPRITE_AUTORNAME,
	SPRITE_POINTER,
	SPRITE_TEST,
	SPRITE_STAR
};

//����� MOVEFIGURE - ��६������� 䨣��
class MOVEFIGURE : public ANIM
{
public:

	MOVEFIGURE(SPRITE *spr) : ANIM(spr) {}

	MOVEFIGURE() {}

	FIGURE Fig;

	void operator+=(MOVEFIGURE &ob) { New(&ob,sizeof(ob));}

	virtual	void	Show()
	{
		Fig.Show(x,y);
	}
};


//����� TETRIS
class TETRIS
{
public:

	enum { SND_POP, SND_XPLODE };

	enum {
		GAMEMODE_GAMEOVER=0x01,		//�뤠���� ����������� �������
		GAMEMODE_PAUSE=0x02,		//०�� ����
		GAMEMODE_NEWFIGURE=0x04		//������� ����� 䨣���
	};

	GLASS Glass;	//�⠪��
	FIGURE	Figure;

	float Timer;

	long Score, Lines;

	int
		SetRecord,	//�� �� ��⠭����� ४��
		Speed,		//᪮���� �������
		relScore,	//�⭮�⥫�� �窨
		GameMode,	//०�� ����
		Up,		//������ �����
		Down,		//������ ����
		Left,		//�����
		Right,		//��ࠢ�
		NewfigureDelay,	//����প� ��� �����樨 ����� 䨣���
		PlayerNumber,	//����� ��ப�
		Pause,		//��㧠
		LRup
	;

	char PlayerName[64];

	ANIM Brick;

public:

	int Save(char *fname);	//��࠭��� ���ﭨ�
	int Load(char *fname);	//����㧨�� ���ﭨ�

	void game();	//����� ����

	int  menu();	//����

	TETRIS();	//���������

	~TETRIS();	//��������

	void NewFigureGenerate();
	void NextFigureShow();
	void Settings1();
	void Settings2();

	unsigned char GenerateFigureColor();

};

#endif
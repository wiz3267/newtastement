#ifndef __SCREEN_H
#define __SCREEN_H

#include "rects.h"

class FONT
{
	//�������� ����᫨��� ࠧ��஢ 䮭�
	enum {
		FONT8x8,
		FONT8x14,
		FONT8x16
	};

	char * FontAddres;		//���� 䮭�
	char SymHigh;			//���� ������ ᨬ����

public:
	FONT(int size=FONT8x8); //��������� size - ࠧ��� 䮭�
	~FONT();  	//��������

	//�������� �� ��࠭� ᨬ��� � �������⥫�� 梥⮬ color
void	Show(int x,int y, int symbol, char color);
};

//=============== ���ᠭ�� ����� 'SCREEN'  - ��࠭ ========================
//  �ਬ�砭��: ⠪�� ������, ��� '�뢮� �� ��࠭', '���⪠ ��࠭�'
// �⭮����� �� � 䨧��᪮� VRAM, � � ⥭����� ��࠭� (doublebuffer)
// �⮡� ���ଠ�� ������ �� ��࠭� ����室��� �맢��� �㭪�� update
class SCREEN {

// ********************** ������-����� ******************************
public:

	enum
	{
		DOUBLEBUF=1,
		NODOUBLEBUF=0,
		//���祭�� 䫠�� ��� �㭪樨 putBigSym
		RANDOM_COLOR=0x01,
		ONE_COLOR=0x02
	};

static	char	constructor_run;//�� �� ����饭 ���������
static	int	CountObjects;	//�᫮ ��쥪⮢ ⨯� SCREEN
static	char	*DoubleBuffer;	//���� ⥭����� ��࠭�
static	char	*VideoMemory;	//���� 䨧��᪮� ����� �����

public:

	int flip;				//��ઠ�쭮 �� update()
	char	bkcolor;		//梥� ������� 䮭�
	char 	color;			//梥� �ᮢ����
	int	x,y;				//���न���� �����
	int StepX;			//蠣 ���饭�� �� �뢮�� ᨬ�����

// ********************** �������-����� *************************
public:
		//���祭�� State ��� bitblt
	enum {
		FLIP_HORZ=0x01,	//��ॢ����� �ࠩ� �� ��ਧ��⠫�
		FLIP_VERT=0x02,	//��ॢ����� �ࠩ� �� ���⨪���
		TRANSP=0x04	//���� �஧��� ���ᥫ�
	};

	enum {
		ONLY_DIGIT=0x01	//���� InputString �㤥� ������� ⮫쪮 ����
	};
	void ScrollUp(int dy);
	void draw_circle (int x0, int y0, int r, int color);
	void fill_circle (int x0, int y0, int r, int color);
	void draw_ellipse (int x0, int y0, int ra, int rb, int color);
	void fill_ellipse (int x0, int y0, int ra, int rb, int color);
	void flip_double_buffer();

	void FontSave(void);
	void bitblt(int X,int Y,int Len,int Hei, char far* Data, char State);
	void bitblt(int x,int y,int len,int hei, char far * addr); //���-���⨭�
	void bitbltTP(int x,int y,int len,int hei, char far * addr); //���-���⨭� � �஧�筮����
	void bitblt_part(int x,int y,int x_in, int y_in,int Len, int Hei,
int FactLen,int FactHei, char far* Data, char State);
	void border(int x,int y,int len,int hei, int color);	//���� ࠬ��
	void Center(int y, char far*);	//�뢮��� ������� �� 業���
	void clear(int=0);				//���⪠ ��࠭�
	void CreatePalette();			//ᮧ���� �������
	void DecrementPalette();		//㬥����� �������
	void IncrementPalette();		//㢥����� �������

	void GetColorRegister(unsigned char,char *r,char*g,char*b);//����祭�� 梥⮣� ॣ����
	void graphmode();			//��⠭���� ���. ०���
	unsigned long InputNumber(unsigned int DigitCounter);	//���� �᫠
	void InputString(int len, char far * str,int flag=0);//���� ��ப� � ���ᨢ str, ������ len, ��稭�� � ���न���
	void locate(int x,int y);	//��������� ���न��� �����
	void locateXY(int x,int y);	//��⠭���� ����� � �筮���� �� ���ᥫ�
	void line(int x1,int y1,int x2,int y2, char color);    //���� �����
	void line_right(int x,int y,int len, char color);
	void line_down(int x,int y,int hei, char color);
	void mode(char output_mode);    //��⠭���� ०��� �⮡ࠦ����: �ࠧ� �� ��࠭

	SCREEN& operator << (char*);
	SCREEN& operator << (unsigned long);
	SCREEN& operator << (unsigned);
	SCREEN& operator << (int);
	SCREEN& operator << (char);
	SCREEN& operator << (double);

	SCREEN& operator<< (SCREEN& (*_f)(SCREEN&))
				{ return (*_f)(*this); }


	void pause(int n);
	void putpixel(int x,int y,int color); //�뢮� �窨
	void putBigNumb(long num);		//����� �᫠
	void putBigNumb(long num,char width);	//����� �᫠ � ����������� �㫥�
	void putBigSym(unsigned char s,char color,char flag);	//�뢮� ����讣� ᨬ����
	void putBigText(char far * text,char color);
	void putBigText(char far * text,char color,char flag);
	void putBigText(char far * text,char color,char flag, char off); //�뢮� ����讣� ⥪��

	void putSymbol(unsigned char s);			//����� �� ��࠭� ᨬ����
	void putSymbol(int x,int y, unsigned char s);	//����� ᨬ���� � �������� ���न����
	void putText(char far *);		//����� ⥪��
	void putNumb(unsigned long num);			//����� �᫠
	void putNumb(unsigned long num,char width);	//����� �᫠ � ����������� �㫥�
	void rectangle(int x,int y,int len,int hei,int color);
	void rectangle(RECT &rt,int color);
	SCREEN(void);				//���������
	~SCREEN(void);				//��������
	void setbkcolor(char c){bkcolor=c;};	//��⠭���� 梥� �뢮��
	void setcolor(char c){color=c;};	//��⠭���� 梥� �뢮��
        void SetColorRegister(unsigned char n,char,char,char);//��⠭���� 梥⮢��� ॣ����
	void Set320x240Mode();			//��⠭����� ०�� 320*240, 256 梥⮢
	void textmode();			//��⠭���� ⥪�⮢��� ०���
	void update(void);			//���������� ��࠭�
	void update(RECT);			//���������� ��࠭�
	void wait(void);			//���� ���⭮�� 室� ���
};
//================== end of class 'SCREEN' ==============================

inline        SCREEN& endl(SCREEN& ob) { return ob; }

class MANIPMY {
public:
	SCREEN& (*fun) (SCREEN& , int);
	int ag;

public:
	MANIPMY ( SCREEN& (*_f)(SCREEN&, int) , int _a)
	{ fun=_f; ag=_a; }

        friend SCREEN& operator<< (SCREEN& obj, MANIPMY& _f)
        {
		(_f.fun) (obj, _f.ag);
		return obj;
	}
};

SCREEN& _setcolor(SCREEN&ob, int);
SCREEN& _setlocate(SCREEN&ob, int);
MANIPMY& setcolor(int);
MANIPMY& cur(int,int);


extern SCREEN scr;

#endif
#ifndef __WINDOWS_H
#define __WINDOWS_H

#include "rects.h"
#include "debug.h"

typedef signed long LONG;
typedef LONG LRESULT;
typedef unsigned int UINT;
typedef LONG LPARAM;
typedef LONG WPARAM;
typedef UINT HWND;


//********************************************************************
//************************** ���� ᮮ�饭�� **************************
//********************************************************************
enum {

        WM_LBUTTONDOWN,	//����� ����� ������ ��� �� ����७��� ������ ����
        WM_RBUTTONDOWN,//����� �ࠢ�� ������ ��� �� ����७��� ������ ����

        WM_LBUTTONUP, //���饭� ����� ������ ��� �� ����७��� ������ ����
        WM_RBUTTONUP,//���饭� �ࠢ�� ������ ��� �� ����७��� ������ ����
        WM_MOUSEMOVE,//��६�饭�� ��� �� ����७��� ������ ����

        WM_COMMAND   //ᮮ�饭�� �� ������

};

//*********************************************************************
//************ ������� MESSAGE - ����뢠�� ᮮ�饭�� ****************
//*********************************************************************
struct MESSAGE
{
	UINT Code;
        WPARAM wParam;
        LPARAM lParam;
        MESSAGE(UINT m,WPARAM wp,LPARAM lp) : Code(m), wParam(wp), lParam(lp) {;}
        MESSAGE() : Code(0), wParam(0), lParam(0) {;}
};


//********************************************************************
//**************** ����� WINDOW - ���� *******************************
//********************************************************************
class WINDOW
{
        friend MOTHERWINDOW;

protected:
        WINDOW *Next;		//㪠��⥫� �� ᫥���饥
        WINDOW *Prev;		//� �।��饥 ���� � ᯨ᪥

        MOTHERWINDOW *Mother;	//㪠��⥫� �� ����-����

        int x,y,len,hei;	//���न���� ���� �⭮�⥫쭮 ����
        RECT Rect;		//���⮯�������� ���� �⭮�⥫쭮 ��砫� ��࠭�
        char *text;		//⥪�� ����
        int ID_Number;		//㭨����� ����� ����
	int Status;		//���ﭨ� ����
public:
	void RenewRect();
//	WINDOW(RECT, int id, char *str);
	WINDOW(int,int,int,int, int id, char *str);
virtual	~WINDOW()
	{
		if (text) delete text;
		else Debug(" Window::~Window text == 0 ?! ");
	}

	//�⨬ ������ ����� ����ࠪ��
virtual void Show()=0;		//�������� ����
	//�㭪�� ����
	//msg - ��� ᮮ�饭��, wParam, lParam - �������⥫�� ��ࠬ����
virtual	LRESULT WndProc(MESSAGE&) = 0;
//	void MousePress(int);	//������ �� ������ ���
	void Set(int x,int y,int len,int hei, int id, char *txt);
};

//********************************************************************
//****************** ����� MOTHERWINDOW ���ਭ᪮� ���� *************
//********************************************************************
class MOTHERWINDOW
{
        friend BUTTON;
        friend WINDOW;

	RECT Place;		//�ᯮ������ ����
        int ID_Mother;		//㭨����� ��� ���� - ����
        int Status;		//���ﭨ� ���� - ����

        char Background;	//梥� ���� ���� (������� ����� ��� ���୨�)

        int ActiveWindow;	//������ � ࠬ���

	WINDOW *First;		//㪠��⥫� �� ��ࢮ� � ��᫥���� ����
        WINDOW *Last;		//ᯨ᪠

	int Counter;		//�᫮ ���� � ᯨ᪥

public:

	LRESULT (*WndProc)(MESSAGE&);	//㪠��⥫� �� �㭪�� ����

        MOTHERWINDOW(int x,int y,int len,int hei, int id);
        MOTHERWINDOW(RECT rt, int id);
	int Add( WINDOW *ob);	//���������� ���� � ᯨ᮪
        void moveto(int dx, int dy);	//��६����� ���� �⭮�⥫쭮 ���
        				//���न���
        void move(int x,int y);		//��⠭����� ���� ���न���� ����
        void ShowAll();			//�������� �� ���� � ᯨ᪥
        void SendMessages();		//��᫠�� ���୨� ����� ᮮ�饭��
        void SetBackground(char);
        int Del(int ID_Number);		//㤠���� ��।������� ���୥� ����
	void Main(MESSAGE&);
};

//**********************************************************************
//**************************** ����� ������ ****************************
//**********************************************************************
class BUTTON : public WINDOW
{
	int TextColor;	 //梥� ⥪��
        int BorderColor; //梥� ࠬ�� ����� ������

public:
//��������� ���ﭨ� ����
        enum
	{
		PRESS   = 0x0001,	//������ �����
		NOPRESS = 0x0002,	//������ �������
                BORDER  = 0x0004,	//����� ������ ���� �ᮢ��� ࠬ��
	};

	BUTTON(int x,int y,int len,int hei, int id, char *txt);
	BUTTON(int x,int y,int id, char *txt);
	LRESULT WndProc(MESSAGE&);
        void Show();
};

#endif
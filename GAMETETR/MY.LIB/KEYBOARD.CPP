////////////////////////////////////////
// Copyright (C) ���設 �������, 1997 //
////////////////////////////////////////

//*********************************************************************
//**** 'KEYBOARD.CPP' -�㭪樨-童�� ����� KEYBOARD ******************
//*********************************************************************
#include <dos.h>

enum {FALSE,TRUE };

//��� 䠩��
#include "process.h"
#include "debug.h"
#include "keyboard.h"
#include "screen.h"


//ᮧ���� �।��।������ ��ꥪ�
KEYBOARD Keyb;


//==========����᪨� (� ���� � ᥣ���� ������) ��६����==============
//�᫮ ᮧ����� ��ꥪ⮢
int KEYBOARD::CountObject=0;

 //㪠��⥫� �� ᮧ����� ��ꥪ�
KEYBOARD far * KEYBOARD::This;

char * KEYBOARD::KEY[]={
// ���� 00-0F
"WHAT?","ESC", "NUM1", "NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7",
"NUM8", "NUM9", "NUM0", "DASH",
"EQUAL", "BACK SPACE","TAB",
// ���� 10-1F
"Q","W","E","R","T","Y","U","I","O","P","{","}","ENTER","LEFT CTRL", "A","S",
// ���� 20-2F
"D","F","G","H","J","K","L","SEMICOLON","RQUOTE", "LQUTE",
"LEFT SHIFT", "BACK SLASH", "Z","X","C", "V",
// ���� 30-3F
"B","N","M", "COMMA", "PERIOD", "SLASH", "RIGHT SHIFT", "PADMUL", "LEFT ALT", "SPACE",
"CAPS LOCK", "F1", "F2", "F3", "F4", "F5",
// ���� 40-4F
"F6","F7","F8","F9","F10","NUM LOCK", "SCROLL LOCK", "PAD7", "PAD8", "PAD9",
"PADSUB", "PAD4", "PAD5", "PAD6", "PADADD", "PAD1",
// ���� 50-5F
"PAD2", "PAD3", "PAD0", "PADDEL","0x54", "0x55", "0x56", "F11", "F12",
"0x59", "0x5a", "0x5b", "0x5c", "0x5d", "0x5e", "0x5f",

// ���� 60-6F	//��� ᮡ�⢥���
"UP", "DOWN", "LEFT", "RIGHT", "INSERT", "DELETE", "HOME", "END","PAGE UP","PAGE DOWN",
"RIGHT ALT", "RIGHT CTRL", "PRINT SCREEN", "PAUSE", "PADENTER", "PADDIV"
};

//-------------'Check' - �஢�ઠ, ����� �� ������ -------------------
// key - ᪠�-��� �஢��塞�� ������
// ������:
//   1 TRUE  - �����
//   0 FALSE - �� �����
int KEYBOARD::Check(unsigned char key)
{
	return Keys[key];
}
//------------------------------------------------------------------------


int KEYBOARD::AnyKey()			// ������ 1 - �᫨ ����� any key
					//         0 - �᫨ ��祣� �� �����
{
	for(int i=1; i<127; i++)
	{
		if (Keyb[i]) return 1;
	}
	return 0;
}


//------------'ReadScan' - ���뢠�� ᪠�-��� ---------------------
char KEYBOARD::ReadScan(unsigned long time)
{
	//��頥� ���� ����������
	Clear();

	unsigned long t=time;
	for(int i=0;;i++, i&=127)
	{
		if (time!=0)
		{	t--;
			if (t==0) break;
		}
		if (Check(i)) break;
	}

	t=time;
	while (Check(i))
	{
		if (time!=0)
		{	t--;
			if (t==0) break;
		}

	}
	return i;
}

//============== 'Clear' - ���⪠ ���� ����⨩ ���������� ===========
//��� �㭪�� ������� ��뢠�� ��᫥ "���� �����",
//��� ��� �������� �ய�᪨ ���᪠��� ������, � ��ꥪ� �� �६� ���������
//� ���� ��஭�
void KEYBOARD::Clear(void)
{
	for (int i=0; i<256; i++)
	{
		Keys[i]=0;
	}
}

//====================== ��������� ===================
KEYBOARD::KEYBOARD(void)
{
	#if DEBUGMESSAGES
		Debug("Run KEYBOARD constructor");
	#endif

	//��ꥪ� ⨯� ���������� ����� ᮧ���� ⮫쪮 ���� ࠧ
	if (CountObject!=0)
	{
		fatal("KEYBOARD-object already exist!");
		return;
	}

	//㢥��稢��� �᫮ ᮧ����� ��ꥪ⮢
	CountObject++;

	//��頥� ���� ����⨩ ������ ����������
	Clear();

	//�� ���७�� ������ ���� �� ��������...
	ExtendedKey=FALSE;

	//��࠭塞 ���� ����� ���뢠��� 0x09
	int09h_old = getvect( 0x09 );

	//��⠭�������� ���� ����� ���뢠��� ����������
	void interrupt ( KEYBOARD::*intnew)(...) = &KEYBOARD::int09h_new;
	asm {
		cli
		xor ax,ax
		mov es,ax
		mov bx,word ptr intnew
		mov es:[9*4],bx
		mov bx,word ptr intnew+2
		mov es:[9*4+2],bx
		sti
	}

	//��⠭�������� ०�� ����� - ��� ��ண� ��ࠡ��稪� ���뢠���
	InputMode = KEYBOARD_NOOLDINTERRUPT;

	//��࠭塞 ���� ᮧ������� ��ꥪ�.
	//�� ����������� � ��ࠡ��稪� ���뢠���,
	//⠪ ��� ��� �� ���������� 㪠��⥫� �� ��ꥪ� this
	This=this;
}

//================= '~KEYBOARD' �������� ��ꥪ� ====================
KEYBOARD::~KEYBOARD(void)
{
	#if DEBUGMESSAGES
		Debug("Run KEYBOARD - destructor");
	#endif

	//����⠭�������� ���� ����� ���뢠��� ����������
	setvect ( 0x09, int09h_old);
}

//=========== 'int09h_new' - ���� ��ࠡ��稪 ���뢠��� ���������� =======
//    ��᪮��� �� ��ࠡ��稪 ���뢠���,
//�������� � �����-童��� ����� ���� �� �����
//��⥣���᪨ ����饭�: this �ਭ����� ��砩�� ���祭��,
//䠪��᪨ this ��⠢����� �� ���� ॣ���஢ ������
//�����, ࠧ㬥����, �� ��।����� �� �室� � ��ࠡ��稪
//���뢠���.
//    ��ࠡ��稪� ����㯥� ᥣ���� ������, ���⮬� � �����-童���
//����� �������� ⮫쪮 ���।�⢮� 㪠��⥫� This [�� ��ꥪ�],
//����� ������ �࠭���� � ᥣ���� ������ �ணࠬ�� (� ����
//������ ���� ����� ��� static-����᪨� )
void interrupt KEYBOARD::int09h_new(...)
{
	//���塞 ��६�����, � ������ �����⨬ ��� ����⮩ ������
	unsigned char key;

	//���뢠�� �� ���� scan-��� ������
	asm in al,0x60
	asm mov byte ptr key,al

	//�᫨ �� ��뢠���� ���� ��ࠡ��稪 ���뢠���,
	//� ����室�� �⠭����� �⢥� ������� ����������
	//� ⮬, �� scan-��� �ਭ�� � ����� ॠ��஢���
	//�� ᫥���騥 ������ ������
	if ( This->InputMode == KEYBOARD_NOOLDINTERRUPT )
	{
		asm in al,0x61
		asm or al,0x82
		asm out 0x61,al
		asm and al,0x7f
		asm out 0x61,al
	}

	//�����뢠�� � ᮡ�⢥��� ���� 䠪� ������/���᪠��� ������

	//�᫨ ����� ������, ����� ���७�� SCAN-���
	if (key == 0xE0 )
	{
		This->ExtendedKey=0xE0;
	}
	else if ( key == 0xE1 )
	{
		This->ExtendedKey=0xE1;
	}
	//���� ����� ���筠� ������
	else
	{
		char t;

		if (key & 0x80)           //�᫨ ���᪠���� ������
		{
			t=0;
			key&=0x7f;
		}
		else	t=1;   		//���� ������ ����������

		if (This->ExtendedKey == 0xE1)
		{
			if ( key == 0x1d ) {
				key=KEY_PAUSE;
				ExtendedKey=FALSE;
				}
		}

		//�᫨ ���������� ���७��� ������
		//�஢���� ⮯����⥫��� ��ࠡ���
		if (This->ExtendedKey == 0xE0)
		{
			//�஢���� �������⥫�� ������ ����� ������
			switch (key) {
				case KEY_PAD0:	key=KEY_INSERT; break;
				case KEY_PAD1:	key=KEY_END;    break;
				case KEY_PAD2:	key=KEY_DOWN;   break;
				case KEY_PAD3:	key=KEY_PAGEDOWN;break;
				case KEY_PAD4:	key=KEY_LEFT;   break;
				case KEY_PAD6:	key=KEY_RIGHT;  break;
				case KEY_PAD7:	key=KEY_HOME;   break;
				case KEY_PAD8:	key=KEY_UP;     break;
				case KEY_PAD9:	key=KEY_PAGEUP; break;

				case KEY_ENTER: key=KEY_PADENTER;     break;
				case KEY_LEFTCTRL: key=KEY_RIGHTCTRL; break;
				case KEY_LEFTALT:  key=KEY_RIGHTALT;   break;
				case KEY_PADDEL:   key=KEY_DELETE;	break;
				case 0x35:	   key=KEY_PADDIV;	break;
//				case KEY_LEFTSHIFT:	key=KEY_PRINTSCREEN; break;

				default:	key=FALSE; break;
			} //����� switch
		}

			This->ExtendedKey=FALSE;

		if (key)
		{
                        if ( (t==1 && This->Keys[key]==0) || t==0)
				This->Keys[key]=t;
                }
	}

	//�᫨ ���� �ᯮ�짮���� �⠭����� ��ࠡ��稪
	//� ��뢠�� ���
	if ( This->InputMode & KEYBOARD_YESOLDINTERRUPT)
	{
		This->int09h_old();	//�� ���뢠�� ��������
						//� ����� �१ 㪠��⥫�
		return;
	}

	//�᫨ �⠭����� ��ࠡ��稪 �� ��뢠����
	//� ����室��� ������ ����஫���� ���뢠���
	//������� "����� ���뢠���".
	//  � ��⨢��� ��砥 �� ������ ���뢠��� >=IRQ 2
	// ����஫��� ���뢠��� ॠ��஢��� �� �㤥�: ��⥬� '����᭥�'
		asm mov al,0x20		//���뫠�� ����஫���� ���뢠��� �������
		asm out 0x20,al		// '����� ���뢠���'

	if ( This->Keys[ KEY_LEFTCTRL] && This->Keys[KEY_LEFTALT] &&
	 This->Keys[ KEY_DELETE] )
	{
		exit(1);
	}
}

void KEYBOARD::Set(char keycode,char what)
{
	Keys[keycode & 0x7f]=what;
}

//========== 'SetInputMode' - ��⠭���� ०��� ����� � ���������� ========
// 'mode' - ०��:
//    KEYBOARD_YESOLDINTERRUPT - ����砥�, �� �஬� ������ ��ࠡ��稪�
// ���뢠��� 0x09 �㤥� ��뢠���� ����, � ���� ������� �����������
// ��뢠�� �� �㭪樨 DOS & BIOS ��� ����������
//    KEYBOARD_NOOLDINTERRUPT - �㤥� ����⢮���� ⮫쪮 ���� ��ࠡ��稪
//  �㭪樨 DOS & BIOS ��� ���������� ��뢠�� �����
//  ������:
//	⥪�騩 ०��
int KEYBOARD::SetInputMode(int mode)
{
	int temp=InputMode;
	InputMode=mode;
	return temp;
}

void KEYBOARD::SuperInput()
{
	SetInputMode(KEYBOARD_NOOLDINTERRUPT);
}

void KEYBOARD::StandartInput()
{
	SetInputMode(KEYBOARD_YESOLDINTERRUPT);
}
#include "SCREEN.H"
#include "KEYBOARD.H"

extern SCREEN scr;
extern KEYBOARD Keyb;

void TetrisHelp()
{
		scr.color=15;
		scr.clear();
		char * help=
		"F1 ������\n"
		"F2 ᫥����� ��몠\n"
		"F5 ��䥪��\n"
		"F9 ���ଠ�� \n"
		"F10 ���\n"
		"F11 ⠡��� ४�म� \n"
		"F12 ������ ��ப�\n"
		"<PAD4> ᫥����� �����\n"
		"<PAD6> �।���� �����\n"
		"<PAD1> ��砩��� ����\n"
		"<PAGEUP><PAGEDOWN> ࠧ��� ����饩 ��ப�\n"
		"<LEFTCTRL> <+><->  �ਭ� �祩��\n"
		"<LEFTSHIFT> <+><-> ���� �祩��\n"
		"<LEFTCTRL> <1><2> �ਭ� �⠪���\n"
		"<LEFTCTRL> <3><4> ���� �⠪���\n"
		;

		scr.x=0;
		scr.y=0;
		scr<<help;

		scr.update();

		Keyb.ReadScan();
}
#include <fstream.h>
#include "settings.h"

SETTINGS::SETTINGS()
{

}

SETTINGS::~SETTINGS()
{
}

void SETTINGS::Save()
{
	ofstream ofl;

    size=sizeof(SETTINGS);
	//��⠥��� ������ 䠩� � ⠡��楩
	ofl.open("settings.dat", ios::binary);
	ofl.write((char*)this, sizeof(SETTINGS));
	ofl.close();
}

void SETTINGS::Load()
{
	size=sizeof(SETTINGS);
	ifstream ifl;

	//��⠥��� ������ 䠩� � ⠡��楩
	ifl.open("settings.dat", ios::binary);

	//�᫨ ���
	if (!ifl)
	{
		EnableMusic=0;   //ࠧ���� ����
		EnableSound=0;	//ࠧ���� ��㪨
		EnableAutoChangeMusic=1; //ࠧ���� ��⮬������ ᬥ�� ��모
		EnableFunctionalKeys=1; //ࠧ���� �㭪樮����� ������
		MinimumBrickCount=10;//�᫨ ����� �᫮ ������� �㡨��� � �� ����

		ShowRunLine=1; //ࠧ���� ������� ��ப�
		ShowTableRecord=1; //�����뢠�� ⠡���� ४�म�
		ShowRealTime=1;	//�����뢠�� ��� ॠ�쭮�� �६���
		BoxXSize=7; //ࠧ��� �祩�� ����
		BoxYSize=7; //ࠧ��� �祩�� ����

		GlassLen=10;	//�ਭ� �⠪���
		GlassHei=20;	//���� �⠪���

		BoxView=0;		//��� �㡨��
		GlassView=0;	//��� �⠪���
		EnableEffects=0;	//ࠧ���� ��䥪��
		BigSymScale=2;

		Volume=0.3;
		Stroy=1;	//ࠢ����୮-⥬���஢����
		BaseFreq=440.0;	//������� ����
		RunLX=0;
		Decrement=10;	//᪮���� ����堭�� ����

		Pause_Symbol=5; //��㧠 ����� ᨬ������
		Pause_Enter=40; //��㧠 ��᫥ ���� 13

		LastFilePosition=0;

		Save();
	}
	else
	{
		ifl.read((char*)this, sizeof(SETTINGS));

		ifl.close();
	}
}

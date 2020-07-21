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
	//пытаемся открыть файл с таблицей
	ofl.open("settings.dat", ios::binary);
	ofl.write((char*)this, sizeof(SETTINGS));
	ofl.close();
}

void SETTINGS::Load()
{
	size=sizeof(SETTINGS);
	ifstream ifl;

	//пытаемся открыть файл с таблицей
	ifl.open("settings.dat", ios::binary);

	//если нет
	if (!ifl)
	{
		EnableMusic=0;   //разрешить музыку
		EnableSound=0;	//разрешить звуки
		EnableAutoChangeMusic=1; //разрешить автоматическую смены музыки
		EnableFunctionalKeys=1; //разрешить функциональные клавиши
		MinimumBrickCount=10;//если меньше число падающих кубиков то все вниз

		ShowRunLine=1; //разрешить бегущую строку
		ShowTableRecord=1; //показывать таблицу рекордов
		ShowRealTime=1;	//показывать часы реального времени
		BoxXSize=7; //размер ячейки тетриса
		BoxYSize=7; //размер ячейки тетриса

		GlassLen=10;	//ширина стакана
		GlassHei=20;	//высота стакана

		BoxView=0;		//вид кубика
		GlassView=0;	//вид стакана
		EnableEffects=0;	//разрешить эффекты
		BigSymScale=2;

		Volume=0.3;
		Stroy=1;	//равномерно-темперированный
		BaseFreq=440.0;	//базовая частота
		RunLX=0;
		Decrement=10;	//скорость затухания ноты

		Pause_Symbol=5; //пауза между символами
		Pause_Enter=40; //пауза после кода 13

		Save();
	}
	else
	{
		ifl.read((char*)this, sizeof(SETTINGS));

		ifl.close();
	}
}

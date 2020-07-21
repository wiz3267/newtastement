#ifndef __SETTINGS__
#define __SETTINGS__

//структура хранит настройки для всей игры на диске
struct SETTINGS
{
	int size;		//размер структуры
	char EnableMusic;   //разрешить музыку
	char EnableSound;	//разрешить звуки
	char EnableAutoChangeMusic; //разрешить автоматическую смены музыки
	char EnableFunctionalKeys; //разрешить функциональные клавиши
	char MusicVolume;	//громкость музыки (0..100)
	char MinimumBrickCount;	//минимум разлетающихся кубиков когда в стакане все идет вниз
	char ShowTableRecord;	//показывать таблицу рекордов во время игры
	char ShowRunLine; //разрешить бегущую строку
	char ShowRealTime; //показывать часы реального времени
	char BoxXSize;	//размер ячейки тетриса
	char BoxYSize;	//размер ячейки тетриса
	char GlassLen;	//ширина "стакана"
	char GlassHei;	//высота "стакана"
	char BoxView;	//вид кубика тетриса (сплошная заливка и др.)
	char GlassView;	//вид стакана есть или нет сетки например или фон
	char EnableEffects; //разрешить эффекты
	char BigSymScale; //размер больших букв
	double Volume;	//громкость
	double BaseFreq;	//базовая частота нот
	int Stroy;		//строй (равномерно темперированный или натуральный)
	unsigned long RunLX;	//место писания
	int StepRunLX;		//скорость текста
	float Decrement;	//скорость уменьшения громкости ноты

	int Pause_Symbol;	//пауза между символами - скорость текста
	int Pause_Enter;	//пауза после кода 13


//////////////
	void Load();	//загрузить с диска
	void Save();	//сохранить на диск
	SETTINGS();
	~SETTINGS();

};

#endif
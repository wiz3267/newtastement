//**************************************************************
//** ���ᠭ�� ����� 'SOUND' - �ந��뢠��� � ��ࠡ�⪠ ��㪠 **
//**************************************************************
struct SNDSTRUCT
{
	unsigned char far * sound;	//���쭨� 㪠��⥫� �� ��㪮�� �����
	unsigned int sndlen;		//����� ��㪮��� ��᫥����⥫쭮��
	unsigned int far * IsPlaying;	//���� 䫠�� ���ﭨ�
	unsigned int frequency;		//���� ���ந��������
	int flag;			//��� �ந���� ����
};

class SOUND
{
#define count_max 25	//�� �᫮ ����� ������: ���ᨬ��쭮� �᫮
			//�������
#define NOSTOP	1	//��� �� ������ ���뢠����
#define YESSTOP 0	//��� ����� ��ࢠ���� ᫥���騩 ��㪮�
#define YESPLAYING 1
#define NOPLAYING 0
private:
static int CountObject;
	int current;		//⥪��� �������
	int count;		//�᫮ ��㪮�
	int load_status;	//����� ����㧪�
	char DIGPAK;		//䫠� ����㧪� DIGPAK
	char MIDPAK;		//䫠� ����㧪� MIDPAK
	char INITDIGPAK;	//䫠� ���樠����樨 DIGPAK
	char INITMIDPAK;	//䫠� ���樠����樨 MIDPAK
	char DIGPAKAVAILABLE;	//䫠� ����㯭��� DIGPAK �� ��� MIDPAK
	unsigned int isplay;	//䫠� ���ﭨ�
public:
	SNDSTRUCT sounds[count_max]; //���ᨢ 㪠��⥫��,�� ���ଠ�� � ��㪠�
	SOUND(void);			//���������
	~SOUND(void);			//��������
	int  load(char far * fname,int=11025);	//�ਡ����� �������
//	void set(int n, SNDSTRUCT far & sndplay); //��⠭����� ᢮��⢠
	void play(int n,int flag=0);	//�ந���� ������� � 䫠���
	void SndPlay(SNDSTRUCT far & sndplay);	//�ந���� ���
	int  soundStatus(void);		//���ﭨ� �ࠩ���
	void stop(void);		//��⠭����� �ந��뢠���
	void unload(int n);		//㤠���� �������� ������� � ����஬ n
};

#define MAX_MUSIC_NUMBER 6
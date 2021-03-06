#ifndef __SPRITE_H
#define __SPRITE_H

//**************************************************************
//***************** ���ᠭ�� ����� SPRITE *********************
//**************************************************************

// ��楤��� �뢮�� �ࠩ⮢ ���ࠧ㬥���� ०�� ��࠭� 320*200, 256 color

class SCREEN;	//�।���⥫쭮� 'taltentive' ������� �����


struct FILEHEADER
{
	char Signature[10];
	long Info;		//ᬥ饭�� �� �������� SPRITEINFO �� ��砫� 䠩��
        long Sprite;		//ᬥ饭�� �� �ࠩ� �� ��砫� 䠩��
        long FileSize;		//ࠧ��� 䠩�� (��� �஢�ન 楫��⭮��)
};

//��������� ⠪�� �࣠����樨, ����� ���騢���, �᫨ �� ����室���
//��� ��������, �᫨ �� �ᯮ������ � ���� 䠩��,
//⮣�� �� ���뢠��� �������騥 �������� ���� �� ���� ������

//������� �ଠ� ��ࠧ�
struct SPRITEINFO
{
	int Len, Hei;	//ࠧ���� �ࠩ�
	int Attribute;	//��ਡ��� �ࠩ�
};


class SPRITE
{
public:

	//=========== ������� 'IMAGE' - ��ࠧ �ࠩ�=================
	struct IMAGE
	{
		unsigned Len;		//�����
		unsigned Hei;		//�ਭ�
                //�᫨ Datacolor==0,//�ࠩ� ��⠥��� ᢮�����
		char far *Datacolor;	//㪠��⥫� ������ � 梥� �祪 ᠬ��� �ࠩ�
		int	Attribute;	//᢮��⢠ �ࠩ�
		int Index;		//�����䨪��� �ࠩ�
                void Write(int x,int y,unsigned char color);	//������ �窨 � �ࠩ�
                unsigned char  Read(int x,int y);	//�⥭�� �窨 � �ࠩ�
	};


	//���ᨬ��쭮� �᫮ �ࠩ⮢, ���஥ ����� ᮧ���� ��� ������ ��쥪�
	//����� SPRITE
	int SPRITES_MAX;

	//��ਡ��� �ࠩ�
	enum {
	TRANSPARENT=0x04	//�ࠩ� � �஧��묨 ���ᥫ���
	};


private:
static	int	CountObject;	//�᫮ ��ꥪ⮢

	IMAGE	*Sprites;	//���� ���ᨢ� ������� ⨯� IMAGE
	char	*DoubleBuffer;	//���� ⥭����� ��࠭�
public:
	int	Counter;	//���稪 �ࠩ⮢
	SCREEN	*scr;		//㪠��⥫� �� ��ꥪ� ⨯� SCREEN

	//���������� ����室���  ��।��� ���� ⥭����� ��࠭�
	SPRITE(SCREEN far *, int spMax=100);

	//�������� - 㭨�⮦��� �� ᮧ����� �ࠩ�� � ᠬ ��쥪�
	//����� SPRITE
	~SPRITE(void);

	//㤠���� �ࠩ� � �����ᮬ n
	int Delete(int n);

	int find(int Index);	//���� �ࠩ� � �����ᮬ Index

        int find_free();	//�����頥� ����� ��ࢮ�� ���⮣� �ࠩ�

	//����㧨�� �ࠩ� � ��᪠ ��� ���ᨢ 梥��� �祪
	int Load(char * fname, unsigned long offset, IMAGE );

        //����㧨�� �ࠩ� � ��᪠(䠩� ����� ��।������ �ଠ�)
	int LoadSpriteform(char * fname, IMAGE & im);

	//������� 梥� ������ �窨 � �ࠩ� i �� ����稭� n
	void ModifyColor(int i, char n);

        //ᮧ������ ���� �ࠩ�. ����ঠ��� �ࠩ� �� ��।�����
        int Create(IMAGE&);

	//�᫨ �ࠩ� ����� ᢮��⢠ �� 㬮�砭��, ����� �ᯮ�짮����
	//⠪�� ᯮᮡ ����㧪�
//	void Load(char far * fname, int len, int hei, int Index);

	//����㧨�� �ࠩ� � ��᪠, � � ���쭥�襬 �������� � ����
	// �� �����饭���� ������
//	int  Load(char far * fname);

//	//��⠭���� ��ࠧ� �ࠩ�
//	void SetImage(IMAGE Image, int Index);

	//����祭�� ��ࠧ� �ࠩ�
	IMAGE far & GetImage(int Index);

//	//��⠭����� ��ਡ��� �ࠩ� � �����ᮬ Index
//	void SetAttribute(int Attribut, int Index);

	//ᤢ�� �ࠩ� Index, � ��஭� F, �� Num �祪
        enum {
		SCROLL_UP=0x01,
		SCROLL_DOWN=0x02,
		SCROLL_LEFT=0x03,
		SCROLL_RIGHT=0x04
        };

	int Scroll(int Index, int Flags, int Num=1);

	//�뢥�� �� ��࠭ �ࠩ� � �����ᮬ Index
	int Show(int x,int y, int Index,int=0);

	//�뢥�� �� ��࠭ �ࠩ� � 㪠������ ����� � ���ᨢ�
        int ShowDirect(int x,int y,int Index,int=0);

};

#endif
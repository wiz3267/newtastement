#ifndef __RECTABLE_H
#define __RECTABLE_H

//����� RECTABLE - ⠡��� १���⮢
class RECTABLE
{
public:

	enum {
		MAX_RECORD=20,		//���ᨬ��쭮� �᫮ ४�म�
		NAME_MAXLEN=14		//���ᨬ��쭠� ����� �����
	};


//�࠭�� ���ଠ�� � ४��ᬥ���
struct RECORD
{
	char name[ NAME_MAXLEN] ;		//���
	char color;
	unsigned long score;	        //�᫮ �窮�
	unsigned long lines;			//�᫮ �����
	unsigned int time	;			//�६� ����
};

RECORD record[MAX_RECORD+1];		//㪠��⥫� �� ⠡���� ४�म�

public:
	RECTABLE();	//���������
	~RECTABLE();	//��������
	void Save();		//������� ⠡���� �� ���
	void Load();		//����� ⠡���� � ��᪠

	//����⪠ �������� ���� ४�� � ⠡����
	//void PutNewRecord(unsigned long record, unsigned long lines);
	char * PutNewRecord(char * _name, unsigned long newscore, unsigned long lines, unsigned int _time);
	void PutNewRecordNameColor(char *name, unsigned long newscore, char Color);
	void Show(int newnum=-1);		//�������� ⠡���� �� ��࠭�

	//������� ���ᨬ���� ४�� �� ⠡����
	unsigned long GetMaxScore();
};

#endif
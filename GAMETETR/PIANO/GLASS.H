#ifndef __GLASS__H
#define __GLASS__H

typedef unsigned BOX;
//����� GLASS - �⠪�� ����
class GLASS
{
public:

	unsigned

		Len,Hei;	//�ਭ� � ���� �⠪��� (� �㡨���)

	BOX * Data;		//㪠��⥫� �� �����(�⠪��)

	int
		x,y;		//���न���� �⠪��� �� ��࠭�(� ���ᥫ��)

	int BoxLen, BoxHei;	//ࠧ���� ������ �㡨��

		GLASS();				//���������
		~GLASS();				//��������

//-----------------------------------------------------------------
	int		Save(char *fname);					//��࠭��� �� ��᪥
	int		Load(char *fname);					//����㧨�� � ��᪠
	void	Clear();                         	//���⪠ �⠪���
	void	Make(unsigned len, unsigned hei);	//ᤥ���� �⠪��
	BOX		Read(int j, int i);					//����� �㡨�
	void	Show();								//�������� �⠪��
	void	ShowBackground(int i);				//�������� ������ ����
	void	ShowBorder();						//�������� ࠬ��
	void	Write(int j, int i, BOX bx);		//������� �㡨�
};

#endif
#ifndef __FIGURE_H_
#define __FIGURE_H_

#include "glass.h"

//����� FIGURE  - �� ����� 䨣�� ����
//��ꥪ� �������筮 �易� � ����� �� �⠪����
class FIGURE
{
public:

	GLASS *Glass;
	//�᫮ 䨣��
	enum { Counter=7 };

	float
		x,y;		//���न���� 䨣���

	int
		Color,		//梥� 䨣���
		Number,		//����� 䨣���
		Aspect		//��� 䨣��� (0..3)
	;
	void	Show();		//���ᮢ��� 䨣��� � �⠪���
	void	Show(int x,int y); //���ᮢ��� 䨣��� �� ��࠭�
	void	Show(int color);
	int	Check();	//�஢�ઠ, ����� �� �ᮢ��� 䨣���

	FIGURE();	//���������
};

#endif
////////////////////////////////////////
// Copyright (C) ���設 �������, 1997 //
////////////////////////////////////////


#include "rects.h"
#include "debug.h"

#define TRUE  1
#define FALSE 0

//���������� ��� ��६����
void swap(int& a,int& b)
{
	int c=a;
	a=b;
	b=c;
}

//�蠥� ������ ��宦����� ����祭�� ������� ����ࠫ��� �ᥫ
//[a,b] � [c,d]
//������: 0 - ��� �襭��
//�᫮ 1...32767 - ����� ��饣� ��१��
//������:
//TRUE - �᫨ ���� ���� ��� ��-�� �襭��, � ��६���� x1, x2 ��������� �襭��
//FALSE - �᫨ ��� �襭��, ⮣�� ��६���� x1 � x2 ������� ��� ���������

int DetermineSystem(int a,int b,int c, int d, int &x1, int &x2)
{
        //��ࠡ��뢠�� �������⨬�� �����
        if (a>b)
        {
        	fatal("DetermineSystem - a>b !");
        }

        if (c>d)
        {
        	fatal("DetermineSystem - c>d !");
        }


        // ��砩 (1)
	if ( b<c || d<a ) return FALSE;

        //������ ��१�� [a,b] ����訬 ��� ࠢ�� ��१�� [c,d]
        if ( (b-a) < (d-c) )
        {
        	swap(a,c);
                swap(b,d);
        }

        // �� �᫮��� (1) ᫥���, �� c<=b && a<=d
        // C��砩 (2) ��१�� [c,d] ����� [a,b]
        if ( a<=c && d<=b) { x1=c; x2=d; return x2-x1+1; }

        // �� (1) � (2) ᫥���, �� c<=b � a<=d
        // � ���� c<a ���� b<d � �� ⮣�, �� ��१�� [c,d] �� ����� [a,b]
        // ������ ⠪�� �뢮�:
        // �᫨ c<a
        if (c<a) { x1=a; x2=d; return x2-x1+1; }
        // ���� b<d
        else { x1=c; x2=b; return x2-x1+1; }

}


//��蠥� ������ ����祭�� ��-� [a,b] � [c,d]
//�����頥� ����� ��饣� ��१��, ��� 0, �᫨ ��� �襭��
int DetermineSystem(int a,int b,int c, int d)
{

        int x1,x2;
        //��ࠡ��뢠�� �������⨬�� �����
        if (a>b)
        {
        	fatal("DetermineSystem - a>b !");
        }

        if (c>d)
        {
        	fatal("DetermineSystem - c>d !");
        }


        // ��砩 (1)
	if ( b<c || d<a ) return FALSE;

        //������ ��१�� [a,b] ����訬 ��� ࠢ�� ��१�� [c,d]
        if ( (b-a) < (d-c) )
        {
        	swap(a,c);
                swap(b,d);
        }

        // �� �᫮��� (1) ᫥���, �� c<=b && a<=d
        // C��砩 (2) ��१�� [c,d] ����� [a,b]
        if ( a<=c && d<=b) { x1=c; x2=d; return x2-x1+1; }

        // �� (1) � (2) ᫥���, �� c<=b � a<=d
        // � ���� c<a ���� b<d � �� ⮣�, �� ��१�� [c,d] �� ����� [a,b]
        // ������ ⠪�� �뢮�:
        // �᫨ c<a
        if (c<a) { x1=a; x2=d; return x2-x1+1; }
        // ���� b<d
        else { x1=c; x2=b; return x2-x1+1; }

}


//��室�� ��騩 ��אַ㣮�쭨�
int DetermineSystem(RECT &a, RECT &b, RECT &c)
{
	if (
	DetermineSystem(a.Left, a.Right, b.Left, b.Right, c.Left, c.Right) &&
	DetermineSystem(a.Top, a.Bottom, b.Top, b.Bottom, c.Top, c.Bottom)
           )

        return 1;

        else return 0;
}

//�஢����, ���ᥪ���� �� ��אַ㣮�쭨��
//������ FALSE - �� ���ᥪ����
// BIG_HORZ - ����� ��ਧ��⠫쭠� ����
// BIG_VERT - ����� ���⨪��쭠� ����
int	CheckUnionRects(RECT far & rect1, RECT far & rect2)
{
	int x1,y1,x2,y2,x3,y3,x4,y4;
	x1=rect1.Left;
	y1=rect1.Top;
	x2=rect1.Right;
	y2=rect1.Bottom;

	x3=rect2.Left;
	y3=rect2.Top;
	x4=rect2.Right;
	y4=rect2.Bottom;

	//�����⮢�� � �믮������ ᫥���騩 �᫮���:
	// x1<x2,y1<y2
	// x3<x4,y3<y4
	if ( x2<x1 )	swap(x2,x1);
	if ( x4<x3 )	swap(x4,x3);
	if ( y2<y1 )	swap(y2,y1);
	if ( y4<y3 )	swap(y4,y3);
	//�⮡� �믮��﫮��: x2-x1>=x4-x3
	//		     y2-y1>=y4-y3
	if ( x2-x1 < x4-x3 )
	{
		swap(x2,x4);
		swap(x1,x3);
	}

	if ( y2-y1 < y4-y3 )
	{
		swap(y2,y4);
		swap(y1,y3);
	}

	int horz,vert;
	if ( (horz=DetermineSystem(x1,x2,x3,x4)) == 0) return FALSE;
	if ( (vert=DetermineSystem(y1,y2,y3,y4)) == 0) return FALSE;
	if ( horz>vert ) return BIG_HORZ;
	if ( vert>horz ) return BIG_VERT;
	return BIG_HORZ | BIG_VERT; //ࠢ��
}

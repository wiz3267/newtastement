#include "glass.h"

#include "sprite.h"
#include "debug.h"

#include "screen.h"
#include "settings.h"
#include <fstream.h>

extern SPRITE SpriteBox;
extern SCREEN scr;

//************************************************************************
//************************ класс GLASS - стакан **************************
//*************************************************************************

GLASS::GLASS() : x(0), y(0), Len(0), Hei(0), Data(0), BoxLen(8), BoxHei(8)
{
}

//показать задний план
void GLASS::ShowBackground(int)
{
	for(int i=0; i<Hei*8; i+=20)
	for(int j=0; j<Len*8; j+=20)
	{
		//Background.Show( x+8+j, y+i, id);
	}
}

//дестркуктор
GLASS::~GLASS()
{
	//если память под стакан была выделена, удаляем ее
	if (Data) delete Data;
}

//очистка стакана
void GLASS::Clear()
{
	for(int i=0; i<Hei; i++)
	for(int j=0; j<Len; j++)
	Write(j, i, 0);
}

//создание стакана
//len - длина
//hei - высота
void GLASS::Make(unsigned len, unsigned hei)
{
	BOX *dat = new BOX[len*hei];
	if (!dat)
	{
		Debug("GLASS::Make out of memory");
	}
	else
	{
		//если память была уже ранее выделена, освобождаем ее

		if (Data) delete Data;

		Len=len;
		Hei=hei;
		Data=dat;
		Clear();	//очищаем стакан
	}

}

//прочитать код
BOX GLASS::Read(int j, int i)
{
	if ( j<0 || j>=Len || i>=Hei )
	{
		//Debug("GLASS::Read bad j,i");
		return -1;
	}
	else if ( i<0) return 0;
	{
		return Data[i*Len+j];
	}

}


extern SETTINGS Settings;

void	DrawGlassBox(int x1,int y1, int _L, int _H, char t)
{
	int View=Settings.BoxView;

//			scr.line(x1,y1, x2,y2,t);
//			scr.line(x2,y2, x3,y3,t);
//			scr.line(x3,y3, x4,y4,t);
//			scr.line(x4,y4, x1,y1,t);

//			scr.line(x1,y1, x5,y5, t);
//			scr.line(x5,y5, x6,y6, t);
//			scr.line(x2,y2, x6,y6, t);

//			scr.line(x6,y6, x7,y7, t);
//			scr.line(x3,y3, x7,y7, t);

			if (View==0)
			{

			scr.rectangle(x1, y1, _L, _H, t);

			int P=_L/2;
			for(int k=1; k<P; k++)
			{
				scr.line_right(x1+k,y1-k, _L, t);
			}
			for(k=1; k<P; k++)
			{
				scr.line_down(x1+_L+k-1,y1-k, _H, t);
			}
			}
			//scr.rectangle(x1 + 1, y1+1, BoxLen-k-2, BoxHei-k-2,0);
			else if (View==1)
			{
				scr.rectangle(x1,y1,_L,_H,t);
			}
			else if (View==2)
			{
				scr.rectangle(x1,y1,_L-1,_H-1,t);
			}
			else if (View==3)
			{
				scr.border(x1,y1,_L-1,_H-1,t);
			}
			else if (View==4)
			{
				scr.line(x1,y1, x1+_L, y1+_H, t);
				scr.line(x1+_L,y1, x1, y1+_H, t);
			}
			else
			{
				int x=x1+_L/2;
				int y=y1+_H/2;
				scr.line(x,y1, x1+_L, y, t);
				scr.line(x1+_L,y, x, y1+_H, t);
				scr.line(x,y1+_H, x1, y, t);
				scr.line(x1,y, x, y1, t);

			}

}

//показать стакан на экране
void  GLASS::Show()
{
	for(int i=0; i<Hei; i++)
	for(int j=0; j<Len; j++)
	{
		int color=Read(j,i);
		if (color>0)
		{
			int x1=x+8+j*BoxLen;
			int y1=y+i*BoxHei;

			DrawGlassBox(x1,y1,BoxLen,BoxHei,color);
		}

	}
	//показываем рамку вокруг стакана
	ShowBorder();

}

void graph_draw_num(int x, int y, unsigned long num);

void GLASS::ShowBorder()
{
	//int c=SPRITE_BORDER;

	/*for(int i=0; i<Hei; i++)
	 SpriteBox.Show(x, y+i*8, c);

	for(i=0; i<Hei; i++)
	 SpriteBox.Show(x+Len*8+8, y+i*8, c);

	for(i=0; i<Len+2; i++)
	 SpriteBox.Show(x+i*8, y+Hei*8, c);
	*/

	scr.border(x+8-1,y, Len*BoxLen+2, Hei*BoxHei+1, 88);


	int x1=x+8-1, y1=y+Hei*BoxHei+1;

	graph_draw_num(x1, y1, Len);
	graph_draw_num(x1+4*3, y1, Hei);

	//scr.rectangle(x+8-1,y,Len*8+2,Hei*8+1,9);

}

//записать код
void  GLASS::Write(int j, int i, BOX bx)
{
	if ( j<0 || i<0 || j>=Len || i>=Hei )
	{
		//Debug("GLASS::Write bad j,i");
		return;
	}
	else
	{
		Data[i*Len+j] = bx;
	}

}

//загрузить из файла
int	GLASS::Load(char *fname)
{
	ifstream ifl;

	//пытаемся открыть файл
	ifl.open(fname, ios::binary);

	//если нет
	if (!ifl)
	{
		return 0;
	}
	else
	{
		delete Data;

		ifl.read((char*)this, sizeof(GLASS));

		Data=new BOX[Len*Hei];

		ifl.read((char*)Data, sizeof(BOX)*Len*Hei);

		ifl.close();
	}

    return 1;
}

//сохранить в файл
int	GLASS::Save(char *fname)
{
	ofstream ofl;

	//пытаемся создать файл
	ofl.open(fname, ios::binary);
	if (!ofl) return 0;

	ofl.write((char*)this, sizeof(GLASS));
	ofl.write((char*)Data, sizeof(BOX)*Len*Hei);
	ofl.close();

	return 1;
}

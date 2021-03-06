#include "debug.h"
#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "settings.h"


double PI=3.1415926;
void GetRealTimeClock(unsigned char &hour, unsigned char &min, unsigned char &sec);
static int counter=0;
extern int BigSymScale;
void		DrawClock();

void DrawClock2()
{

		unsigned char hour,min,sec;

		GetRealTimeClock(hour, min, sec);

		char txt[12]={0};
		sprintf(txt,"%02i:%02i.%02i", int(hour), int(min), int(sec));

		scr.x=20;
		scr.y=135;
		scr.color=9;

		int old=BigSymScale;

		BigSymScale=6;
		for(int v=0;v<strlen(txt);v++)
		scr.putBigSym(txt[v], 9,11);

		BigSymScale=old;

		scr.color=15;



}

void DrawStrelka(int xc, int yc, int rad, int r,int g, int b,
int base_palette, int CLRMAX, double strelka_position)
{
	double min=strelka_position;

	for(int Z=0;Z<CLRMAX;Z++)
	{

		int color=63.0/CLRMAX*(CLRMAX-Z);
		scr.SetColorRegister(base_palette-Z, color*r,color*g,color*b);
	}

	int clrbig=base_palette;

	double K_2PI_div_60=2*PI/60.0;

	scr.line(xc,yc, xc + rad*sin(PI + -K_2PI_div_60*min), yc + rad*cos(PI + -K_2PI_div_60*min), clrbig);
	//��᭭! �ᨒ ���� ���������� ������� ����������� �����������
	for(int P=0; P<CLRMAX;P++)
	{
	//����������!
		double delta=2*PI/60/14.0*P;
		double Cof=P*2;
		double R=rad-Cof;
		scr.line(xc,yc, xc + R*sin(PI + -K_2PI_div_60*min + delta), yc + R*cos(PI + -K_2PI_div_60*min+delta), clrbig-P);
		scr.line(xc,yc, xc + R*sin(PI + -K_2PI_div_60*min - delta), yc + R*cos(PI + -K_2PI_div_60*min-delta), clrbig-P);
	}
}

void DrawClock()
{
		DrawClock2();

		double R=200/2;

		int xc=320/2, yc=200/2-40;

		static double angle=0;

		int OldBigSymScale=BigSymScale;

		static sec_start=0;

		unsigned char hour,min,sec;

		GetRealTimeClock(hour, min, sec);
		sec_start=sec;

		static double Speed=1;
		static double SpeedAdd=0.01;

		for(double a=0; a>-2*PI*12000; a-=2*PI/70*Speed)
		{

			int digit=-1;

			int _clr_=0;

			static double spd=0.0001;
			static double speed_bukve=0.0001;
			//static double speed_bukve_add=0.00001;


			for(double b=0; b>=-2*PI; b-=2*PI/12)
			{
				digit++;

				//double R=64*sin(angle);

				if (Keyb[KEY_SPACE])
				{
					static int old_angle;

					static int sav;
					angle=old_angle;
					old_angle=sav;
				}

				angle+=0.0001;
				angle+=0.0000001;
				angle=spd;

				spd+=speed_bukve;

				/*if (Keyb[KEY_A])
				{
					Keyb.Set(KEY_A,0);
					speed_bukve+=speed_bukve_add;
				}

				if (Keyb[KEY_S])
				{
					Keyb.Set(KEY_S,0);
					speed_bukve-=speed_bukve_add;
				}
				*/

				R=64;
				angle=1;
				scr.x=xc + R*sin(b*angle + PI);//+spd);
				scr.y=yc + R*cos(b*angle + PI);//+spd);

				int dig=digit;
				if (digit==0) dig=12;

				//scr<<dig;

				char d[16];
				sprintf(d, "%i", dig);

				OldBigSymScale=BigSymScale;

				static int BigSymSc=1;

				BigSymScale=BigSymSc;

				scr.color=_clr_;

				_clr_++;

				for(int i=0; i<strlen(d); i++)
				scr.putBigSym(d[i], _clr_,7);

				BigSymScale=OldBigSymScale;
			}

			unsigned char hour,min,sec;

			static old_sec=0;

			GetRealTimeClock(hour, min, sec);

			if (old_sec != sec) {old_sec=sec;counter=0;}

			char txt[12]={0};
			sprintf(txt,"%02i:%02i:%02i", int(hour), int(min),int(sec));

			scr.x=40;
			scr.y=0;
			scr.color=1;

			char clr=15;

			if (Keyb[KEY_C]) Speed+=SpeedAdd;
			if (Keyb[KEY_V]) Speed-=SpeedAdd;

			a=-Speed*2*PI*counter/70.0;
			//scr.line(xc,yc, xc + R*sin(a+PI), yc + R*cos(a+PI), clr);
			DrawStrelka(xc, yc, R, 1,1,0, 255-8, 8, counter/70.0*60);

			int R_sec=60;

			double sec_plavno=sec+2*PI/12.0*(counter/70.0);
			DrawStrelka(xc, yc, R_sec, 0,1,0, 255-16, 8, sec_plavno);

			int R_min=70;
			DrawStrelka(xc, yc, R_min, 1,0,0, 255, 8, min);

			//��᭭! �ᨒ ���� ���������� ������� ����������� �����������

			int R_hour=40;

			if (hour>12) hour-=12;

			DrawStrelka(xc, yc, R_hour, 0,0,1, 255-32, 8, hour*5);
//			scr.line(xc,yc, xc + R_hour*sin(PI + -2*PI*hour/12), yc + R_hour*cos(PI + -2*PI*hour/12), clr-3);

			counter++;

			if (counter>=69) counter=69;

			BigSymScale=OldBigSymScale;
			return;
 		}

}

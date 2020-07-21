#include "debug.h"
#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "settings.h"


void GetRealTimeClock(unsigned char &hour, unsigned char &min, unsigned char &sec);
static int counter=0;
extern int BigSymScale;

void DrawClock2()
{
		unsigned char hour,min,sec;

		GetRealTimeClock(hour, min, sec);

		char txt[12]={0};
		sprintf(txt,"%02i:%02i", int(hour), int(min));

		scr.x=60;
		scr.y=30;
		scr.color=1;

		int old=BigSymScale;

		BigSymScale=6;
		for(int v=0;v<strlen(txt);v++)
		scr.putBigSym(txt[v], 15,11);

		BigSymScale=old;

		scr.color=15;

}

void DrawClock()
{
		double PI=3.1415926;

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
			static double speed_bukve=0.00009;
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

				int tt_xx=3.3;

				//angle+=0.00001;
				//angle+=0.0000001;
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
				scr.x=xc + R*sin(b*angle + PI);//+spd);
				scr.y=yc + R*cos(b*angle + PI);//+spd);

				int dig=digit;
				if (digit==0) dig=12;

				//scr<<dig;

				char d[16];
				sprintf(d, "%i", dig);

				OldBigSymScale=BigSymScale;

				static int BigSymSc=1;

				/*if (Keyb[KEY_PADADD])
				{
					BigSymSc++;
					Keyb.Set(KEY_PADADD,0);
				}

				if (Keyb[KEY_PADSUB])
				{
					BigSymSc--;
					if (BigSymSc<1) BigSymSc=1;
					Keyb.Set(KEY_PADSUB,0);
				}*/


				BigSymScale=BigSymSc;

				scr.color=_clr_;

				_clr_++;

				for(int i=0; i<strlen(d); i++)
				scr.putBigSym(d[i], _clr_,7);

				BigSymScale=OldBigSymScale;
			}


			//static char rd=0,gr=0,bl=0;
			//int rd1=rd, rd2=gr, rd3=bl;

			//for(int g=0; g<12; g++)
			//{
			//if (g&1) rd2=0;
			//else if (g&2) rd3=0;
			//else if (g&4) rd1=0;

			{
				//for(int i=1; i<=12; i++)
				//{
				//	int r=i,g=i,b=i;
					//scr.SetColorRegister(i, r,g,b);
				//}
			}

			//rd++;
			//gr++;
			//bl++;

			unsigned char hour,min,sec;
			static old_sec=0;
			GetRealTimeClock(hour, min, sec);
			if (old_sec != sec) {old_sec=sec;counter=0;}

			char txt[12]={0};
			sprintf(txt,"%02i:%02i:%02i", int(hour), int(min),int(sec));

			scr.x=40;
			scr.y=0;
			scr.color=1;

			//for(int v=0;v<strlen(txt);v++)
			//scr.putBigSym(txt[v], 15,13);

			char clr=15;

			if (Keyb[KEY_C]) Speed+=SpeedAdd;
			if (Keyb[KEY_V]) Speed-=SpeedAdd;


			a=-Speed*2*PI*counter/70.0;
			scr.line(xc,yc, xc + R*sin(a+PI), yc + R*cos(a+PI), clr);

			int R_sec=60;

			scr.line(xc,yc,
			xc + R_sec*sin(PI + -2*PI*sec/60),// + -2*PI/60 * (counter)/70),
			yc + R_sec*cos(PI + -2*PI*sec/60),// + -2*PI/60 * (counter)/70),
			 clr-1);

			int R_min=70;

			double delta=2*PI/60/10;

			scr.line(xc,yc, xc + R_min*sin(PI + -2*PI*min/60), yc + R_min*cos(PI + -2*PI*min/60), clr-5);
			scr.line(xc,yc, xc + R_min*sin(PI + -2*PI*min/60 + delta), yc + R_min*cos(PI + -2*PI*min/60+delta), clr-5);
			scr.line(xc,yc, xc + R_min*sin(PI + -2*PI*min/60 - delta), yc + R_min*cos(PI + -2*PI*min/60-delta), clr-5);

			int R_hour=40;

			if (hour>12) hour-=12;

			scr.line(xc,yc, xc + R_hour*sin(PI + -2*PI*hour/12), yc + R_hour*cos(PI + -2*PI*hour/12), clr-3);

			counter++;

			if (counter>=70) counter=0;

			BigSymScale=OldBigSymScale;
			return;
 		}

}

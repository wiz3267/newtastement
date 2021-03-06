//---------------------------------------
// Copyright (C) ���設 E.O., 1997
//---------------------------------------


//*******************************************************************
//***************** �㭪樨-童�� ����� 'SOUND' ********************
//*******************************************************************
#include <process.h>
#include <stdlib.h>
#include <stdio.h>

//#include <iostream.h>
//#include <conio.h>
//#include <alloc.h>

#include "debug.h"
#include "sound.h"
#include "midpak2.h"
#include "doscall2.h"
#include "loader2.h"
#include "settings.h"
#include <math.h>
#include "digplay.h"
#include "screen.h"

extern "C" short cdecl far  DigPlay(SNDSTRUC far *sndplay); // 688h -> Play 8 bit digitized sound.
extern "C" short	cdecl far SoundStatus(void);	// 689h -> Report sound driver status.
extern "C" void cdecl far MassageAudio(SNDSTRUC far *sndplay); // 68Ah -> Preformat 8 bit digitized sound.
extern "C" void cdecl far DigPlay2(SNDSTRUC far *sndplay); // 68Bh -> Play preformatted data.
extern "C" short	cdecl far AudioCapabilities(void); // 68Ch -> Report audio driver capabilities.
extern "C" void cdecl far StopSound(void); // 68Fh -> Stop current sound from playing.


extern SETTINGS Settings;
int SOUND::CountObject=0;

static SNDSTRUC sound;

const int MAXBUFLEN=5000;
unsigned char buf[MAXBUFLEN];
short IsPlay;

extern "C" extern short	cdecl far PostAudioPending(SNDSTRUC far *sndplay);
extern "C" void cdecl far WaitSound(void); // Wait until sound playback completed.

void PlayFreq(int freq, int ampl, int len);
void PlayNote(int note, int volume, int len)
{
	//double scale=pow(2,1/12.0);
	//double freq=220*pow(scale,note);
	double freq=220*note;
	PlayFreq(freq, volume, len);
}

//���������
SOUND::SOUND(void)
{
	CountObject++;

	#if DEBUGMESSAGES
		Debug("Run SOUND constructor");
	#endif



	int yes;

//�맮� �����稪� ��� ��砫쭮� ����㧪� �⠭���⭮�� �ࠩ��� DIGPAK
	if ( ! (DIGPAK=LoadDigPak("driver\\SOUNDRV.COM")) )
		fatal("Failed to load sound driver.");
//� ��砥 ��㤠筮� ���樠����樨 �����蠥� �ணࠬ��
	if ( ! (INITDIGPAK=InitDigPak()) )
		Debug("Failed to initialize sound driver.");


//����㧪� MIDPAK

	if ( ! (MIDPAK=LoadMidPak("driver\\MIDPAK.COM", "driver\\MIDPAK.ADV", "driver\\MIDPAK.AD")) )
		fatal("Failed to load MIDPAK driver.");
	if ( ! (INITMIDPAK=InitMidPak()) )
		Debug("Failed to initialize MIDPAK driver.");

	if (  ! (DIGPAKAVAILABLE=DigPakAvailable()) );
//	Debug("DIGPAK not available from MIDPAK!");

	asm {
	xor ax,ax
	mov es,ax
	mov ax,es:[0x66*4+2]
	mov yes,ax
	}
	if ( !yes )
        	fatal("DIGPAK not loaded!");
	count=0;
}
//============ 'soundStatus' ================================
int SOUND::soundStatus(void)
{
  if ( DIGPAK && INITDIGPAK && DIGPAKAVAILABLE)
  {
	asm {
	mov ax,0x689
	int 0x66
	}
   return _AX;
  }
 return 0;
}


//=============== SndPlay - �ந���� ��� ===================
void SOUND::SndPlay(SNDSTRUCT far & sndplay)
{
   if ( DIGPAK && INITDIGPAK && DIGPAKAVAILABLE)
   {
	int yes;
	asm {
	xor ax,ax
	mov es,ax
	mov ax,es:[0x66*4+2]
	mov yes,ax
	}
	if ( !yes )
	{
		fatal("DIGPAK not loaded!");
		return;
	}

	asm {
	push ds
//	mov ax,0x688
	mov ax,0x694
	lds si,sndplay
	int 0x66
	mov ax,0x69f
	mov bx,99
	mov cx,99
	int 0x66
	pop ds
	}
    }
}
//============= end of 'SndPlay' ==============================



//============= 'play' - �ந���� ��� n =============
void SOUND::play(int n,int flag)
{
	return;

	if (Settings.EnableSound==0) return;
        //�᫨ ⥪�騩 ��� �ந��뢠���� � ०��� "�� ��⠭��������"
        //� ᥩ�� �ந��뢠���� ��㣮� ��� - ��⠭����� �ந��뢠���
	if (sounds[current].flag == NOSTOP && soundStatus()==YESPLAYING) return;

	if (n<0 || n>count-1 || count==0)
	{
				//WARNING ! This is only on some-time !
		//fatal ("SOUND::play - cannot play not exist a sound!");
        	return;
	}

	sounds[n].flag=flag;
	current=n;
	stop();	//��⠭�������� ⥪�騩 ���
	SndPlay(sounds[n]);
}
//============== end of 'play' ==================================


//============= 'stop' - ��⠭����� ⥪�騩 ��� =========
void SOUND::stop(void)
{
   if ( DIGPAK && INITDIGPAK && DIGPAKAVAILABLE)
   {
	asm mov ax,0x68F
	asm int 0x66
   }
}

//============ 'load' - ����㧨�� ��� � ������ ===============
int SOUND::load(char far * fname,int FreqDefault)
{
 FILE * stream;
 long file_size;
 if (count == count_max) { fatal ("SOUND::load - cannot load over 'cound_max' sounds!"); return -1; }

 // ���뢠�� 䠩� � ��।��塞 ��� �����
 if ( (stream = fopen(fname, "rb")) != NULL )
   {
     fseek(stream, 0L, SEEK_END);
     fgetpos(stream, &file_size);
     fseek(stream, 0L, SEEK_SET);
   }
 else fatal ("SOUND::load - cannot open file!");

 //�६���� !!!
// file_size = 12000;

 if (file_size>65535u) fatal("SOUND::load - length of file >65535");
 // �뤥�塞 ������ ��� ��� � ������塞 �������� SNDSTRUCT
//  sounds[count].sound=(unsigned char *) farmalloc(file_size);
  sounds[count].sound=new char[file_size];
//  sounds[count].sound=new char[file_size];
 if (sounds[count].sound == NULL)
   fatal("SOUND::load - not enough memory for load sound");

 // �⠥� �� 䠩��
 fread(sounds[count].sound, 1, file_size, stream);
// if (st!=1) fatal("SOUND::load - file loading error");

 for(long i=0; i<file_size; i++)
 {
   int a=sounds[count].sound[i];
   a-=128;
   a/=8;
   a+=128;
   sounds[count].sound[i]=a;

 }

 //����뢠�� 䠩�
 fclose(stream);

 sounds[count].sndlen=file_size;
 sounds[count].frequency=FreqDefault;		//���� �� 㬮�砭��
 sounds[count].IsPlaying=&isplay;	//���� 䫠��
 sounds[count].flag=0;
 count++;
 return count;
}
//================ end of 'load '========================================

//=============== 'unload' - ���㧨�� ��� n �� ����� =============
void SOUND::unload(int n)
{
	if (n<0 || n>count-1 || count==0) fatal ("SOUND::unload - cannot unload not exist a sound!");
	delete sounds[n].sound;
}
//================= end of 'unload' =====================


//================ �������� ============================
SOUND::~SOUND(void)
{
    if ( CountObject != 0 )
    {
	#if DEBUGMESSAGES
		Debug("Run SOUND destructor");
	#endif


//���㧪� �ࠩ��஢
	DeInitMidPak();
	UnLoadMidPak();

	DeInitDigPak();
	UnLoadDigPak();

	for (int i=0; i<count; i++)
		unload(i);
    }

    else	//�� ������ ��ꥪ� �� �뫮 ᮧ����
    {
	#if DEBUGMESSAGES
		Debug("SOUND-destructor: obj no exist!");
	#endif
    }

}
//================= end of '��������' ===================
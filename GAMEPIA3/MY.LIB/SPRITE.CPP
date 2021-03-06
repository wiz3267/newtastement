//----------------------------------
// (C) ������ �.�., 1997
//----------------------------------

#define FALSE 0
#define TRUE  1

//*************************************************************
//********* SPRITE.CPP  �������-����� ������ SPRITE ***********
//*************************************************************
#include <stdio.h>
#include <mem.h>
#include <fstream.h>
#include <string.h>

#include "debug.h"
#include "sprite.h"
#include "screen.h"

//�������������� ����������� ����������
int SPRITE::CountObject=0;  //����� ��������� ��������

// �������� �������
// Image - ����� ���������, ����������� ������
int SPRITE::Create(IMAGE& Image)
{
  //���������, ����� �� ��� ������� ������
  if ( Counter == SPRITES_MAX)
  {
    fatal("SPRITE::Create - sprites too many");
    return FALSE;
  }

  //���������, �������� �� ������
  if ( find(Image.Index) != -1 )
  {
      fatal("SPRITE::Create - Index already exist");
      return FALSE;
  }

  //��������� ��������� ��������� Image
  if (Image.Len > 320 || Image.Hei> 200)
  {
    fatal("SPRITE::Create - sprite too big");
    return FALSE;
  }

  //������� �������� ������ ��� ������
  Image.Datacolor = new char[Image.Len * Image.Hei];
  if ( !Image.Datacolor )
  {
    fatal("SPRITE::Create - can't allocate memory");
    return FALSE;
  }

  //�������� ��������� Image � ������� Count ������� �������� Sprites
        int k=find_free();
  Sprites[k]=Image;
  Counter++;
  return TRUE;
}



//================== SPRITE-����������� =====================
SPRITE::SPRITE(SCREEN far * screen, int SpMax)
{
  CountObject++;
  #if DEBUGMESSAGES
    Debug("Run SPRITE constructor");
  #endif

  //���������� ����� ������ ���� SCREEN
  scr = screen;
        //���������� ������������ ���������� �������� ���������� ��� �����
        //�������
        SPRITES_MAX=SpMax;

  Counter=0;  //���� ����� ��������� �������� ����� ����

  //�������� ������ ��� ������ ���� IMAGE
  Sprites=new IMAGE[SPRITES_MAX];

  //���� �� ������� �������� ������
  if ( !Sprites)
  {
    CountObject--;
    fatal("SPRITE::SPRITE - no ram for IMAGE[]");
  }
        for(int i=0; i<SPRITES_MAX; i++)
        {
          Sprites[i].Datacolor=0;
        }
}

// �����! ���������� ����� ���� ������ ��� ������ ������������ !!!
// ����� ����������, �������� ���
//����������
SPRITE::~SPRITE(void)
 {
     //���� ��������� ����������� �������� - �� ��� ������ �����������
     if (CountObject!=0)
     {

  #if DEBUGMESSAGES
    Debug("Run SPRITE destructor");
  #endif


  if ( Sprites != 0 )
  {
    //������� ������ �� ��� ��������
    for (int i=0; i<Counter; i++)
    {
      delete Sprites[i].Datacolor;
    }
    //������� ������ �� ��� ������� �������� ���� IMAGE
    delete Sprites;
  }
  else
  {
    Debug(" '~SPRITE' - Sprite==NULL! ");
  }
     }

     else //�� ������ ������� �� ���� �������
     {
  Debug("SPRITE-destructor: objects not exist");
     }

}

//����� ������ �������� ������� ������� ������� � ��������������� Index
//-1 ���
//0...(SPRITES_MAX-1) ����� �������� �������
int SPRITE::find(int Index)
{
  for (int i=0; i<Counter; i++)
  {
    if ( Sprites[i].Index==Index)
    {
      return i;
    }
  }

  return -1;
}

//����� ������ �������� �������, ��� ��� �������
//-1 ���
//0...(SPRITES_MAX-1) ����� �������� �������
int SPRITE::find_free()
{
  for (int i=0; i<SPRITES_MAX; i++)
  {
    if (Sprites[i].Datacolor==0) return i;
  }

        Debug("SPRITE::find_free - no free sprites!");
  return -1;
}


SPRITE::IMAGE far & SPRITE::GetImage(int Index)
{
  int n=find(Index);
  if (n==-1)
  {
    fatal("SPRITE::GetImage - Sprite not exist");
  }
  return Sprites[n];
}

//��������� ������ �� �����
// fname - ��� �����
// offset - �������� �� ������ �����
// Image - ����� ���������, ����������� ������
int SPRITE::Load(char far * fname, unsigned long offset, IMAGE Image)
{
  FILE * stream;

  //���������, ����� �� ��� ��������� ������
  if ( Counter == SPRITES_MAX)
  {
    fatal("SPRITE::Load - sprites too many");
    return FALSE;
  }

  //���������, �������� �� ������
  if ( find(Image.Index) != -1 )
  {
      Debug("SPRITE::Load - Index already exist");
      return FALSE;
  }

  //��������� ��������� ��������� Image
  if (Image.Len > 320 || Image.Hei> 200)
  {
    fatal("SPRITE::Load - sprite too big");
    return FALSE;
  }

  //��������� ����
  stream = fopen( fname, "rb" );

  //���� �� ������ ������� ����
  if ( !stream)
  {
    fatal("SPRITE::Load - can't open file");
    return FALSE;
  }

  //������������� ��������� �����
  if ( fseek( stream, offset, SEEK_SET) )
  {
    Debug("SPRITE::Load - can't seek pointer");
    fclose(stream);
    return FALSE;
  }

  //������� �������� ������ ��� ������
  Image.Datacolor = new char[Image.Len * Image.Hei];
  if ( !Image.Datacolor )
  {
    fatal("SPRITE::Load - can't allocate memory");
    fclose(stream);
    return FALSE;
  }


  //������ �� �����
  fread(Image.Datacolor, Image.Len, Image.Hei, stream);

  //��������� ����
  fclose(stream);

//  if ( l != Image.Len*Image.Hei )
//  {
//    Debug("SPRITE::Load - sprite loaded not all");
//    return FALSE;
//  }

  //�������� ��������� Image � ������� Count ������� �������� Sprites
  Sprites[find_free()]=Image;
  Counter++;
  return TRUE;
}


int SPRITE::LoadSpriteform(char * fname, SPRITE::IMAGE &im)
{
        ifstream ifl(fname, ios::binary);
        if (!ifl)
        {
          fatal("LoadSpriteForm - File not found");
                                  return 0;
        }


        SPRITEINFO sf;

        FILEHEADER fh;

        //��������� ���������
        ifl.read( (char*) &fh, sizeof(fh) );

        fh.Signature[9]=0;
        if ( _fstrcmp(fh.Signature, "SPRITEFOR") )
        {
          fatal("LoadSpriteform - bad file format");
                                        return 0;
                                }

                                //��������������� �� �������������� ���������
                                ifl.seekg( fh.Info );

                                //��������� �������� �������
                                ifl.read( (char*) &sf, sizeof(sf) );

                                //��������������� �� ������ �����
                                ifl.seekg( fh.Sprite);

        im.Len=sf.Len;
                                im.Hei=sf.Hei;
//                                im.Attribute=sf.Attribute;

                                //���� ������ ��������
                                if (Create(im))
                                {
                                        //��������� ��� ������
                                  ifl.read(im.Datacolor, im.Len*im.Hei);
                                }
                                else
        {
          fatal("Can't create the Sprite");
          return 0;

                                }
                          ifl.close();
                                return 1;

}


//���������� � ����� ������ ����� (����� ����) ��� n
void SPRITE::ModifyColor(int index, char n)
{
  IMAGE &im=GetImage(index);
  unsigned t=0;
  for(int i=0; i<im.Len; i++)
  for(int j=0; j<im.Hei; j++)
  {
	t=im.Read(i,j);
	if (t) im.Write(i,j, t + n);
  }
}

//================= Scroll - ��������� ������� � ��������� ============
//Index - ����� �������
//V - �� ������� ����� ��������
//F - ����� ������ ( SCROLL_UP, SCROLL_DOWN, SCROLL_LEFT, SCROLL_RIGHT )
int SPRITE::Scroll(int Index, int F, int)
{

  int n=find(Index);
  if (n==-1) {
    Debug("SPRITE::Scroll - sprite not exist");
    return FALSE;
  }

  IMAGE & Im = GetImage(Index);

        char *Datacolor=Im.Datacolor;
        int Len=Im.Len;
        int Hei=Im.Hei;

  char *save = new char[Im.Len];

        //��������� �����
  if (F & SCROLL_UP) {
    //��������� ������ ������ �������
    _fmemcpy( save, Im.Datacolor, Im.Len );
    //�������� ������ �����
    _fmemcpy( Im.Datacolor, Im.Datacolor+Im.Len,
      (Im.Hei-1)*Im.Len );
    //��������������� ������ ������
    _fmemcpy( Im.Datacolor+(Im.Hei-1)*(Im.Len), save, Im.Len );
  }

        //��������� ������
        if (F & SCROLL_RIGHT) {
          //������� ��� ��������� �� ����������
                asm {
                        push ds
                        push es

                        lds si,Datacolor
                  les di,Datacolor

                        mov dx,Hei
                        mov cx,Len
                }

        Scroll_Loop_0:

          asm {

                        dec cx

                        add di,cx

                        mov al,es:[di]

                        mov si,di
                        dec si

      std
                        rep movsb

                        mov es:[di],al

                        mov cx,Len
                        add di,cx

                        dec dx
                        jnz Scroll_Loop_0

      pop es
                        pop ds
                }
        }

  //������� ��������� �����
  delete save;
  return 0;
}



//================= 'ShowDirect' ���������� ������ � �������� � ������� =============
// x,y - ���������� ������ �������
// Index - ������������� �������
// Def - �������� ��� ������� bitblt
// �������: TRUE - ������ ���������, FALSE - ���������� ������ �� �������
int SPRITE::ShowDirect(int x,int y,int n,int Def)
{
  if (n>=Counter || n<0)
  {
    Debug("SPRITE::ShowDirect: Index out of range");
    return FALSE;
  }

    scr ->  bitblt(x,y,
      Sprites[n].Len, Sprites[n].Hei,
      Sprites[n].Datacolor, Sprites[n].Attribute | Def);
  return TRUE;
}

//================= 'Show' ���������� ������ � �������� Index =============
// x,y - ���������� ������ �������
// Index - ������������� �������
// Def - �������� ��� ������� bitblt, ������� ���������� � ���������� �������
// �������: TRUE - ������ ����������� �������, FALSE - ���������� ����������
// ����
int SPRITE::Show(int x,int y,int Index,int )
{
x++;y++;Index++;
  /*int n=find(Index);
  if (n==-1)
  {
	Debug("SPRITE::Show - sprite not exist");
	return FALSE;
  }*/

	/*scr ->  bitblt(x,y,
	  Sprites[n].Len, Sprites[n].Hei,
	  Sprites[n].Datacolor, Sprites[n].Attribute | Def);
	*/
  //scr->rectangle(x,y, 8-1,8-1, Index);
  //DrawBox(x,y,8
  return TRUE;
}
//������� ������ � ������� n
//????????????
int SPRITE::Delete(int Index)
{
  int n=find(Index);
  if (n==-1)
  {
    Debug("SPRITE::Delete - sprite not exist");
    return FALSE;
  }
        if (Sprites[n].Datacolor == 0)
        {
          Debug("SPRITE::Delete - you want delete space!");
                return FALSE;
        }

        delete Sprites[n].Datacolor;
		Sprites[n].Datacolor=0;
        Sprites[n].Index=-1;
        Counter--;

  return TRUE;
}

void SPRITE::IMAGE::Write(int x,int y,unsigned char color)
{
  if (x<0 || x>=Len || y<0 || y>=Hei)
        {
          Debug("IMAGE::Write - bad coordinate");
				return;
        }
        else
        {
          Datacolor[y*Len+x]=color;
        }
}

unsigned char SPRITE::IMAGE::Read(int x,int y)
{
  if (x<0 || x>=Len || y<0 || y>=Hei)
        {
          Debug("IMAGE::Read - bad coordinate");
                return 0;
        }
        else
        {
          return char(Datacolor[y*Len+x]);

        }
}
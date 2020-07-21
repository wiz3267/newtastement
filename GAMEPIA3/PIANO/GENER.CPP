// DTFM_GeneratorDlg.cpp : implementation file
//
#include "stdafx.h"
#include "DTFM_Generator.h"
#include "DTFM_GeneratorDlg.h"

#include <atlbase.h>
#include <math.h>
#include "inifile.h"

#pragma comment ( lib , "winmm.lib" )

//#import "D:\Program Files\Microsoft Visual Studio\VC98\Lib\winmm.lib"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DrawButton(CDC *dc, int x, int y, int start);

CINIFiles ini;

int BaseKeyboard=59;

int change=true;

HMIDIIN hmidiIn;

CString ENDL="\r\n";

CFile file;

BYTE *PlayWriten=NULL;
DWORD PlayWritenSize=0;
WORD PlayWritenPosition=0;

bool write=false; //нужно ли записывать в файл

union DWORD_BYTES
{
	DWORD data;
	BYTE b[4];
};

struct MIDI_DATA
{
	BYTE key;
	BYTE status;
	BYTE reserv1,reserv2;
	DWORD time;
};

double MaxSound=0;	//максимальный уровень звука на данный момент (до 32768)

#define GetData UpdateData(true)
#define PutData UpdateData(false)

double Piano(double Ampl, double freq, double t, double phase);

static int _time_;

const double
	
	AMPLITUDE_START=6000,
	AMPLITUDE_DECREMENT=10;

int IdMidiOpen=2;

const int 
	SAMPLE_RATE=44100,
	BUFFER_MAX=60,
	BUFFER_SIZE=100;

struct KEY
{
	UINT press;
	double decrement;
	double Ampl;
	KEY() { press=0; decrement=0; Ampl=0;}
} Keys[256];


struct _KEYSA_
{
	BYTE code;
	char * help;
};

_KEYSA_ Keysa[]=
{
	{9, "TAB"},
	{'Q', "Q"},
	{'2', "2"},
	{'W', "W"},
	{'3', "3"},
	{'E', "E"},
	{'R', "R"},
	{'5', "5"},
	{'T', "T"},
	{'6', "6"},
	{'Y', "Y"},
	{'7', "7"},
	{'U', "U"},
//	{'8', "8"},
	{'I', "I"},
	{'9', "9"},
	{'O', "O"},
	{'0', "0"},
	{'P', "P"},
	{219, "{"},
	{187, "+"},
	{221, "}"},
	{220, "|"},
	{13, "EN"},
	{8,"BK"},
	{46,"DEL"},
	{35,"END"},
	{36,"HM"},
	{34,"PD"},
	{33,"PU"},
	{103,"7"},
	{104,"8"},
	{111,"/"},
	{105,"9"},
	{106,"*"},
	{107,"+"},
	{109,"-"},
	{'M', "M"},
	{0,0}
};


//содержит информацию о нажатых клавишах
//UINT Keys[256];
//double KeysDec[256];
//double Ampl[256];


int aaaa;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTFM_GeneratorDlg dialog

CDTFM_GeneratorDlg::CDTFM_GeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDTFM_GeneratorDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CDTFM_GeneratorDlg)
	i_buffer_count = 0;
	m_blockcounter = 0;
	m_edit = _T("");
	m_midi_open_str = _T("");
	m_midi_name = _T("");
	m_amplitude = _T("4000");
	m_edit_freq = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	BlockCounter=0;
	hWaveOut=0;
	NeedClose=false;
	CurrentDigit=' ';
}

void CDTFM_GeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDTFM_GeneratorDlg)
	DDX_Control(pDX, IDC_EDIT_MIDI_NAME, m_midi_name_ctrl);
	DDX_Control(pDX, IDC_STOP_PLAY, m_stop_play);
	DDX_Control(pDX, IDC_BUTTON_PLAY_WRITEN, m_play_writen);
	DDX_Control(pDX, IDC_BUTTON_WRITE_STOP, m_button_write_stop);
	DDX_Control(pDX, IDC_BUTTON_WRITE, m_button_write);
	DDX_Control(pDX, IDC_BUTTON_MIDI_OPEN, m_midi_open);
	DDX_Control(pDX, IDC_BUTTON_MIDI_CLOSE, m_midi_close);
	DDX_Control(pDX, IDC_SLIDERM4, m_slm4);
	DDX_Control(pDX, IDC_SLIDERM3, m_slm3);
	DDX_Control(pDX, IDC_SLIDERM2, m_slm2);
	DDX_Control(pDX, IDC_SLIDER4, m_sl4);
	DDX_Control(pDX, IDC_SLIDER3, m_sl3);
	DDX_Control(pDX, IDC_SLIDER2, m_sl2);
	DDX_Control(pDX, IDC_SLIDER1, m_sl1);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDOK, m_generate);
	DDX_Text(pDX, IDC_EDIT_BUFFER_COUNT, i_buffer_count);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT_MIDI_OPEN, m_midi_open_str);
	DDX_Text(pDX, IDC_EDIT_MIDI_NAME, m_midi_name);
	DDX_Text(pDX, IDC_AMPLITUDE, m_amplitude);
	DDX_Text(pDX, IDC_EDIT_FREQ, m_edit_freq);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDTFM_GeneratorDlg, CDialog)
	//{{AFX_MSG_MAP(CDTFM_GeneratorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PLAY_STRING, OnPlayString)
	ON_BN_CLICKED(IDC_ABORT, OnAbort)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MIDI_CLOSE, OnButtonMidiClose)
	ON_BN_CLICKED(IDC_BUTTON_MIDI_OPEN, OnButtonMidiOpen)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_STOP, OnButtonWriteStop)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_WRITEN, OnButtonPlayWriten)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_CLOSE, OnClose2)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_STOP_PLAY, OnStopPlay)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDTFM_GeneratorDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTFM_GeneratorDlg message handlers


int sl1, sl2, sl3, sl4;
int slm2, slm3, slm4;

double Piano(double Ampl, double freq, double t, double phase)
{
	double k=0;

	double limit=20000;	//ограничение частоты
	
	double AMP=Ampl;

	double f;
	if (freq<limit) k+=sl1/100.0*Ampl*sin(freq*t+phase);

	f=2*freq;
	if (f<limit) k+=sl2/100.0*AMP*sin(f*t+0.3);

	f=3*freq;
	if (f<limit) k+=sl3/100.0*AMP*sin(f*t+0.7);

	f=4*freq;
	if (f<limit) k+=sl4/100.0*AMP*sin(f*t+0.5);

	k+=slm2/100.0*AMP*sin(freq/2*t+0.3);
	k+=slm3/100.0*AMP*sin(freq/3*t+0.7);
	k+=slm4/100.0*AMP*sin(freq/4*t+0.5);

	
/*for(int i=2; i<2; i++)
	{
		k+=Ampl/i*sin(i*freq*t+0.3*i);
	}

	for(i=2; i<2; i++)
	{
		k+=Ampl/i*sin(freq/i*t+0.3*i);
	}
*/

	return k;
}

BOOL CDTFM_GeneratorDlg::OnInitDialog()
{
	InitToneData();
	CDialog::OnInitDialog();
	OnOK();

	m_sl1.SetScrollRange(SB_CTL,0,100);

	m_sl2.SetScrollRange(SB_CTL,0,100);
	m_sl3.SetScrollRange(SB_CTL,0,100);
	m_sl4.SetScrollRange(SB_CTL,0,100);


	m_slm2.SetScrollRange(SB_CTL,0,100);
	m_slm3.SetScrollRange(SB_CTL,0,100);
	m_slm4.SetScrollRange(SB_CTL,0,100);

	if (ini.IsExist())
	{

	m_sl1.SetPos(ini.QueryValue("sl1"));
	m_sl2.SetPos(ini.QueryValue("sl2"));
	m_sl3.SetPos(ini.QueryValue("sl2"));
	m_sl4.SetPos(ini.QueryValue("sl4"));

	m_slm2.SetPos(ini.QueryValue("slm2"));
	m_slm3.SetPos(ini.QueryValue("slm3"));
	m_slm4.SetPos(ini.QueryValue("slm4"));

	}
	else
	{
		m_sl1.SetPos(100);
	}
	

	SetTimer(0,500,NULL);
	SetTimer(1,100,NULL);

	OnButtonMidiOpen();

	SetWindowText("Midi In Piano");

	if (ini.QueryValue("HideWindow")==1)
	{
		ShowWindow(SW_HIDE);
	}

	return FALSE;
}

void CDTFM_GeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CDTFM_GeneratorDlg::OnPaint() 
{

	change=true;

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDTFM_GeneratorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDTFM_GeneratorDlg::OnOK() 
{
	if (!GetData) return;

	mode=0;
	
	int channels=1;	//число каналов
	samplerate=SAMPLE_RATE;	//частота дискретизации
	//по закону должна быть как минимум в два раза выше максимально используемой

	WAVEFORMATEX wform;	//формат данных устройства
	wform.wFormatTag=WAVE_FORMAT_PCM;	//используется PCM-данны
	wform.nChannels=WORD(channels);			 //моно (1 канал)
	wform.nSamplesPerSec=samplerate;	//частота дискретиазации
	wform.nAvgBytesPerSec=samplerate*channels;	//скорость потока данных (для моно
	//wform.nBlockAlign=(1*8)/8;	//выравнивание блока данных
	wform.nBlockAlign=2;	//выравнивание блока данных
	wform.wBitsPerSample=16;		//число битов на выборку

	//пытаемся открыть wave-device
	UINT rc=waveOutOpen(&hWaveOut, WAVE_MAPPER, &wform, (DWORD) m_hWnd,0, CALLBACK_WINDOW);

	//неудача ?
	if (rc) {
		TextError(rc);
		return;
	}

	//waveOutPause(hWaveOut);
	for(int i=0; i<BUFFER_MAX; i++) AddBuffer(0,0,0, BUFFER_SIZE);
	//waveOutRestart(hWaveOut);

	m_generate.EnableWindow(false);
	m_stop.EnableWindow();
	SetFocus();
}

struct TONEDATA
{
	double f1;
	BYTE sym;
	int idc;
};

const double DO=1, RE=9/8.0, ME=5/4.0, FA=4/3.0, SOL=3/2.0, LA=5/3.0, SI=15/8.0;

//const double DIES=6/5.0;	//=1.2

const int DigitCount=250;
TONEDATA digit[DigitCount]=
{
	{ DO*1,		60,	IDC_DO },		//do
	{ DO*2,		61,	IDC_DO_DIES,},	//DO dies
	{ DO*3,		62,	IDC_RE},		//re
	{ DO*4,		63,	IDC_RE_DIES,},	//re dies
	{ DO*5,		64,	IDC_ME },		//me
	{ DO*6,		65,	IDC_FA },		//fa
	{ DO*7,		66,	IDC_FA_DIES,},	//fa dies
	{ DO*8,		67,	IDC_SOL },		//sol
	{ DO*9,		68,	IDC_SOL_DIES,},	//sol dies
	{ DO*10,	69,	IDC_LA },		//la
	{ DO*11,	70, IDC_LA_DIES,},	//la dies
	{ DO*12,	71,	IDC_SI },		//si

	{ DO*13, 72, IDC_DO2 },
	{ DO*14, 73, IDC_DO2_DIES,},	
	{ DO*15, 74, IDC_RE2 },
	{ DO*16, 75, IDC_RE2_DIES},
	{ DO*17, 76, IDC_ME2 },
	{ DO*18, 77, IDC_FA2 },
	{ DO*19, 78, IDC_SOL2 }
};

BOOL CDTFM_GeneratorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == MM_WOM_DONE)
	{
		WAVEHDR *wh=(WAVEHDR*) pMsg->lParam;
		TextError(waveOutUnprepareHeader(hWaveOut, wh,
			sizeof(WAVEHDR)));
		
		BlockCounter--;

		GetData;
		i_buffer_count=BlockCounter;
		PutData;

		int bufsize=wh->dwBufferLength;

		if (mode==MODE_NEED_CLOSE_WAVEOUT && !BlockCounter)
		{
			TextError(waveOutClose(hWaveOut));
			hWaveOut=0;
			m_stop.EnableWindow(false);
			m_generate.EnableWindow(true);
			SetFocus();
		}

		if (BlockCounter==0 && NeedClose==true) 
		{
			OnClose();
		}
		delete wh->lpData;
		delete wh;

		//добавляем в очередь новый буфер!!!!!!!!!!!!
		//если не нужно закрывать
		if (mode==0)
		{
			MaxSound=0;
			AddBuffer();

			/*static k;
			k++;
			if (k>4)
			{
				k=0;
				//MaxSound=0;
			}*/
		}

	}

	UINT nChar=pMsg->wParam;

	if (pMsg->message == WM_KEYDOWN && hWaveOut)
	{

		if (nChar==VK_LEFT)
		{

			if (BaseKeyboard>0) BaseKeyboard--;
		
			change=true;

			CDC *pdc=GetDC();
			DrawButton(pdc, 0,0, 60);
			ReleaseDC(pdc);
		}

		if (nChar==VK_RIGHT)
		{

			if (BaseKeyboard<127) BaseKeyboard++;

			change=true;
			CDC *pdc=GetDC();
			DrawButton(pdc, 0,0, 60);
			ReleaseDC(pdc);

		}

		//OnChar(nChar,0,0);
		for(int i=0; i<256; i++)
		{
			if (Keysa[i].code==0) break;
			if (Keysa[i].code==nChar)
			{
				int r=i+BaseKeyboard;

				MidiKeyPress(r,127);
			}

		}

		return 0;
	}

	if (pMsg->message == WM_KEYUP && hWaveOut)
	{
		//OnChar(nChar,0,0);
		for(int i=0; i<256; i++)
		{
			if (Keysa[i].code==0) break;
			if (Keysa[i].code==nChar)
			{
				int r=i+BaseKeyboard;
				MidiKeyPress(r,0);
			}

		}

		return 0;
	}


	//return CDialog::PreTranslateMessage(pMsg);

	return 0;
}

const double PI=3.1415926;
double scale=pow(2.0,1.0/12.0);

void CDTFM_GeneratorDlg::AddBuffer()
{

	UpdateData(true);

	WAVEHDR * wh1=new WAVEHDR;
	WAVEHDR& wh=*wh1;

	int size=BUFFER_SIZE;
	BYTE *plbuf=new BYTE[size];

	double freq_1=0;

	for(int i=0; i<size; i+=2, _time_++)
	{
		double t=_time_*2*PI/samplerate;
		double m=0;

		for(int k=0; k<256; k++) 
		{
			if (Keys[k].press) 
			{
				double freq=440*pow(scale,k-60+3);
				freq_1=freq;
				static double phase;
				phase=0;

				int AMP=20000;

				m+=Piano(Keys[k].Ampl,freq,t,phase);
			}
		}
		
		//m*=2;

		if (m>32000) m=32000;
		if (m<-32000) m=-32000;

		if (m>MaxSound) MaxSound=m;

		short int z=(short int)m;

		BYTE *s=(BYTE*)&z;

		plbuf[i+0]=s[0];
		plbuf[i+1]=s[1];
	}

	double SumAmpl=0;

	for(int i=0; i<256; i++)
	{
		SumAmpl+=Keys[i].Ampl;

		if (Keys[i].Ampl>0) 
		{
			Keys[i].Ampl-=Keys[i].decrement;
		}

		if (Keys[i].Ampl<=0) 
		{
			Keys[i].Ampl=0;
			Keys[i].decrement=0;
			Keys[i].press=0;
		}
	}
	
	if (fabs(SumAmpl)<1) 
	{
		_time_=0;
	}

	wh.dwFlags=0;
	wh.dwBufferLength=size;
	wh.dwLoops=1;
	wh.lpData=(char*)plbuf;
	wh.dwUser='b';

	int rc=waveOutPrepareHeader(hWaveOut, &wh, sizeof(wh));
	TextError(rc);
	rc=waveOutWrite(hWaveOut, &wh, sizeof(wh));
	TextError(rc);

	BlockCounter++;

//	GetData;

	m_blockcounter=BlockCounter;

	m_edit_freq=freq_1;
	PutData;

}

void CDTFM_GeneratorDlg::AddBuffer(double freq1, double freq2, char sym, int size)
{
	WAVEHDR * wh1=new WAVEHDR;
	WAVEHDR& wh=*wh1;

//	int size=1024;
	BYTE *plbuf=new BYTE[size];

	for(int i=0; i<size; i++)
	{
		plbuf[i]=0;
	}

	wh.dwFlags=0;//WHDR_BEGINLOOP | WHDR_ENDLOOP;
	wh.dwBufferLength=size;
	wh.dwLoops=1;
	wh.lpData=(char*)plbuf;
	wh.dwUser=sym;

	int rc=waveOutPrepareHeader(hWaveOut, &wh, sizeof(wh));
	TextError(rc);

	rc=waveOutWrite(hWaveOut, &wh, sizeof(wh));

	TextError(rc);

	BlockCounter++;
}


void CDTFM_GeneratorDlg::OnClose() 
{
	if (!BlockCounter) 
	{

		OnButtonMidiClose();

		ExitDialog();
	}
	else
	{
		mode=MODE_NEED_CLOSE_WAVEOUT;
		NeedClose=true;
	}
}

void CDTFM_GeneratorDlg::OnStop() 
{
	if (BlockCounter) mode=MODE_NEED_CLOSE_WAVEOUT;

	GetData;
	i_buffer_count=BlockCounter;
	PutData;

	if (BlockCounter==0)
	{
		m_generate.EnableWindow();
		m_stop.EnableWindow(false);
		SetFocus();
		TextError(waveOutClose(hWaveOut));

		hWaveOut=0;
	}
}

int CDTFM_GeneratorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

// TODO: Add your specialized creation code here
//	CheckRadioButton(IDC_RADIO_CONSTANT,IDC_RADIO_CONSTANT,IDC_RADIO_CONSTANT);
	
	return 0;
}

BOOL CDTFM_GeneratorDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

//void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//}

void CDTFM_GeneratorDlg::ExitDialog()
{
	GetData;


	ini.SetValue(m_sl1.GetPos(),"sl1");
	ini.SetValue(m_sl2.GetPos(),"sl2");
	ini.SetValue(m_sl3.GetPos(),"sl3");
	ini.SetValue(m_sl4.GetPos(),"sl4");

	ini.SetValue(m_slm2.GetPos(),"slm2");
	ini.SetValue(m_slm3.GetPos(),"slm3");
	ini.SetValue(m_slm4.GetPos(),"slm4");

	
	EndDialog(0);
}

void CDTFM_GeneratorDlg::OnPlayString() 
{
}

void CDTFM_GeneratorDlg::OnAbort() 
{
	OnButtonMidiClose();
	CDialog::OnOK();
}

void CDTFM_GeneratorDlg::TextError(MMRESULT rc)
{
	if (!rc) return;
	char buf[8192];
	waveOutGetErrorText(rc, buf,8192);
	//MessageBox(buf);
}	

BOOL CDTFM_GeneratorDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CDialog::OnCommand(wParam, lParam);
}


LRESULT CDTFM_GeneratorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == MM_MIM_DATA)
	{
		DWORD_BYTES mm;
		mm.data=lParam;

		if (mm.b[0] != 254)
		{
			GetData;
			
			CString s;
			s.Format("%i,%i\t%i\t%i\t%i",mm.b[0]>>4,mm.b[0]&15,mm.b[1],mm.b[2],mm.b[3]);

			static int counter=0;
			counter++;
			if ( (counter %10) == 0)
			{
				m_edit="";
			}

			m_edit += s+ENDL;

			BYTE nChar=mm.b[1];
			BYTE Volume=mm.b[2];
			if ((mm.b[0]>>4) == 9 || (mm.b[0]>>4) == 8)
			{
				if (nChar<256) 
				{
					MidiKeyPress(nChar, Volume);
				}
			}
			UpdateData(false);
		}

	}

	if (message == MM_MIM_LONGDATA)
	{
		int b=3;
		b++;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDTFM_GeneratorDlg::InitToneData()
{
}

KEY KeysOld[256];

void DrawButton(CDC *dc, int x, int y, int start)
{

	CBrush br_level,br2;
	br2.CreateSolidBrush(RGB(250,250,250));
	//br_level.CreateSolidBrush(RGB(255*MaxSound/32768.0,0,0));
	br_level.CreateSolidBrush(RGB(255,0,0));
	dc->SelectObject(br_level);

	int L;
	int H;

	x=10;
	y=200;

	L=300;
	H=10;

	if (MaxSound)
	{
		int k=int(MaxSound/32768.0*L);

		dc->SelectObject(br_level);
		dc->Rectangle(x,y,x+k,y+H);
		dc->SelectObject(br2);
		dc->Rectangle(x+k,y,x+L,y+H);
	}


	for(int i=0; i<256; i++)
	{
		if ( (KeysOld[i].press!=Keys[i].press) 
			|| KeysOld[i].Ampl != Keys[i].Ampl) change=true;
	}

	if (change)
	{
		for(int i=0; i<256; i++) KeysOld[i]=Keys[i];
	}
	else return;

	change=false;


	L=20;
	H=60;

	y=250;
	x=1;


	CBrush br_red,br_bl;
	br_red.CreateSolidBrush(RGB(0,250,200));
	
	br_bl.CreateSolidBrush(RGB(10,10,10));



	int Z[]={0,2,4,5,7,9,11};
	int R[]={1,3,6,8,10};

	/*CFont font;
	LOGFONT lf={0};
	lf.lfWidth=L-5;
	lf.lfHeight=lf.lfWidth*2;
	font.CreateFontIndirect(&lf);
	dc->SelectObject(font);

	dc->SetBkMode(TRANSPARENT);
	*/

	start=12+12*3;

//	CBrush * oldbr=dc->SelectObject(&br_red);

	for(int T=0; T<10; T++)
	{
		for(int i=0; i<7; i++)
		{

			CBrush bra;


			int ampl=Keys[start+ Z[i]].Ampl;

			int a=250*ampl/4000;
			if (a>250) a=250;
			int b=200*ampl/4000;
			if (b>200) b=200;

			bra.CreateSolidBrush(RGB(0,a,b));

			if (Keys[start+ Z[i]].press)
				dc->SelectObject(bra);
			else
				dc->SelectObject(br2);

			int x1=x+L*i;

			dc->Rectangle(x1,y,x+L*i+L, y+H);

			if ( (start+Z[i])==BaseKeyboard)
			{
				dc->Ellipse(x1+3, y+H-1, x1+L-6, y+H-8);
			}

			//dc->TextOut(x1+1,y+H-lf.lfHeight,"A");
		}

		int p=0;
		for(int i=0; i<7; i++)
		{
			int L2=int(L*0.6);
			int H2=int(H*0.6);
			if (i==2 || i==6) continue;
			int s=(2*L-L2)/2;

			if (Keys[start+ R[p]].press)
				dc->SelectObject(br_red);
			else
				dc->SelectObject(br_bl);

			int x1=x+s+L*i;


			dc->Rectangle(x1,y,x+s+L*i+L2, y+H2);

			if ( (start+R[p])==BaseKeyboard)
			{
				dc->SelectObject(br2);
				dc->Ellipse(x1+1, y+H-1, x1+L2, y+H-8);
			}

			p++;

		}

		x+=L*7;
		
		start+=12;
	}

//	dc->SelectObject(oldbr);
}



void CDTFM_GeneratorDlg::OnTimer(UINT nIDEvent) 
{
	sl1=m_sl1.GetPos();
	sl2=m_sl2.GetPos();
	sl3=m_sl3.GetPos();
	sl4=m_sl4.GetPos();

	slm2=m_slm2.GetPos();
	slm3=m_slm3.GetPos();
	slm4=m_slm4.GetPos();


	CDC *pdc=GetDC();
	DrawButton(pdc, 0,0, 60);
	ReleaseDC(pdc);

	if (ini.QueryValue("HideWindow")) ShowWindow(SW_HIDE);
	else ShowWindow(SW_SHOW);

	CDialog::OnTimer(nIDEvent);
}

//SetWindowsHookEx

void CDTFM_GeneratorDlg::OnButtonMidiClose() 
{
	if (hmidiIn!=NULL)
	{
		int err=midiInStop(hmidiIn);

		if (err!=MMSYSERR_NOERROR) 
		{
			m_midi_name="Ошибка midiInStop";
			return;
		}

		err=midiInClose(hmidiIn);
		if (err!=MMSYSERR_NOERROR) 
		{
			m_midi_name="Ошибка midiInClose";
		}

		m_midi_close.EnableWindow(false);
		m_midi_open.EnableWindow(true);
		SetFocus();

		m_midi_name="midi close";

		PutData;

		hmidiIn=NULL;
	}
	
}

void CDTFM_GeneratorDlg::OnButtonMidiOpen() 
{
	GetData;

	if (hmidiIn!=NULL) return;

	int err;

	int midicount=midiInGetNumDevs();

	MIDIINCAPS mic={0};

	
	IdMidiOpen=atoi(m_midi_open_str);

	
	m_midi_open_str.Format("%i",IdMidiOpen);

	midiInGetDevCaps(IdMidiOpen,&mic,sizeof(mic));

	err=midiInOpen(&hmidiIn, IdMidiOpen, (DWORD) this->m_hWnd, 0, CALLBACK_WINDOW);

	if (err==MMSYSERR_NOERROR)
	{
		m_midi_name=mic.szPname;
	}

	if (err!=MMSYSERR_NOERROR)
	{
		m_midi_name="MidiInOpen error";

		PutData;

		//midiInClose(hmidiIn);
		hmidiIn=NULL;
		return;
	}

	err=midiInStart(hmidiIn);
	if (err!=MMSYSERR_NOERROR)
	{
		m_midi_name="MidiInStart error";
		PutData;

		midiInClose(hmidiIn);
		hmidiIn=NULL;
		return;
	}


	m_midi_open.EnableWindow(false);
	m_midi_close.EnableWindow(true);
	SetFocus();

	PutData;
}

void CDTFM_GeneratorDlg::OnButtonWrite() 
{
	DeleteFile("midi.dat");
	if (file.Open("midi.dat",file.modeWrite|file.modeCreate))
	{
		m_button_write.EnableWindow(false);
		m_button_write_stop.EnableWindow(true);
		SetFocus();
		write=true;
	}
	else
	{
		//MessageBox("File busy");
	}
	
}

void CDTFM_GeneratorDlg::OnButtonWriteStop() 
{
	m_button_write.EnableWindow(true);
	m_button_write_stop.EnableWindow(false);
	SetFocus();
	write=false;
	file.Close();
	
}

DWORD START_TIME;

void CALLBACK TimerProc_PlayMidi(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT_PTR idEvent,  // timer identifier
  DWORD dwTime       // current system time
)
{
 	if (PlayWritenPosition*sizeof(MIDI_DATA)>=PlayWritenSize)
	{
		//KillTimer(5555);
		//::KillTimer(NULL,5555);
		PlayWritenPosition=0;
		START_TIME=GetTickCount();
		return;
	}

	int j=PlayWritenPosition;
	//MIDI_DATA *md=(MIDI_DATA*) &PlayWriten;
	MIDI_DATA *md=(MIDI_DATA*) PlayWriten;
	//md+=PlayWritenPosition;

	double K=1;
	double delta=(md[j].time - md[0].time)*K;
	double delta2=GetTickCount()-START_TIME;
	if ( delta <= delta2)
	{
		BYTE nChar=md[j].key;
		if (md[j].status>0)
		{

			Keys[nChar].press=1;
			Keys[nChar].Ampl=4000;
			Keys[nChar].decrement=0;
		}
		else
		{
			Keys[nChar].decrement=AMPLITUDE_DECREMENT;
		}
		PlayWritenPosition++;

	}
	
}


void CDTFM_GeneratorDlg::OnButtonPlayWriten() 
{
	if (PlayWriten) return;

/*	HRSRC hr=FindResource(NULL,"IDR_USER","USERDATA");
	int err=GetLastError();

	HGLOBAL hgl=LoadResource(NULL,hr);
	BYTE *bufz=(BYTE*)LockResource(hgl);

	int size2=SizeofResource(NULL,hr);

	
	PlayWriten=new BYTE[size2];
	PlayWritenSize=size2;
	memcpy(PlayWriten,bufz,size2);
	
	UnlockResource(bufz);
*/

	CFile f;
	if (!f.Open("midi.dat",file.modeRead)) return;
	int size=f.GetLength();
	PlayWriten=new BYTE[size];
	PlayWritenSize=size;
	if (PlayWriten==NULL)
	{
		PlayWritenSize=0;
		f.Close();
		return;
	}

	f.Read(PlayWriten, PlayWritenSize);
	f.Close();


	m_play_writen.EnableWindow(false);
	m_stop_play.EnableWindow(true);
	SetFocus();


	PlayWritenPosition=0;

	START_TIME=GetTickCount();

	SetTimer(5555,1, (TIMERPROC)TimerProc_PlayMidi);

}

void CDTFM_GeneratorDlg::OnButtonReset() 
{
	for(int i=0; i<256; i++)
	{
		Keys[i].Ampl=0;
		Keys[i].press=0;
	}
}

void CDTFM_GeneratorDlg::OnMove(int x, int y) 
{
//	CDialog::OnMove(x, y);
	
	
}

void CDTFM_GeneratorDlg::OnClose2() 
{
	OnClose();
}

void CDTFM_GeneratorDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CDTFM_GeneratorDlg::OnStopPlay() 
{
	delete PlayWriten;
	PlayWriten=NULL;

	m_stop_play.EnableWindow(false);
	m_play_writen.EnableWindow(true);
	SetFocus();
	OnButtonReset();

	KillTimer(5555);
}

void CDTFM_GeneratorDlg::SetFocus()
{
	m_midi_name_ctrl.SetFocus();
}

void CDTFM_GeneratorDlg::MidiKeyPress(BYTE key, BYTE value)
{
	if (hWaveOut==NULL) return;

	if (value!=0)
	{
		Keys[key].press=true;
		//Keys[key].Ampl=atoi(m_amplitude)*value/127.0;
		Keys[key].Ampl=atoi(m_amplitude)*value/127.0;

		Keys[key].decrement=0;

		CDC *pdc=GetDC();
		DrawButton(pdc, 0,0, 60);
		ReleaseDC(pdc);
	}
	else 
	{
		Keys[key].decrement=AMPLITUDE_DECREMENT;
	}

	if (write)
	{
			MIDI_DATA md={0};
			md.key=key;
			md.status=value;
			md.time=GetTickCount();
			file.Write(&md, sizeof(md));
	}

}

void CDTFM_GeneratorDlg::OnHide() 
{
	ini.SetValue(1,"HideWindow");
	ShowWindow(SW_HIDE);	
}

void CDTFM_GeneratorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

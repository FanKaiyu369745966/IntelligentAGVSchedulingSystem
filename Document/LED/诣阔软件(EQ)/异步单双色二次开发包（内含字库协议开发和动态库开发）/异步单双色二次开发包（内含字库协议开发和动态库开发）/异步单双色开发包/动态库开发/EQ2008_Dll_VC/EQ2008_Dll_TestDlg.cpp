// EQ2008_Dll_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EQ2008_Dll_Test.h"
#include "EQ2008_Dll_TestDlg.h"
#include "EQ2008_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CEQ2008_Dll_TestDlg dialog

CEQ2008_Dll_TestDlg::CEQ2008_Dll_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEQ2008_Dll_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEQ2008_Dll_TestDlg)
	m_iScreenX = 0;
	m_iScreenY = 0;
	m_iScreenHeight = 32;
	m_iScreenWidth = 64;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//控制卡地址
	m_iCardNum=1;
	m_iProgramIndex=-1;
	m_iProgramCount=0;
}

void CEQ2008_Dll_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEQ2008_Dll_TestDlg)
	DDX_Text(pDX, IDC_SCREEN_X, m_iScreenX);
	DDV_MinMaxUInt(pDX, m_iScreenX, 0, 4096);
	DDX_Text(pDX, IDC_SCREEN_Y, m_iScreenY);
	DDX_Text(pDX, IDC_SCREEN_HEIGHT, m_iScreenHeight);
	DDV_MinMaxUInt(pDX, m_iScreenHeight, 16, 512);
	DDX_Text(pDX, IDC_SCREEN_WIDTH, m_iScreenWidth);
	DDV_MinMaxUInt(pDX, m_iScreenWidth, 16, 4096);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_CARDADDR, m_ctrCardNum);
}

BEGIN_MESSAGE_MAP(CEQ2008_Dll_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CEQ2008_Dll_TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDPROGRAM, OnAddprogram)
	ON_BN_CLICKED(IDC_AJUSTTIME, OnAjusttime)
	ON_BN_CLICKED(IDC_ADDTEXT, OnAddtext)
	ON_BN_CLICKED(IDC_ADDRTF, OnAddrtf)
	ON_BN_CLICKED(IDC_ADDSINGLELINETEXT, OnAddsinglelinetext)
	ON_BN_CLICKED(IDC_ADDBMP, OnAddbmp)
	ON_BN_CLICKED(IDC_ADDTIME, OnAddtime)
	ON_BN_CLICKED(IDC_ADDTIMECOUNT, OnAddtimecount)
	ON_BN_CLICKED(IDC_ADDTEMPERATURE, OnAddtemperature)
	ON_BN_CLICKED(IDC_CLOSESCREEN, OnClosescreen)
	ON_BN_CLICKED(IDC_OPENSCREEN, OnOpenscreen)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_DLLPROGRAM, OnDllprogram)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_REALTIMESEND, OnBtnRealtimesend)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_CARDADDR, &CEQ2008_Dll_TestDlg::OnCbnSelchangeComboCardaddr)
	ON_BN_CLICKED(IDC_BTN_REALTIMESEND2, &CEQ2008_Dll_TestDlg::OnBnClickedBtnRealtimesend2)
	ON_BN_CLICKED(IDC_BTN_REALTIMESEND3, &CEQ2008_Dll_TestDlg::OnBnClickedBtnRealtimesend3)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT2, &CEQ2008_Dll_TestDlg::OnBnClickedBtnDisconnect2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEQ2008_Dll_TestDlg message handlers

BOOL CEQ2008_Dll_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString strNum;
	for(int i=1;i<256;i++)
	{
		strNum.Format("%d",i);
		m_ctrCardNum.AddString(strNum);
	}
	m_ctrCardNum.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEQ2008_Dll_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEQ2008_Dll_TestDlg::OnPaint() 
{
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
HCURSOR CEQ2008_Dll_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//函数：选择控制卡地址
void CEQ2008_Dll_TestDlg::OnCbnSelchangeComboCardaddr()
{
	this->UpdateData();
	m_iCardNum =m_ctrCardNum.GetCurSel() + 1;
}

//函数：添加节目索引
void CEQ2008_Dll_TestDlg::OnAddprogram() 
{
	m_iProgramIndex=User_AddProgram(m_iCardNum,FALSE,10);

	//提示信息
	CString str;
	m_iProgramCount++;
	str.Format("当前节目号：%d",m_iProgramCount);
	this->SetDlgItemText(IDC_STATIC1,str);
}

//函数：添加文本窗
void CEQ2008_Dll_TestDlg::OnAddtext() 
{
	User_Text  Text;

	Text.BkColor = 0;
	Text.chContent = "欢迎使用EQ2008型控制卡动态库!";

	Text.PartInfo.FrameColor = 0;
	Text.PartInfo.iFrameMode = 0;
	Text.PartInfo.iHeight = 32;
	Text.PartInfo.iWidth = 96;
	Text.PartInfo.iX = 0;
	Text.PartInfo.iY = 0;

	Text.FontInfo.bFontBold = FALSE;
	Text.FontInfo.bFontItaic= FALSE;
	Text.FontInfo.bFontUnderline = FALSE;
	Text.FontInfo.colorFont = 0xFFFF;
	Text.FontInfo.iFontSize = 12;
	Text.FontInfo.strFontName = "";
	Text.FontInfo.iAlignStyle = 1;
	Text.FontInfo.iVAlignerStyle = 0;
	Text.FontInfo.iRowSpace = 0;

	Text.MoveSet.bClear = 0;
	Text.MoveSet.iActionSpeed = 4;
	Text.MoveSet.iActionType = 0;
	Text.MoveSet.iHoldTime = 10;
	Text.MoveSet.iClearActionType	= 0;
	Text.MoveSet.iClearSpeed		= 4;
	Text.MoveSet.iFrameTime			= 10;

	if(-1 == User_AddText(m_iCardNum,&Text,m_iProgramIndex))
	{
		MessageBox("添加文本失败！");
	}
	else
	{
		MessageBox("添加文本成功！");
	}
}

//函数：添加RTF文件
void CEQ2008_Dll_TestDlg::OnAddrtf() 
{
	User_RTF  RTF;

	RTF.strFileName = "EQ2008_RTF.rtf";

	RTF.PartInfo.FrameColor		= 0;
	RTF.PartInfo.iFrameMode		= 0;
	RTF.PartInfo.iHeight		= 32;
	RTF.PartInfo.iWidth			= 64;
	RTF.PartInfo.iX				= 0;
	RTF.PartInfo.iY				= 0;

	RTF.MoveSet.bClear			= 0;
	RTF.MoveSet.iActionSpeed	= 4;
	RTF.MoveSet.iActionType		= 0;
	RTF.MoveSet.iHoldTime		= 10;
	RTF.MoveSet.iClearActionType= 0;
	RTF.MoveSet.iClearSpeed		= 4;
	RTF.MoveSet.iFrameTime		= 10;

	if(-1 == User_AddRTF(m_iCardNum,&RTF,m_iProgramIndex))
	{
		MessageBox("添加rtf文件失败！");
	}
	else
	{
		MessageBox("添加rtf文件成功！");
	}
}

//函数：添加单行文本
void CEQ2008_Dll_TestDlg::OnAddsinglelinetext() 
{
	User_SingleText  SingleText;

	SingleText.BkColor		= 0;
	SingleText.chContent	= "欢迎使用！";
	SingleText.PartInfo.iFrameMode	= 0;
	SingleText.PartInfo.iHeight		= 32;
	SingleText.PartInfo.iWidth		= 64;
	SingleText.PartInfo.iX = 0;
	SingleText.PartInfo.iY = 0;
	SingleText.FontInfo.bFontBold		= 0;
	SingleText.FontInfo.bFontItaic		= 0;
	SingleText.FontInfo.bFontUnderline	= 0;
	SingleText.FontInfo.colorFont		= 0xFF;
	SingleText.FontInfo.iFontSize		= 16;
	SingleText.PartInfo.FrameColor		= 0;
	SingleText.FontInfo.strFontName		= "";
	SingleText.MoveSet.bClear			= 0;
	SingleText.MoveSet.iActionSpeed		= 0;
	SingleText.MoveSet.iActionType		= 20;
	SingleText.MoveSet.iHoldTime		= 0;
	SingleText.MoveSet.iClearActionType	= 0;
	SingleText.MoveSet.iClearSpeed		= 4;
	SingleText.MoveSet.iFrameTime		= 50;
	if(-1 == User_AddSingleText(m_iCardNum,&SingleText,m_iProgramIndex))
	{
		MessageBox("添加单行文本失败！");
	}
	else
	{
		MessageBox("添加单行文本成功！");
	}
}

//函数：添加图片
void CEQ2008_Dll_TestDlg::OnAddbmp() 
{
	User_Bmp		BmpZone;
	CBitmap			bitmap;
	User_MoveSet	MoveSet;
	int             iBMPZoneNum=0;

	BmpZone.PartInfo.iX		 = 0;
	BmpZone.PartInfo.iY		 = 0;
	BmpZone.PartInfo.iHeight = 32;
	BmpZone.PartInfo.iWidth  = 64;
	BmpZone.PartInfo.FrameColor = 0xFF00;
	BmpZone.PartInfo.iFrameMode = 1;

	MoveSet.bClear			= 1;
	MoveSet.iActionSpeed	= 4;
	MoveSet.iActionType		= 0;
	MoveSet.iHoldTime		= 50;
	MoveSet.iClearActionType= 0;
	MoveSet.iClearSpeed		= 4;
	MoveSet.iFrameTime		= 10;

	iBMPZoneNum = User_AddBmpZone(m_iCardNum,&BmpZone,m_iProgramIndex);

	bitmap.LoadBitmap(IDB_BITMAP1);
	if(-1 == User_AddBmp(m_iCardNum,iBMPZoneNum,(HBITMAP)bitmap,&MoveSet,m_iProgramIndex))
	{
		AfxMessageBox("添加图片失败！");
	}

	bitmap.DeleteObject();
	if(-1 == User_AddBmpFile(m_iCardNum,iBMPZoneNum,"BMP2.bmp",&MoveSet,m_iProgramIndex))
	{
		MessageBox("添加图片失败！");
	}
	else
	{
		MessageBox("添加图片成功！");
	}
	
}

//函数：添加时间
void CEQ2008_Dll_TestDlg::OnAddtime() 
{
  	User_DateTime  DateTime;
  
  	DateTime.bDay = 0;
  	DateTime.bHour = 1;
  	DateTime.BkColor = 0;
  	DateTime.bMin = 1;
  	DateTime.bMouth = 0;
  	DateTime.bMulOrSingleLine = 0;
  	DateTime.bSec =0;
  	DateTime.bWeek = 0;
  	DateTime.bYear = 0;
  	DateTime.bYearDisType = 0;
  	DateTime.chTitle = "";
  
  	DateTime.PartInfo.iFrameMode = 1;
  	DateTime.iDisplayType = 1;
  	DateTime.PartInfo.FrameColor =0xFFFF;
  	DateTime.PartInfo.iHeight = 32;
  	DateTime.PartInfo.iWidth = 64;
  	DateTime.PartInfo.iX=0;
  	DateTime.PartInfo.iY=0;
  	DateTime.FontInfo.bFontBold=0;
  	DateTime.FontInfo.bFontItaic=0;
  	DateTime.FontInfo.bFontUnderline=0;
  	DateTime.FontInfo.colorFont = 0xFF;
  	DateTime.FontInfo.iAlignStyle =1;
  	DateTime.FontInfo.iFontSize = 12;
  	DateTime.FontInfo.strFontName = "";
  
  	if(-1 == User_AddTime(m_iCardNum,&DateTime,m_iProgramIndex))
	{
		MessageBox("添加时间失败！");
	}
	else
	{
		MessageBox("添加时间成功！");
	}
}

//函数：添加计时
void CEQ2008_Dll_TestDlg::OnAddtimecount() 
{
	User_Timer  TimeCount;

	TimeCount.bDay = 1;
	TimeCount.bHour = 0;
	TimeCount.BkColor =0;
	TimeCount.bMin = 0;
	TimeCount.bMulOrSingleLine =1;
	TimeCount.bSec =0;
	TimeCount.chTitle = "距离五一";
	TimeCount.FontInfo.bFontBold=0;
	TimeCount.FontInfo.bFontItaic=0;
	TimeCount.FontInfo.bFontUnderline=0;
	TimeCount.FontInfo.colorFont=0xFFFF;
	TimeCount.FontInfo.iAlignStyle=2;
	TimeCount.FontInfo.iFontSize=16;
	TimeCount.FontInfo.strFontName="";
	TimeCount.PartInfo.FrameColor = 0xFF00;
	TimeCount.PartInfo.iFrameMode =0;
	TimeCount.PartInfo.iHeight = 32;
	TimeCount.PartInfo.iWidth = 64;
	TimeCount.PartInfo.iX =0;
	TimeCount.PartInfo.iY=0;
	TimeCount.ReachTimeYear=2007;
	TimeCount.ReachTimeMonth=5;
	TimeCount.ReachTimeDay= 1;
	TimeCount.ReachTimeHour=10;
	TimeCount.ReachTimeMinute=0;
	TimeCount.ReachTimeSecond=0;

	if(-1 == User_AddTimeCount(m_iCardNum,&TimeCount,m_iProgramIndex))
	{
		MessageBox("添加计时失败！");
	}
	else
	{
		MessageBox("添加计时成功！");
	}
}

//函数：添加温度
void CEQ2008_Dll_TestDlg::OnAddtemperature() 
{
	User_Temperature  Temperature;

	Temperature.BkColor=0;
	Temperature.chTitle="";
	Temperature.DisplayType=0;
	Temperature.PartInfo.FrameColor=0xFF00;
	Temperature.PartInfo.iFrameMode=1;
	Temperature.PartInfo.iHeight=32;
	Temperature.PartInfo.iWidth=64;
	Temperature.PartInfo.iX=0;
	Temperature.PartInfo.iY=0;
	Temperature.FontInfo.bFontBold=0;
	Temperature.FontInfo.bFontItaic=0;
	Temperature.FontInfo.bFontUnderline=0;
	Temperature.FontInfo.colorFont=0xFFFF;
	Temperature.FontInfo.iAlignStyle=0;
	Temperature.FontInfo.iFontSize=12;
	Temperature.FontInfo.strFontName="宋体";

	if(-1 == User_AddTemperature(m_iCardNum,&Temperature,m_iProgramIndex))
	{
		MessageBox("添加温度失败！");
	}
	else
	{
		MessageBox("添加温度成功！");
	}
}

//函数：删除所有节目
void CEQ2008_Dll_TestDlg::OnDllprogram() 
{
	if(!User_DelAllProgram(m_iCardNum))
	{
		MessageBox("删除节目失败！");
	}
	else
	{
		//提示信息
		m_iProgramCount=0;
		this->SetDlgItemText(IDC_STATIC1,"请先添加节目！");
	}
}

//函数：发送数据到显示屏
void CEQ2008_Dll_TestDlg::OnSend() 
{
	if(!User_SendToScreen(m_iCardNum))
	{
		MessageBox("数据发送失败！");
	}
	else
	{
		MessageBox("数据发送成功！");
	}

}

//函数：校正时间
void CEQ2008_Dll_TestDlg::OnAjusttime() 
{
	if(!User_AdjustTime(m_iCardNum))
	{
		MessageBox("校准时间失败！");
	}
	else
	{
		MessageBox("校准时间成功！");
	}
}

//函数：打开显示屏
void CEQ2008_Dll_TestDlg::OnOpenscreen() 
{
	if(!User_OpenScreen(m_iCardNum))
	{
		MessageBox("打开显示屏失败！");
	}
	else
	{
		MessageBox("打开显示屏成功！");
	}
}

//函数：关闭显示屏
void CEQ2008_Dll_TestDlg::OnClosescreen() 
{
	if(!User_CloseScreen(m_iCardNum))
	{
		MessageBox("关闭显示屏失败！");
	}
	else
	{
		MessageBox("关闭显示屏成功！");
	}
}

//函数：实时发送数据，建立连接
//日期：2008-04-30
void CEQ2008_Dll_TestDlg::OnBtnConnect() 
{
	if(!User_RealtimeConnect(m_iCardNum))
	{
		MessageBox("实时发送数据建立连接失败！");
	}
	else
	{
		MessageBox("实时发送数据建立连接成功！");
	}
}

//函数：实时发送数据，发送数据
//日期：2008-04-30
void CEQ2008_Dll_TestDlg::OnBtnRealtimesend() 
{
	UpdateData();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);
	if(FALSE == User_RealtimeSendData(m_iCardNum,m_iScreenX,m_iScreenY,m_iScreenWidth,m_iScreenHeight,(HBITMAP)bitmap))
	{
		MessageBox("发送实时图片句柄失败！");
	}
	else
	{
		MessageBox("发送实时图片句柄成功！");
	}

}

//函数：实时发送图片路径
//日期：2015-09-21
void CEQ2008_Dll_TestDlg::OnBnClickedBtnRealtimesend2()
{
	UpdateData();
	if(FALSE == User_RealtimeSendBmpData(m_iCardNum,m_iScreenX,m_iScreenY,m_iScreenWidth,m_iScreenHeight,"test1.bmp"))
	{
		MessageBox("发送实时图片路径失败！");
	}
	else
	{
		MessageBox("发送实时图片路径成功！");
	}
}

//函数：实时发送文本内容
//日期：2015-09-21
void CEQ2008_Dll_TestDlg::OnBnClickedBtnRealtimesend3()
{
	UpdateData();
	char  strText[] = "实时发送文本测试!";
	User_FontSet FontInfo;

	FontInfo.bFontBold = FALSE;
	FontInfo.bFontItaic= FALSE;
	FontInfo.bFontUnderline = FALSE;
	FontInfo.colorFont = 0xFFFF;
	FontInfo.iFontSize = 12;
	FontInfo.strFontName = "宋体";
	FontInfo.iAlignStyle = 0;
	FontInfo.iVAlignerStyle = 0;
	FontInfo.iRowSpace = 0;

	if(!User_RealtimeSendText(m_iCardNum,m_iScreenX,m_iScreenY,m_iScreenWidth,m_iScreenHeight,strText,&FontInfo))
	{
		MessageBox("发送实时文本失败！");
	}
	else
	{
		MessageBox("发送实时文本成功！");
	}
}

//函数：实时发送数据，断开连接
//日期：2008-04-30
void CEQ2008_Dll_TestDlg::OnBtnDisconnect() 
{
	if(!User_RealtimeDisConnect(m_iCardNum))
	{
		MessageBox("实时发送数据断开连接失败！");
	}
	else
	{
		MessageBox("实时发送数据断开连接成功！");
	}
}


//函数：清空控制卡节目
//日期：2015-09-21
void CEQ2008_Dll_TestDlg::OnBnClickedBtnDisconnect2()
{
	if(!User_RealtimeScreenClear(m_iCardNum))
	{
		MessageBox("清空控制卡节目失败！");
	}
	else
	{
		MessageBox("清空控制卡节目成功！");
	}
}

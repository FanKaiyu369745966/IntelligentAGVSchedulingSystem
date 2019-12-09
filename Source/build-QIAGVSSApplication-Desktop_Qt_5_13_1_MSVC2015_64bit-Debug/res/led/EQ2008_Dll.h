#ifndef __EQ2008_DLL__
#define __EQ2008_DLL__

#ifdef DLL_API
#else
#define DLL_API _declspec(dllimport) 
#endif

#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <Windows.h>

typedef struct						//��������
{
	char*		strFontName;		//���������
	int			iFontSize;			//����Ĵ�С
	BOOL		bFontBold;			//�����Ƿ�Ӵ�
	BOOL		bFontItaic;			//�����Ƿ���б��
	BOOL		bFontUnderline;		//�����Ƿ���»���
	COLORREF	colorFont;			//�������ɫ
	int			iAlignStyle;		//���뷽ʽ
									//0�� ����� 
									//1������ 
									//2���Ҷ���
	int         iVAlignerStyle;		//���¶��뷽ʽ
									//0-������
									//1-���¾���
									//2-�׶���
	int         iRowSpace;			//�м��
}User_FontSet;

typedef struct						//��������
{
	int			iX;					//���ڵ����X
	int			iY;					//���ڵ����Y
	int			iWidth;				//����Ŀ��
	int			iHeight;			//����ĸ߶�
	int			iFrameMode;			//�߿����ʽ
	COLORREF	FrameColor;			//�߿���ɫ
}User_PartInfo;

typedef struct _User_MoveSet
{
	int			iActionType;		//��Ŀ�任��ʽ
	int			iActionSpeed;		//��Ŀ�Ĳ����ٶ�	
	BOOL		bClear;				//�Ƿ���Ҫ�������
	int			iHoldTime;			//����Ļ��ͣ����ʱ��
	int         iClearSpeed;		//�����ʾ�����ٶ�
	int			iClearActionType;	//��Ŀ����ı任��ʽ
	int         iFrameTime;
}User_MoveSet;

//����ʱ���ڡ� 
typedef struct _User_Timer
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	DWORD			ReachTimeYear;		//������
	DWORD			ReachTimeMonth;		//������
	DWORD			ReachTimeDay;		//������
	DWORD			ReachTimeHour;		//����ʱ
	DWORD			ReachTimeMinute;	//�����
	DWORD			ReachTimeSecond;	//������
	BOOL			bDay;				//�Ƿ���ʾ�� 0������ʾ 1����ʾ
	BOOL			bHour;				//�Ƿ���ʾСʱ
	BOOL			bMin;				//�Ƿ���ʾ����
	BOOL			bSec;				//�Ƿ���ʾ��
	BOOL			bMulOrSingleLine;	//���л��Ƕ���
	char			*chTitle;			//�����ʾ����
}User_Timer;


//���¶ȴ��ڡ�
typedef struct _User_Temperature
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	char			*chTitle;			//����
	int				DisplayType;		//��ʾ��ʽ��0���� 1��C
}User_Temperature;

//������ʱ�䴰�ڡ� 
typedef struct _User_DateTime
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	int				iDisplayType;		//��ʾ���
	char			*chTitle;			//�����ʾ����
	BOOL			bYearDisType;		//���λ��0 ��4��1��2λ
	BOOL			bMulOrSingleLine;	//���л��Ƕ���
	BOOL			bYear;
	BOOL			bMouth;
	BOOL			bDay;
	BOOL			bWeek;
	BOOL			bHour;
	BOOL			bMin;
	BOOL			bSec;
}User_DateTime;

//��ͼ�Ŀ� 
typedef struct _User_Bmp
{
	User_PartInfo	PartInfo;			 //������Ϣ
}User_Bmp;

//�������ı���
typedef struct _User_SingleText
{
	char			*chContent;         //��ʾ����
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	User_MoveSet	MoveSet;			//������ʽ����
}User_SingleText;

//���ı��� 
typedef struct _User_Text
{
	char			*chContent;         //��ʾ����
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	User_MoveSet	MoveSet;			//������ʽ����
}User_Text;

//��RTF�ļ��� 
typedef struct _User_RTF
{
	char            *strFileName;       //RTF�ļ���
	User_PartInfo	PartInfo;			//������Ϣ
	User_MoveSet    MoveSet;			//������ʽ����
}User_RTF;

#endif _DATASTRUCTURE_H_


/****************����˵��******************/
//��ɫ����
#define RED					0x0000FF
#define GREEN				0x00FF00
#define YELLOW				0x00FFFF

//����ֵ����
#define EQ_FALSE			0
#define EQ_TRUE			    1

//1����Ŀ����������===================================================
//��ӽ�Ŀ
DLL_API int __stdcall User_AddProgram(int CardNum,BOOL bWaitToEnd,int iPlayTime);
//ɾ�����н�Ŀ
DLL_API BOOL __stdcall User_DelAllProgram(int CardNum);

//���ͼ����
DLL_API int  __stdcall User_AddBmpZone(int CardNum,User_Bmp *pBmp,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmp(int CardNum,int iBmpPartNum,HBITMAP hBitmap,User_MoveSet* pMoveSet,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmpFile(int CardNum,int iBmpPartNum,char *strFileName,User_MoveSet* pMoveSet,int iProgramIndex);
//����ı���
DLL_API int __stdcall User_AddText(int CardNum,User_Text *pText,int iProgramIndex);
//���RTF��
DLL_API int __stdcall User_AddRTF(int CardNum,User_RTF *pRTF,int iProgramIndex);
//��ӵ����ı���
DLL_API int __stdcall User_AddSingleText(int CardNum,User_SingleText *pSingleText,int iProgramIndex);
//���ʱ����
DLL_API int __stdcall User_AddTime(int CardNum,User_DateTime *pDateTime,int iProgramIndex);
//��Ӽ�ʱ��
DLL_API int __stdcall User_AddTimeCount(int CardNum,User_Timer *pTimeCount,int iProgramIndex);
//����¶���
DLL_API int __stdcall User_AddTemperature(int CardNum,User_Temperature *pTemperature,int iProgramIndex);

//��������
DLL_API BOOL __stdcall User_SendToScreen(int CardNum);


//2��ʵʱ���º�����=================================================
//ʵʱ�������ݽ�������
DLL_API BOOL __stdcall User_RealtimeConnect(int CardNum);
//ʵʱ����ͼƬ���
DLL_API BOOL __stdcall User_RealtimeSendData(int CardNum,int x,int y,int iWidth,int iHeight,HBITMAP hBitmap);
//ʵʱ����ͼƬ�ļ�
DLL_API BOOL __stdcall User_RealtimeSendBmpData(int CardNum,int x,int y,int iWidth,int iHeight,char *strFileName);	 
//ʵʱ�����ı�
DLL_API BOOL __stdcall User_RealtimeSendText(int CardNum,int x,int y,int iWidth,int iHeight,char *strText,User_FontSet	*pFontInfo);	
//ʵʱ���ͶϿ�����
DLL_API BOOL __stdcall User_RealtimeDisConnect(int CardNum);	
//ʵʱ��������
DLL_API BOOL __stdcall User_RealtimeScreenClear(int CardNum);	


//3����ʾ�����ƺ�����==============================================
//����
DLL_API BOOL __stdcall User_OpenScreen(int CardNum);
//����
DLL_API BOOL __stdcall User_CloseScreen(int CardNum);
//У��ʱ��
DLL_API BOOL __stdcall User_AdjustTime(int CardNum);
//���ȵ���
DLL_API BOOL __stdcall User_SetScreenLight(int CardNum,int iLightDegreen);
//Reload�����ļ�
DLL_API void __stdcall User_ReloadIniFile(char *pEQ2008_Dll_Set_Path);

/********************************************************************************/

#endif __EQ2008_DLL__

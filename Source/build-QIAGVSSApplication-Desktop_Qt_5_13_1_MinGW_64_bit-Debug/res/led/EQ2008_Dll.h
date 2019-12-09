#ifndef __EQ2008_DLL__
#define __EQ2008_DLL__

#ifdef DLL_API
#else
#define DLL_API _declspec(dllimport) 
#endif

#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <Windows.h>

typedef struct						//字体设置
{
	char*		strFontName;		//字体的名称
	int			iFontSize;			//字体的大小
	BOOL		bFontBold;			//字体是否加粗
	BOOL		bFontItaic;			//字体是否是斜体
	BOOL		bFontUnderline;		//字体是否带下划线
	COLORREF	colorFont;			//字体的颜色
	int			iAlignStyle;		//对齐方式
									//0－ 左对齐 
									//1－居中 
									//2－右对齐
	int         iVAlignerStyle;		//上下对齐方式
									//0-顶对齐
									//1-上下居中
									//2-底对齐
	int         iRowSpace;			//行间距
}User_FontSet;

typedef struct						//窗体设置
{
	int			iX;					//窗口的起点X
	int			iY;					//窗口的起点Y
	int			iWidth;				//窗体的宽度
	int			iHeight;			//窗体的高度
	int			iFrameMode;			//边框的样式
	COLORREF	FrameColor;			//边框颜色
}User_PartInfo;

typedef struct _User_MoveSet
{
	int			iActionType;		//节目变换方式
	int			iActionSpeed;		//节目的播放速度	
	BOOL		bClear;				//是否需要清除背景
	int			iHoldTime;			//在屏幕上停留的时间
	int         iClearSpeed;		//清除显示屏的速度
	int			iClearActionType;	//节目清除的变换方式
	int         iFrameTime;
}User_MoveSet;

//“计时窗口” 
typedef struct _User_Timer
{
	User_PartInfo	PartInfo;			//分区信息
	COLORREF		BkColor;			//背景颜色
	User_FontSet	FontInfo;			//字体设置
	DWORD			ReachTimeYear;		//到达年
	DWORD			ReachTimeMonth;		//到达月
	DWORD			ReachTimeDay;		//到达日
	DWORD			ReachTimeHour;		//到达时
	DWORD			ReachTimeMinute;	//到达分
	DWORD			ReachTimeSecond;	//到达秒
	BOOL			bDay;				//是否显示天 0－不显示 1－显示
	BOOL			bHour;				//是否显示小时
	BOOL			bMin;				//是否显示分钟
	BOOL			bSec;				//是否显示秒
	BOOL			bMulOrSingleLine;	//单行还是多行
	char			*chTitle;			//添加显示文字
}User_Timer;


//“温度窗口”
typedef struct _User_Temperature
{
	User_PartInfo	PartInfo;			//分区信息
	COLORREF		BkColor;			//背景颜色
	User_FontSet	FontInfo;			//字体设置
	char			*chTitle;			//标题
	int				DisplayType;		//显示格式：0－度 1－C
}User_Temperature;

//“日期时间窗口” 
typedef struct _User_DateTime
{
	User_PartInfo	PartInfo;			//分区信息
	COLORREF		BkColor;			//背景颜色
	User_FontSet	FontInfo;			//字体设置
	int				iDisplayType;		//显示风格
	char			*chTitle;			//添加显示文字
	BOOL			bYearDisType;		//年份位数0 －4；1－2位
	BOOL			bMulOrSingleLine;	//单行还是多行
	BOOL			bYear;
	BOOL			bMouth;
	BOOL			bDay;
	BOOL			bWeek;
	BOOL			bHour;
	BOOL			bMin;
	BOOL			bSec;
}User_DateTime;

//“图文框” 
typedef struct _User_Bmp
{
	User_PartInfo	PartInfo;			 //分区信息
}User_Bmp;

//“单行文本框”
typedef struct _User_SingleText
{
	char			*chContent;         //显示内容
	User_PartInfo	PartInfo;			//分区信息
	COLORREF		BkColor;			//背景颜色
	User_FontSet	FontInfo;			//字体设置
	User_MoveSet	MoveSet;			//动作方式设置
}User_SingleText;

//“文本框” 
typedef struct _User_Text
{
	char			*chContent;         //显示内容
	User_PartInfo	PartInfo;			//分区信息
	COLORREF		BkColor;			//背景颜色
	User_FontSet	FontInfo;			//字体设置
	User_MoveSet	MoveSet;			//动作方式设置
}User_Text;

//“RTF文件” 
typedef struct _User_RTF
{
	char            *strFileName;       //RTF文件名
	User_PartInfo	PartInfo;			//分区信息
	User_MoveSet    MoveSet;			//动作方式设置
}User_RTF;

#endif _DATASTRUCTURE_H_


/****************参数说明******************/
//颜色常量
#define RED					0x0000FF
#define GREEN				0x00FF00
#define YELLOW				0x00FFFF

//返回值常量
#define EQ_FALSE			0
#define EQ_TRUE			    1

//1、节目操作函数组===================================================
//添加节目
DLL_API int __stdcall User_AddProgram(int CardNum,BOOL bWaitToEnd,int iPlayTime);
//删除所有节目
DLL_API BOOL __stdcall User_DelAllProgram(int CardNum);

//添加图文区
DLL_API int  __stdcall User_AddBmpZone(int CardNum,User_Bmp *pBmp,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmp(int CardNum,int iBmpPartNum,HBITMAP hBitmap,User_MoveSet* pMoveSet,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmpFile(int CardNum,int iBmpPartNum,char *strFileName,User_MoveSet* pMoveSet,int iProgramIndex);
//添加文本区
DLL_API int __stdcall User_AddText(int CardNum,User_Text *pText,int iProgramIndex);
//添加RTF区
DLL_API int __stdcall User_AddRTF(int CardNum,User_RTF *pRTF,int iProgramIndex);
//添加单行文本区
DLL_API int __stdcall User_AddSingleText(int CardNum,User_SingleText *pSingleText,int iProgramIndex);
//添加时间区
DLL_API int __stdcall User_AddTime(int CardNum,User_DateTime *pDateTime,int iProgramIndex);
//添加计时区
DLL_API int __stdcall User_AddTimeCount(int CardNum,User_Timer *pTimeCount,int iProgramIndex);
//添加温度区
DLL_API int __stdcall User_AddTemperature(int CardNum,User_Temperature *pTemperature,int iProgramIndex);

//发送数据
DLL_API BOOL __stdcall User_SendToScreen(int CardNum);


//2、实时更新函数组=================================================
//实时发送数据建立连接
DLL_API BOOL __stdcall User_RealtimeConnect(int CardNum);
//实时发送图片句柄
DLL_API BOOL __stdcall User_RealtimeSendData(int CardNum,int x,int y,int iWidth,int iHeight,HBITMAP hBitmap);
//实时发送图片文件
DLL_API BOOL __stdcall User_RealtimeSendBmpData(int CardNum,int x,int y,int iWidth,int iHeight,char *strFileName);	 
//实时发送文本
DLL_API BOOL __stdcall User_RealtimeSendText(int CardNum,int x,int y,int iWidth,int iHeight,char *strText,User_FontSet	*pFontInfo);	
//实时发送断开连接
DLL_API BOOL __stdcall User_RealtimeDisConnect(int CardNum);	
//实时发送清屏
DLL_API BOOL __stdcall User_RealtimeScreenClear(int CardNum);	


//3、显示屏控制函数组==============================================
//开屏
DLL_API BOOL __stdcall User_OpenScreen(int CardNum);
//关屏
DLL_API BOOL __stdcall User_CloseScreen(int CardNum);
//校正时间
DLL_API BOOL __stdcall User_AdjustTime(int CardNum);
//亮度调节
DLL_API BOOL __stdcall User_SetScreenLight(int CardNum,int iLightDegreen);
//Reload参数文件
DLL_API void __stdcall User_ReloadIniFile(char *pEQ2008_Dll_Set_Path);

/********************************************************************************/

#endif __EQ2008_DLL__

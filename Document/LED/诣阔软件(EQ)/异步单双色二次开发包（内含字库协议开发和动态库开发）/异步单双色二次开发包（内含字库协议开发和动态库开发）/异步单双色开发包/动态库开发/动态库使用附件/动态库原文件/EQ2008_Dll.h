#ifndef __EQ2008_DLL__
#define __EQ2008_DLL__

#ifdef DLL_API
#else
#define DLL_API _declspec(dllimport) 
#endif

#include "EQ2008_DataStruct.h" 

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

Attribute VB_Name = "Function"

'/****************参数说明******************/

'颜色常量
Public Const RED = &HFF00FF
Public Const GREEN = &HFFFF00
Public Const YELLOW = &HFFFFFF

'返回值常量
Public Const EQ_FALSE = 0
Public Const EQ_TRUE = 1

'/***************************************/
'1、节目编辑函数组
'添加节目
Public Declare Function User_AddProgram Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal bWaitToEnd As Boolean, ByVal iPlayTime As Long) As Long
'添加图文区及向图文区添加图片
Public Declare Function User_AddBmpZone Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pBmp As User_Bmp, ByVal iProgramIndex As Long) As Long
Public Declare Function User_AddBmp Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal iBmpPartNum As Long, ByVal hBitmap As Long, ByRef pMoveSet As User_MoveSet, ByVal iProgramIndex As Long) As Boolean
'添加时间区
Public Declare Function User_AddTime Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pDateTime As User_DateTime, ByVal iProgramIndex As Long) As Long
'添加倒计时区
Public Declare Function User_AddTimeCount Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pTimeCount As User_Timer, ByVal iProgramIndex As Long) As Long
'添加温度区
Public Declare Function User_AddTemperature Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pTemperature As User_Temperature, ByVal iProgramIndex As Long) As Long
'添加文本区
Public Declare Function User_AddText Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pText As User_Text, ByVal iProgramIndex As Long) As Long
'添加RTF区
Public Declare Function User_AddRTF Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pRTF As User_RTF, ByVal iProgramIndex As Long) As Long
'添加单行文本区
Public Declare Function User_AddSingleText Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByRef pSingleText As User_SingleText, ByVal iProgramIndex As Long) As Long
'删除所有节目
Public Declare Function User_DelAllProgram Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'发送数据
Public Declare Function User_SendToScreen Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean

'2、实时更新数据函数组
'实时发送数据建立连接
Public Declare Function User_RealtimeConnect Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'实时发送图片句柄
Public Declare Function User_RealtimeSendData Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal x As Long, ByVal y As Long, ByVal iWidth As Long, ByVal iHeight As Long, ByVal hBitmap As Long) As Boolean
'实时发送图片文件
Public Declare Function User_RealtimeSendBmpData Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal x As Long, ByVal y As Long, ByVal iWidth As Long, ByVal iHeight As Long, ByVal strFileName As String) As Boolean
'实时发送文本
Public Declare Function User_RealtimeSendText Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal x As Long, ByVal y As Long, ByVal iWidth As Long, ByVal iHeight As Long, ByVal strText As String, ByRef pFontInfo As User_FontSet) As Boolean
'实时发送断开连接
Public Declare Function User_RealtimeDisConnect Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'实时发送清屏
Public Declare Function User_RealtimeScreenClear Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean

'3、显示屏控制函数组
'开屏
Public Declare Function User_OpenScreen Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'关屏
Public Declare Function User_CloseScreen Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'校正时间
Public Declare Function User_AdjustTime Lib "EQ2008_Dll.dll" (ByVal CardNum As Long) As Boolean
'亮度调节
Public Declare Function User_SetScreenLight Lib "EQ2008_Dll.dll" (ByVal CardNum As Long, ByVal iLightDegreen As Long) As Boolean
'重新加载配置文件INI
Public Declare Function User_ReloadIniFile Lib "EQ2008_Dll.dll" (ByVal strEQ2008_Dll_Set_Path As String) As Boolean


'
Public Declare Function GetCurrentDirectory Lib "kernel32" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long

 

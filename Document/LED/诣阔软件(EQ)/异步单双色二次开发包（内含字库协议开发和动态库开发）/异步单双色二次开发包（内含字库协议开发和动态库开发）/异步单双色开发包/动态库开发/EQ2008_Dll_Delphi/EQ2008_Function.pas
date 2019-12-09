unit EQ2008_Function;

interface
  Const
  EQ2008_Dll = 'EQ2008_Dll.dll';


//颜色常量
   RED				=		$0000FF;
   GREEN			=		$00FF00;
   YELLOW			=		$00FFFF;

//返回值常量
   EQ_FALSE		=		0;
   EQ_TRUE		=	  1;

Type
  	User_FontSet  = Packed Record   //字体设置
    strFontName: String;            //字体的名称
    iFontSize: Longword	;           //字体的大小
    bFontBold: Longword;            //字体是否加粗
    bFontItaic: Longword;           //字体是否是斜体
    bFontUnderline: Longword;       //字体是否带下划线
    colorFont: Longword;            //字体的颜色
    iAlignStyle: Longword;          //对齐方式
                                    //0－ 左对齐
                                    //1－居中
                                    //2－右对齐
    iVAlignerStyle: Longword;       //上下对齐方式
                                    //0-顶对齐
                                    //1-上下居中
                                    //2-底对齐
    iRowSpace: Longword;            //行间距
		End;
		PUser_FontSet = ^User_FontSet;

		User_PartInfo  = Packed Record     //字体设置
    iX: Longword;                      //窗口的起点X
    iY: Longword;                      //窗口的起点Y
    iWidth: Longword;                  //窗体的宽度
    iHeight: Longword;                 //窗体的高度
    iFrameMode: Longword;              //边框的样式
    FrameColor: Longword;              //边框颜色
		End ;
		PUser_PartInfo = ^User_PartInfo;

		User_MoveSet = Packed Record
    iActionType: Longword;             //节目变换方式
    iActionSpeed: Longword;            //节目的播放速度
    bClear: Longword;                  //是否需要清除背景
    iHoldTime: Longword;               //在屏幕上停留的时间
    iClearSpeed: Longword;		         //清除显示屏的速度
	  iClearActionType: Longword;	       //节目清除的变换方式
	  iFrameTime: Longword;
		End ;
		PUser_MoveSet = ^User_MoveSet;

		//“计时窗口”
		User_Timer = Packed Record
    PartInfo: User_PartInfo;           //分区信息
    BkColor: Longword;                 //背景颜色
    FontInfo: User_FontSet;            //字体设置
    ReachTimeYear: Longword;           //到达年
    ReachTimeMonth: Longword;          //到达月
    ReachTimeDay: Longword;            //到达日
    ReachTimeHour: Longword;           //到达时
    ReachTimeMinute: Longword;         //到达分
    ReachTimeSecond: Longword;         //到达秒
    bDay: Longword;                    //是否显示天 0－不显示 1－显示
    bHour: Longword;                   //是否显示小时
    bMin: Longword;                    //是否显示分钟
    bSec: Longword;                    //是否显示秒
    bMulOrSingleLine: Longword;        //单行还是多行
    chTitle: String;                   //添加显示文字
		End;
		PUser_Timer = ^User_Timer;

		//“温度窗口”
		User_Temperature = Packed Record
    PartInfo: User_PartInfo;       //分区信息
    BkColor: Longword;             //背景颜色
    FontInfo: User_FontSet;        //字体设置
    chTitle: String;               //标题
    iDisplayType: Longword;        //显示格式：0－度 1－C
		End ;
		PUser_Temperature = ^User_Temperature;
		
		//“日期时间窗口”
		User_DateTime = Packed Record
    PartInfo: User_PartInfo;           //分区信息
    BkColor: Longword;                 //背景颜色
    FontInfo: User_FontSet;            //字体设置
    iDisplayType: Longword;            //显示风格
    chTitle: String;                   //添加显示文字
    bYearDisType: Longword;            //年份位数0 －4；1－2位
    bMulOrSingleLine: Longword;        //单行还是多行
    bYear: Longword;
    bMouth: Longword;
    bDay: Longword;
    bWeek: Longword;
    bHour: Longword;
    bMin: Longword;
    bSec: Longword;
		End;
		PUser_DateTime = ^User_DateTime;

		//“图文框”
		User_Bmp = Packed Record
     	PartInfo: User_PartInfo;       //分区信息
		End;
		PUser_Bmp = ^User_Bmp;
		
		//“单行文本框”
		User_SingleText = Packed Record
     chContent: String;            //显示内容
    PartInfo: User_PartInfo;       //分区信息
    BkColor: Longword;             //背景颜色
    FontInfo: User_FontSet ;       //字体设置
    MoveSet: User_MoveSet;         //动作方式设置
		End;
		PUser_SingleText = ^User_SingleText;

		//“文本框”
		User_Text = Packed Record
    chContent: String;                 //显示内容
    PartInfo: User_PartInfo ;          //分区信息
    BkColor: Longword;                 //背景颜色
    FontInfo: User_FontSet ;           //字体设置
    MoveSet: User_MoveSet;             //动作方式设置
		End;
		PUser_Text = ^User_Text;

   	//“RTF文件”
		User_RTF = Packed Record
    strFileName: String;                //显示内容
    PartInfo: User_PartInfo ;          //分区信息
    MoveSet: User_MoveSet;             //动作方式设置
		End;
		PUser_RTF = ^User_RTF;

//1、节目编辑函数组
//添加节目
Function User_AddProgram (CardNum: Longword;bWaitToEnd: Boolean;iPlayTime: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加图文区
Function User_AddBmpZone (CardNum: Longword;pBmp: PUser_Bmp; iProgramIndex: Longword): Longword ; Stdcall; External EQ2008_Dll;
Function User_AddBmp (CardNum: Longword;iBmpPartNum: Longword; hBitmap: Longword;pMoveSet: PUser_MoveSet; iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加时间区
Function User_AddTime (CardNum: Longword; pDateTime: PUser_DateTime;iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加倒计时区
Function User_AddTimeCount (CardNum: Longword;pTimeCount: PUser_Timer; iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加温度区
Function User_AddTemperature (CardNum: Longword;pTemperature: PUser_Temperature;iProgramIndex: Longword): Longword ; Stdcall; External EQ2008_Dll;

//添加文本区
Function User_AddText (CardNum: Longword;pText: PUser_Text; iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加文本区
Function User_AddRTF (CardNum: Longword;pRTF: PUser_RTF; iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;

//添加单行文本区
Function User_AddSingleText (CardNum: Longword; pSingleText: PUser_SingleText; iProgramIndex: Longword): Longword; Stdcall; External EQ2008_Dll;
                                            
//删除所有节目
Function User_DelAllProgram (CardNum: Longword): Boolean; Stdcall; External EQ2008_Dll;

//发送节目
Function User_SendToScreen (CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;

//2、实时更新数据函数组
//实时发送数据建立连接
Function User_RealtimeConnect(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//实时发送图片句柄
Function User_RealtimeSendData(CardNum: Longword; x: Longword; y: Longword; iWidth: Longword; iHeight: Longword; hBitmap: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//实时发送图片文件
Function User_RealtimeSendBmpData(CardNum: Longword; x: Longword; y: Longword; iWidth: Longword; iHeight: Longword; strFileName:String): Boolean ; Stdcall; External EQ2008_Dll;
//实时发送文本
Function User_RealtimeSendText(CardNum: Longword;x: Longword; y: Longword; iWidth: Longword; iHeight: Longword; strText: String; pFontInfo:PUser_FontSet): Boolean ; Stdcall; External EQ2008_Dll;
//实时发送断开连接
Function User_RealtimeDisConnect(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//实时发送清屏
Function User_RealtimeScreenClear(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;

//3、显示屏控制函数组
//开屏
Function User_OpenScreen(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//关屏
Function User_CloseScreen(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//校正时间
Function User_AdjustTime(CardNum: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//亮度调节
Function User_SetScreenLight(CardNum: Longword; iLightDegreen: Longword): Boolean ; Stdcall; External EQ2008_Dll;
//重新加载配置文件INI
Function User_ReloadIniFile(strEQ2008_Dll_Set_Path: string): Boolean ; Stdcall; External EQ2008_Dll;

implementation

end.

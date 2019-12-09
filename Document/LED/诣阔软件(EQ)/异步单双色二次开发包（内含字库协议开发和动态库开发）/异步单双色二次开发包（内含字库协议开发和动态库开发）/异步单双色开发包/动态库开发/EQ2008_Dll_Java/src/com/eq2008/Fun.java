package com.eq2008;

import com.sun.jna.win32.StdCallLibrary;
import com.sun.jna.win32.W32APIOptions;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.platform.win32.WinDef.HBITMAP;
import com.sun.jna.platform.win32.WinDef.HINSTANCE;
import com.sun.jna.platform.win32.WinNT.HANDLE;  
import com.sun.jna.Structure;

//EQ2008动态库接口定义
interface DllLibrary extends StdCallLibrary {
	// 1、节目操作函数组===================================================
	// 添加节目
	int User_AddProgram(int CardNum, boolean bWaitToEnd, int iPlayTime);

	// 删除所有节目
	boolean User_DelAllProgram(int CardNum);

	// 添加图文区
	int User_AddBmpZone(int CardNum, User_Bmp pBmp, int iProgramIndex);

	boolean User_AddBmp(int CardNum, int iBmpPartNum, HANDLE hBitmap,
			User_MoveSet pMoveSet, int iProgramIndex);

	boolean User_AddBmpFile(int CardNum, int iBmpPartNum,
			String strFileName, User_MoveSet pMoveSet, int iProgramIndex);

	// 添加文本区
	int User_AddText(int CardNum, User_Text pText, int iProgramIndex);

	// 添加RTF区
	int User_AddRTF(int CardNum, User_RTF pRTF, int iProgramIndex);

	// 添加单行文本区
	int User_AddSingleText(int CardNum, User_SingleText pSingleText,
			int iProgramIndex);

	// 添加时间区
	int User_AddTime(int CardNum, User_DateTime pDateTime, int iProgramIndex);

	// 添加计时区
	int User_AddTimeCount(int CardNum, User_Timer pTimeCount,
			int iProgramIndex);

	// 添加温度区
	int User_AddTemperature(int CardNum, User_Temperature pTemperature,
			int iProgramIndex);

	// 发送数据
	boolean User_SendToScreen(int CardNum);

	// 2、实时更新函数组=================================================
	// 实时发送数据建立连接
	boolean User_RealtimeConnect(int CardNum);

	// 实时发送图片句柄
	boolean User_RealtimeSendData(int CardNum, int x, int y, int iWidth,
			int iHeight, HANDLE hBitmap);

	// 实时发送图片文件
	boolean User_RealtimeSendBmpData(int CardNum, int x, int y, int iWidth,
			int iHeight, String strFileName);

	// 实时发送文本
	boolean User_RealtimeSendText(int CardNum, int x, int y, int iWidth,
			int iHeight, String strText, User_FontSet pFontInfo);

	// 实时发送断开连接
	boolean User_RealtimeDisConnect(int CardNum);

	// 实时发送清屏
	boolean User_RealtimeScreenClear(int CardNum);

	// 3、显示屏控制函数组==============================================
	// 开屏
	boolean User_OpenScreen(int CardNum);

	// 关屏
	boolean User_CloseScreen(int CardNum);

	// 校正时间
	boolean User_AdjustTime(int CardNum);

	// 亮度调节
	boolean User_SetScreenLight(int CardNum, int iLightDegreen);

	// 重新加载配置文件
	void User_ReloadIniFile(String strEQ2008_Dll_Set_Path);

	/**
	 * 节目区域参数
	 */
	public class User_PartInfo extends Structure {

		public int iX; // 窗口的起点X
		public int iY; // 窗口的起点Y
		public int iWidth; // 窗体的宽度
		public int iHeight; // 窗体的高度
		public int iFrameMode; // 边框的样式
		public int FrameColor; // 边框颜色
	}

	/**
	 * 字体参数
	 */
	public class User_FontSet extends Structure {

		public String strFontName; // 字体的名称
		public int iFontSize; // 字体的大小
		public boolean bFontBold; // 字体是否加粗
		public boolean bFontItaic; // 字体是否是斜体
		public boolean bFontUnderline; // 字体是否带下划线
		public int colorFont; // 字体的颜色
		public int iAlignStyle; // 对齐方式
		// 0－ 左对齐
		// 1－居中
		// 2－右对齐
		public int iVAlignerStyle; // 上下对齐方式
		// 0-顶对齐
		// 1-上下居中
		// 2-底对齐
		public int iRowSpace; // 行间距
	}

	/**
	 * 计时窗口
	 */
	public class User_Timer extends Structure {

		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public int BkColor; // 背景颜色
		public User_FontSet FontInfo = new User_FontSet(); // 字体设置
		public int ReachTimeYear; // 到达年
		public int ReachTimeMonth; // 到达月
		public int ReachTimeDay; // 到达日
		public int ReachTimeHour; // 到达时
		public int ReachTimeMinute; // 到达分
		public int ReachTimeSecond; // 到达秒
		public boolean bDay; // 是否显示天 0－不显示 1－显示
		public boolean bHour; // 是否显示小时
		public boolean bMin; // 是否显示分钟
		public boolean bSec; // 是否显示秒
		public boolean bMulOrSingleLine; // 单行还是多行
		public String chTitle; // 添加显示文字
	}

	/**
	 * 温度窗口
	 */
	public class User_Temperature extends Structure {

		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public int BkColor; // 背景颜色
		public User_FontSet FontInfo = new User_FontSet(); // 字体设置
		public String chTitle; // 标题
		public int DisplayType; // 显示格式：0－度 1－C
	}

	/**
	 * 日期时间窗口
	 */
	public class User_DateTime extends Structure {

		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public int BkColor; // 背景颜色
		public User_FontSet FontInfo = new User_FontSet(); // 字体设置
		public int iDisplayType; // 显示风格
		public String chTitle; // 添加显示文字
		public int bYearDisType; // 年份位数0 －4；1－2位
		public int bMulOrSingleLine; // 单行还是多行
		public int bYear; // 是否显示年
		public int bMouth; // 是否显示月
		public int bDay; // 是否显示日
		public int bWeek; // 是否显示星期
		public int bHour; // 是否显示小时
		public int bMin; // 是否显示分钟
		public int bSec; // 是否显示秒钟
	}

	/**
	 * 图文框
	 */
	public class User_Bmp extends Structure {
		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
	}

	/**
	 * 单行文本框
	 */
	public class User_SingleText extends Structure {

		public String chContent; // 显示内容
		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public int BkColor; // 背景颜色
		public User_FontSet FontInfo = new User_FontSet(); // 字体设置
		public User_MoveSet MoveSet = new User_MoveSet(); // 动作方式设置

	}

	/**
	 * 文本框
	 */
	public class User_Text extends Structure {

		public String chContent; // 显示内容
		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public int BkColor; // 背景颜色
		public User_FontSet FontInfo = new User_FontSet(); // 字体设置
		public User_MoveSet MoveSet = new User_MoveSet(); // 动作方式设置

	}

	/**
	 * RTF文件
	 */
	public class User_RTF extends Structure {

		public String strFileName; // RTF文件名
		public User_PartInfo PartInfo = new User_PartInfo(); // 分区信息
		public User_MoveSet MoveSet = new User_MoveSet(); // 动作方式设置

	}

	/**
	 * 动画方式设置
	 */
	public class User_MoveSet extends Structure {

		public int iActionType; // 节目变换方式
		public int iActionSpeed; // 节目的播放速度
		public boolean bClear; // 是否需要清除背景
		public int iHoldTime; // 在屏幕上停留的时间
		public int iClearSpeed; // 清除显示屏的速度
		public int iClearActionType; // 节目清除的变换方式
		public int iFrameTime; // 每帧时间

	}

	boolean User_RealtimeSendData(int m_iCardNum, int x, int y, int iWidth,
			int iHeight, Pointer pointer);
}

//调用图片句柄接口
interface User32 extends com.sun.jna.platform.win32.User32 {  
    User32 INSTANCE = (User32) Native.loadLibrary(User32.class,W32APIOptions.ASCII_OPTIONS);  
    
    final int IMAGE_BITMAP=0;
    final int LR_LOADFROMFILE=0x00000010;
    HANDLE LoadImage(HINSTANCE hinst,String  lpszName,int uType,int cxDesired,int cyDesired,int fuLoad);
} 

interface GDI32 extends com.sun.jna.platform.win32.GDI32 {  
	GDI32 INSTANCE = (GDI32) Native.loadLibrary(GDI32.class,W32APIOptions.ASCII_OPTIONS);  
	boolean DeleteObject(HANDLE handle);} 

//JNA 方法调用动态库
public class Fun {
	// 全局参数定义
	static int m_iCardNum = 1;
	static int m_iProgramIndex = -1;
	static int m_iProgramCount = 0;
	static DllLibrary m_DllLibrary=null;
	static String m_strUserPath = System.getProperty("user.dir"); 
    private static final User32 USER = User32.INSTANCE;
    private static final GDI32 GDI = GDI32.INSTANCE;   
    
	// 主函数
	public static void main(String[] args) {
		// 1、加载动态库EQ2008_Dll.dll
		String strDllFileName = m_strUserPath + "\\res\\EQ2008_Dll";
		String strEQ2008_Dll_Set_Path = m_strUserPath + "\\res\\EQ2008_Dll_Set.ini";
		m_DllLibrary = (DllLibrary) Native.loadLibrary(strDllFileName,DllLibrary.class);
		m_DllLibrary.User_ReloadIniFile(strEQ2008_Dll_Set_Path);
		
		// 2、节目操作函数演示
		////(1)、删除所有节目
		//OnDelAllProgram();
		////(2)、添加节目(可以添加多个节目)
		for(int i=0;i<1;i++)
		{
			OnAddProgram();//添加节目
			////(3)、添加分区窗口到节目,每个节目可以添加多个分区，但分区位置不能重叠
			OnAddText(); 		//文本窗操作演示
			//OnAddRTF(); 		//RTF窗操作演示
			//OnAddSingleText();//单行文本窗操作演示
			//OnAddbmp(); 		//图片窗操作演示
			//OnAddTime(); 		//时间窗操作演示
			//OnAddTimeCount(); //计时窗操作演示
			//OnAddTemperature();//温度窗操作演示	
		}
		////(3)、发送节目到显示屏
		OnSendToScreen(); 		
	
		
		// 3、实时更新数据
		////(0)、清空控制卡原有节目，只需要清空一次
		//OnRealtimeScreenClear();
		////(1)、建立连接
		/*if(OnRealtimeConnect())
		{
			////(2)、发送数据
			//OnRealtimeSendData(); 	//图片句柄
			OnRealtimeSendBmpData();//图片路径
			//OnRealtimeSendText();	//文本信息
			////(3)、断开连接
			OnRealtimeDisConnect(); 
		}
		*/
		
		// 4、显示屏控制函数		
		//OnAjusttime();  //校准显示屏时间
		//OnOpenScreen(); //打开显示屏
		//OnCloseScreen();//关闭显示屏
	}

	//函数：添加节目索引
	public static void OnAddProgram() 
	{
		m_iProgramIndex= m_DllLibrary.User_AddProgram(m_iCardNum,false,10);
		m_iProgramCount++;
		System.out.println("添加节目"+m_iProgramCount);
	}

	//函数：添加文本窗
	public static void OnAddText() 
	{
		DllLibrary.User_Text  Text = new DllLibrary.User_Text();

		Text.BkColor = 0;
		Text.chContent = "欢迎使用EQ异步控制卡！";

		Text.PartInfo.FrameColor = 0;
		Text.PartInfo.iFrameMode = 0;
		Text.PartInfo.iHeight = 32;
		Text.PartInfo.iWidth = 64;
		Text.PartInfo.iX = 0;
		Text.PartInfo.iY = 0;

		Text.FontInfo.bFontBold = false;
		Text.FontInfo.bFontItaic= false;
		Text.FontInfo.bFontUnderline = false;
		Text.FontInfo.colorFont = 0xFFFF;
		Text.FontInfo.iFontSize = 12;
		Text.FontInfo.strFontName = "";
		Text.FontInfo.iAlignStyle = 1;
		Text.FontInfo.iVAlignerStyle = 0;
		Text.FontInfo.iRowSpace = 0;

		Text.MoveSet.bClear = false;
		Text.MoveSet.iActionSpeed = 4;
		Text.MoveSet.iActionType = 0;
		Text.MoveSet.iHoldTime = 10;
		Text.MoveSet.iClearActionType	= 0;
		Text.MoveSet.iClearSpeed		= 4;
		Text.MoveSet.iFrameTime			= 10;

		if(-1 == m_DllLibrary.User_AddText(m_iCardNum,Text,m_iProgramIndex))
		{
			System.out.println("添加文本失败！");
		}
		else
		{
			System.out.println("添加文本成功！");
		}
	}

	//函数：添加RTF文件
	public static void OnAddRTF() 
	{
		DllLibrary.User_RTF  RTF = new DllLibrary.User_RTF();
		
		String strFileName = m_strUserPath + "\\res\\EQ2008_RTF.rtf";
		RTF.strFileName = strFileName;

		RTF.PartInfo.FrameColor		= 0;
		RTF.PartInfo.iFrameMode		= 0;
		RTF.PartInfo.iHeight		= 32;
		RTF.PartInfo.iWidth			= 64;
		RTF.PartInfo.iX				= 0;
		RTF.PartInfo.iY				= 0;

		RTF.MoveSet.bClear			= false;
		RTF.MoveSet.iActionSpeed	= 4;
		RTF.MoveSet.iActionType		= 0;
		RTF.MoveSet.iHoldTime		= 10;
		RTF.MoveSet.iClearActionType= 0;
		RTF.MoveSet.iClearSpeed		= 4;
		RTF.MoveSet.iFrameTime		= 10;

		if(-1 == m_DllLibrary.User_AddRTF(m_iCardNum,RTF,m_iProgramIndex))
		{
			System.out.println("添加rtf文件失败！");
		}
		else
		{
			System.out.println("添加rtf文件成功！");
		}
	}

	//函数：添加单行文本
	public static void OnAddSingleText() 
	{
		DllLibrary.User_SingleText  SingleText = new DllLibrary.User_SingleText();

		SingleText.BkColor		= 0;
		SingleText.chContent	= "欢迎使用EQ异步控制卡！";
		SingleText.PartInfo.iFrameMode	= 0;
		SingleText.PartInfo.iHeight		= 32;
		SingleText.PartInfo.iWidth		= 64;
		SingleText.PartInfo.iX = 0;
		SingleText.PartInfo.iY = 0;
		SingleText.FontInfo.bFontBold		= false;
		SingleText.FontInfo.bFontItaic		= false;
		SingleText.FontInfo.bFontUnderline	= false;
		SingleText.FontInfo.colorFont		= 0xFF;
		SingleText.FontInfo.iFontSize		= 12;
		SingleText.PartInfo.FrameColor		= 0;
		SingleText.FontInfo.strFontName		= "";
		SingleText.MoveSet.bClear			= false;
		SingleText.MoveSet.iActionSpeed		= 8;
		SingleText.MoveSet.iActionType		= 2;
		SingleText.MoveSet.iHoldTime		= 0;
		SingleText.MoveSet.iClearActionType	= 0;
		SingleText.MoveSet.iClearSpeed		= 4;
		SingleText.MoveSet.iFrameTime		= 20;
		if(-1 == m_DllLibrary.User_AddSingleText(m_iCardNum,SingleText,m_iProgramIndex))
		{
			System.out.println("添加单行文本失败！");
		}
		else
		{
			System.out.println("添加单行文本成功！");
		}
	}

	//函数：添加图片
	public static void OnAddbmp() 
	{
		DllLibrary.User_Bmp		BmpZone = new DllLibrary.User_Bmp();
		DllLibrary.User_MoveSet	MoveSet = new DllLibrary.User_MoveSet();
		int iBMPZoneNum;

		BmpZone.PartInfo.iX		 = 0;
		BmpZone.PartInfo.iY		 = 0;
		BmpZone.PartInfo.iHeight = 32;
		BmpZone.PartInfo.iWidth  = 64;
		BmpZone.PartInfo.FrameColor = 0xFF00;
		BmpZone.PartInfo.iFrameMode = 1;

		MoveSet.bClear			= true;
		MoveSet.iActionSpeed	= 4;
		MoveSet.iActionType		= 0;
		MoveSet.iHoldTime		= 50;
		MoveSet.iClearActionType= 0;
		MoveSet.iClearSpeed		= 4;
		MoveSet.iFrameTime		= 10;

		iBMPZoneNum = m_DllLibrary.User_AddBmpZone(m_iCardNum,BmpZone,m_iProgramIndex);

		//通过图片路径添加两张图片
		if(m_DllLibrary.User_AddBmpFile(m_iCardNum,iBMPZoneNum,m_strUserPath +"\\res\\BMP1.bmp",MoveSet,m_iProgramIndex))
		{
			System.out.println("添加图片路径1成功！");
		}
		else
		{
			System.out.println("添加图片路径1失败！");
		}		

		// 通过图片句柄添加图片
		HANDLE hBitmap = USER.LoadImage(null,           // 模块实例句柄
							            m_strUserPath + "\\res\\BMP1.bmp",   // 位图路径
							            USER.IMAGE_BITMAP,  // 位图类型
							            64,   				// 指定图片宽
							            32,  				// 指定图片高
							            USER.LR_LOADFROMFILE );
		if(hBitmap != null)
		{
			if(m_DllLibrary.User_AddBmp(m_iCardNum,iBMPZoneNum,hBitmap,MoveSet,m_iProgramIndex))
			{
				System.out.println("添加图片句柄2成功！");
			}
			else
			{
				System.out.println("添加图片句柄2失败！");
			}	
			GDI.DeleteObject(hBitmap);
		}
		else
		{
			System.out.println("添加图片句柄2失败！");
		}
	}

	//函数：添加时间
	public static void OnAddTime() 
	{
	  	DllLibrary.User_DateTime  DateTime = new DllLibrary.User_DateTime();
	  
	  	DateTime.bDay = 1;
	  	DateTime.bHour = 1;
	  	DateTime.BkColor = 1;
	  	DateTime.bMin = 1;
	  	DateTime.bMouth = 1;
	  	DateTime.bMulOrSingleLine = 1;
	  	DateTime.bSec =1;
	  	DateTime.bWeek = 1;
	  	DateTime.bYear = 1;
	  	DateTime.bYearDisType = 1;
	  	DateTime.chTitle = "北京";
	  
	  	DateTime.PartInfo.iFrameMode = 1;
	  	DateTime.iDisplayType = 4;
	  	DateTime.PartInfo.FrameColor =0xFFFF;
	  	DateTime.PartInfo.iHeight = 64;
	  	DateTime.PartInfo.iWidth = 64;
	  	DateTime.PartInfo.iX=0;
	  	DateTime.PartInfo.iY=0;
	  	DateTime.FontInfo.bFontBold=false;
	  	DateTime.FontInfo.bFontItaic=false;
	  	DateTime.FontInfo.bFontUnderline=false;
	  	DateTime.FontInfo.colorFont = 0xFFFF;
	  	DateTime.FontInfo.iAlignStyle = 1;
	  	DateTime.FontInfo.iFontSize = 12;
	  	DateTime.FontInfo.strFontName = "";
	  
	  	if(-1 == m_DllLibrary.User_AddTime(m_iCardNum,DateTime,m_iProgramIndex))
		{
			System.out.println("添加时间失败！");
		}
		else
		{
			System.out.println("添加时间成功！");
		}
	}

	//函数：添加计时
	public static void OnAddTimeCount() 
	{
		DllLibrary.User_Timer  TimeCount = new DllLibrary.User_Timer();

		TimeCount.bDay = true;
		TimeCount.bHour = false;
		TimeCount.BkColor =0;
		TimeCount.bMin = false;
		TimeCount.bMulOrSingleLine =true;
		TimeCount.bSec =false;
		TimeCount.chTitle = "距离五一";
		TimeCount.FontInfo.bFontBold=false;
		TimeCount.FontInfo.bFontItaic=false;
		TimeCount.FontInfo.bFontUnderline=false;
		TimeCount.FontInfo.colorFont=0xFFFF;
		TimeCount.FontInfo.iAlignStyle=2;
		TimeCount.FontInfo.iFontSize=12;
		TimeCount.FontInfo.strFontName="";
		TimeCount.PartInfo.FrameColor = 0xFF00;
		TimeCount.PartInfo.iFrameMode =0;
		TimeCount.PartInfo.iHeight = 32;
		TimeCount.PartInfo.iWidth = 64;
		TimeCount.PartInfo.iX =0;
		TimeCount.PartInfo.iY=0;
		TimeCount.ReachTimeYear=2015;
		TimeCount.ReachTimeMonth=5;
		TimeCount.ReachTimeDay= 1;
		TimeCount.ReachTimeHour=10;
		TimeCount.ReachTimeMinute=0;
		TimeCount.ReachTimeSecond=0;

		if(-1 == m_DllLibrary.User_AddTimeCount(m_iCardNum,TimeCount,m_iProgramIndex))
		{
			System.out.println("添加计时失败！");
		}
		else
		{
			System.out.println("添加计时成功！");
		}
	}

	//函数：添加温度
	public static void OnAddTemperature() 
	{
		DllLibrary.User_Temperature  Temperature = new DllLibrary.User_Temperature();

		Temperature.BkColor=0;
		Temperature.chTitle="";
		Temperature.DisplayType=0;
		Temperature.PartInfo.FrameColor=0xFF00;
		Temperature.PartInfo.iFrameMode=1;
		Temperature.PartInfo.iHeight=32;
		Temperature.PartInfo.iWidth=64;
		Temperature.PartInfo.iX=0;
		Temperature.PartInfo.iY=0;
		Temperature.FontInfo.bFontBold=false;
		Temperature.FontInfo.bFontItaic=false;
		Temperature.FontInfo.bFontUnderline=false;
		Temperature.FontInfo.colorFont=0xFFFF;
		Temperature.FontInfo.iAlignStyle=0;
		Temperature.FontInfo.iFontSize=12;
		Temperature.FontInfo.strFontName="宋体";

		if(-1 == m_DllLibrary.User_AddTemperature(m_iCardNum,Temperature,m_iProgramIndex))
		{
			System.out.println("添加温度失败！");
		}
		else
		{
			System.out.println("添加温度成功！");
		}
	}

	//函数：删除所有节目
	public static void OnDelAllProgram() 
	{
		if(!m_DllLibrary.User_DelAllProgram(m_iCardNum))
		{
			System.out.println("删除节目失败！");
		}
		else
		{
			//提示信息
			m_iProgramCount=0;
			System.out.println("请首先添加节目！");
		}
	}

	//函数：发送数据到显示屏
	public static void OnSendToScreen() 
	{
		if(!m_DllLibrary.User_SendToScreen(m_iCardNum))
		{
			System.out.println("数据发送失败！");
		}
		else
		{
			System.out.println("数据发送成功！");
		}

	}

	//函数：校正时间
	public static void OnAjusttime() 
	{
		if(!m_DllLibrary.User_AdjustTime(m_iCardNum))
		{
			System.out.println("校准时间失败！");
		}
		else
		{
			System.out.println("校准时间成功！");
		}
	}

	//函数：打开显示屏
	public static void OnOpenScreen() 
	{
		if(!m_DllLibrary.User_OpenScreen(m_iCardNum))
		{
			System.out.println("打开显示屏失败！");
		}
		else
		{
			System.out.println("打开显示屏成功！");
		}
	}

	//函数：关闭显示屏
	public static void OnCloseScreen() 
	{
		if(!m_DllLibrary.User_CloseScreen(m_iCardNum))
		{
			System.out.println("关闭显示屏失败！");
		}
		else
		{
			System.out.println("关闭显示屏成功！");
		}
	}

	//函数：实时发送数据，建立连接
	//日期：2008-04-30
	public static boolean OnRealtimeConnect() 
	{
		if(!m_DllLibrary.User_RealtimeConnect(m_iCardNum))
		{
			System.out.println("实时发送数据建立连接失败！");
			return false;
		}
		else
		{
			System.out.println("实时发送数据建立连接成功！");
			return true;
		}
	}

	//函数：实时发送数据，发送数据
	//日期：2008-04-30
	public static void OnRealtimeSendData() 
	{
		// 通过图片句柄添加图片
		HANDLE hBitmap = USER.LoadImage(null,           	// 模块实例句柄
							            m_strUserPath + "\\res\\BMP1.bmp",// 位图路径
							            USER.IMAGE_BITMAP,	// 位图类型
							            64,   				// 指定图片宽
							            32,  				// 指定图片高
							            USER.LR_LOADFROMFILE);// 从路径处加载图片
		if(hBitmap != null)
		{
			if(false == m_DllLibrary.User_RealtimeSendData(m_iCardNum,0,0,64,32,hBitmap))
			{
				System.out.println("发送实时图片句柄失败！");
			}
			else
			{
				System.out.println("发送实时图片句柄成功！");
			}
			GDI.DeleteObject(hBitmap);
		}
		else
		{
			System.out.println("发送实时图片句柄失败！");					
		}
	}

	//函数：实时发送图片路径
	//日期：2015-09-21
	public static void OnRealtimeSendBmpData()
	{
		if(false == m_DllLibrary.User_RealtimeSendBmpData(m_iCardNum,0,0,64,32,m_strUserPath + "\\res\\BMP2.bmp"))
		{
			System.out.println("发送实时图片路径失败！");
		}
		else
		{
			System.out.println("发送实时图片路径成功！");
		}
	}

	//函数：实时发送文本内容
	//日期：2015-09-21
	public static void OnRealtimeSendText()
	{
		String   strText = "实时发送文本测试!";
		DllLibrary.User_FontSet FontInfo = new DllLibrary.User_FontSet();

		FontInfo.bFontBold = false;
		FontInfo.bFontItaic= false;
		FontInfo.bFontUnderline = false;
		FontInfo.colorFont = 0xFFFF;
		FontInfo.iFontSize = 12;
		FontInfo.strFontName = "宋体";
		FontInfo.iAlignStyle = 0;
		FontInfo.iVAlignerStyle = 0;
		FontInfo.iRowSpace = 0;

		if(!m_DllLibrary.User_RealtimeSendText(m_iCardNum,0,0,64,32,strText,FontInfo))
		{
			System.out.println("发送实时文本失败！");
		}
		else
		{
			System.out.println("发送实时文本成功！");
		}
	}

	//函数：实时发送数据，断开连接
	//日期：2008-04-30
	public static void OnRealtimeDisConnect() 
	{
		if(!m_DllLibrary.User_RealtimeDisConnect(m_iCardNum))
		{
			System.out.println("实时发送数据断开连接失败！");
		}
		else
		{
			System.out.println("实时发送数据断开连接成功！");
		}
	}


	//函数：清空控制卡节目
	//日期：2015-09-21
	public static void OnRealtimeScreenClear()
	{
		if(!m_DllLibrary.User_RealtimeScreenClear(m_iCardNum))
		{
			System.out.println("清空控制卡节目失败！");
		}
		else
		{
			System.out.println("清空控制卡节目成功！");
		}
	}
}

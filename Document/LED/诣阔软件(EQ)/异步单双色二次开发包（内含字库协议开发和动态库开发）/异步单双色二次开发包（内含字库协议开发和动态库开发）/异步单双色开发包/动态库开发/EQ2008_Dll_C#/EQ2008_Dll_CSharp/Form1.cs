using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using EQ2008_DataStruct;

namespace EQ2008_Dll_CSharp
{
    public partial class Form1 : Form
    {
        //==========================1、节目操作函数======================//
        //添加节目
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddProgram(int CardNum, Boolean bWaitToEnd, int iPlayTime);
        //删除所有节目
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_DelAllProgram(int CardNum);

        //添加单行文本区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddSingleText(int CardNum, ref User_SingleText pSingleText, int iProgramIndex);
        //添加文本区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddText(int CardNum, ref User_Text pText, int iProgramIndex);
        //添加时间区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddTime(int CardNum, ref User_DateTime pdateTime, int iProgramIndex);
        //添加图文区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddBmpZone(int CardNum,ref User_Bmp pBmp,int iProgramIndex);
        //指定图像句柄添加图片
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern bool User_AddBmp(int CardNum,int iBmpPartNum,IntPtr hBitmap,ref User_MoveSet pMoveSet,int iProgramIndex);
        //指定图像路径添加图片
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern bool User_AddBmpFile(int CardNum, int iBmpPartNum, string strFileName, ref User_MoveSet pMoveSet, int iProgramIndex);

        //添加RTF区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddRTF(int CardNum, ref User_RTF pRTF, int iProgramIndex);
        //添加计时区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddTimeCount(int CardNum,ref User_Timer pTimeCount,int iProgramIndex);
        //添加温度区
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern int User_AddTemperature(int CardNum, ref User_Temperature pTemperature, int iProgramIndex);

        //发送数据
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_SendToScreen(int CardNum);
        //====================================================================//       

        //=======================2、实时发送数据（高频率发送）=================//
        //实时建立连接
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeConnect(int CardNum);
        //实时发送图片数据
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeSendData(int CardNum,int x,int y,int iWidth,int iHeight,IntPtr hBitmap);
        //实时发送图片文件
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeSendBmpData(int CardNum, int x, int y, int iWidth, int iHeight, string strFileName);
        //实时发送文本
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeSendText(int CardNum, int x, int y, int iWidth, int iHeight, string strText, ref User_FontSet pFontInfo);	
        //实时关闭连接
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeDisConnect(int CardNum);
        //实时发送清屏
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_RealtimeScreenClear(int CardNum);
        //====================================================================//

        //==========================3、显示屏控制函数组=======================//
        //校正时间
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_AdjustTime(int CardNum);
        //开屏
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_OpenScreen(int CardNum);
        //关屏
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_CloseScreen(int CardNum);
        //亮度调节
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern Boolean User_SetScreenLight(int CardNum,int iLightDegreen);
        //Reload参数文件
        [DllImport("EQ2008_Dll.dll", CharSet = CharSet.Ansi)]
        public static extern void User_ReloadIniFile(string strEQ2008_Dll_Set_Path);
        //====================================================================//

        [System.Runtime.InteropServices.DllImport("gdi32.dll")]
        public static extern bool DeleteObject(IntPtr hObject);

        public static int g_iCardNum    =   1;      //控制卡地址
        public static int g_iGreen      =   0xFF00; //绿色
        public static int g_iYellow     =   0xFFFF; //黄色
        public static int g_iRed        =   0x00FF; //红色
        public static int g_iProgramIndex   =   0;
        public static int g_iProgramCount   =   0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.comboBox1.SelectedIndex = 0;
            this.ZoneX.Text = "0";
            this.ZoneY.Text = "0";
            this.ZoneW.Text = "64";
            this.ZoneH.Text = "32";
        }

        //选择控制卡地址
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            g_iCardNum = comboBox1.SelectedIndex + 1;
        }

        //添加节目
        private void button1_Click(object sender, EventArgs e)
        {
            g_iProgramIndex = User_AddProgram(g_iCardNum, false, 10);

            //提示信息
            string str = "当前节目号：";
            g_iProgramCount++;
            string strProgramCount = Convert.ToString(g_iProgramCount);
            str = str + strProgramCount;
            this.label1.Text = str;
        }

        //删除所有节目
        private void button2_Click(object sender, EventArgs e)
        {
            if (User_DelAllProgram(g_iCardNum) == false)
            {
                MessageBox.Show("删除节目失败！");
            }
            else
            {
                //提示信息
                g_iProgramCount = 0;
                this.label1.Text = "请先添加节目！";
            }
        }

        //添加文本
        private void button3_Click(object sender, EventArgs e)
        {
            User_Text Text              =   new User_Text();

            Text.BkColor                =   0;
            Text.chContent              =   ZoneText.Text.Replace("\\n","\n");

            Text.PartInfo.FrameColor    =   0;
            Text.PartInfo.iFrameMode    =   0;
            Text.PartInfo.iHeight       =   32;
            Text.PartInfo.iWidth        =   64;
            Text.PartInfo.iX            =   0;
            Text.PartInfo.iY            =   0;

            Text.FontInfo.bFontBold     =   false;
            Text.FontInfo.bFontItaic    =   false;
            Text.FontInfo.bFontUnderline=   false;
            Text.FontInfo.colorFont = g_iYellow;
            Text.FontInfo.iFontSize     =   12;
            Text.FontInfo.strFontName   =   "宋体";
            Text.FontInfo.iAlignStyle   =   0;
            Text.FontInfo.iVAlignerStyle=   0;
            Text.FontInfo.iRowSpace     =   2;

            Text.MoveSet.bClear         =   false;
            Text.MoveSet.iActionSpeed   =   5;
            Text.MoveSet.iActionType    =   0;
            Text.MoveSet.iHoldTime      =   20;
            Text.MoveSet.iClearActionType = 0;
            Text.MoveSet.iClearSpeed    = 4;
            Text.MoveSet.iFrameTime     = 20;

            if (-1 == User_AddText(g_iCardNum, ref Text, g_iProgramIndex))
            {
                MessageBox.Show("添加文本失败！");
            }
            else
            {
                MessageBox.Show("添加文本成功！");
            }
        }

        //添加rtf文件
        private void button4_Click(object sender, EventArgs e)
        {
	        User_RTF    RTF             =   new User_RTF();

            RTF.strFileName             =   ".\\EQ2008_RTF.rtf";

	        RTF.PartInfo.FrameColor		=   0;
	        RTF.PartInfo.iFrameMode		=   0;
	        RTF.PartInfo.iHeight		=   32;
	        RTF.PartInfo.iWidth			=   64;
	        RTF.PartInfo.iX				=   0;
	        RTF.PartInfo.iY				=   0;

	        RTF.MoveSet.bClear			=   false;
	        RTF.MoveSet.iActionSpeed	=   0;
	        RTF.MoveSet.iActionType		=   20;
	        RTF.MoveSet.iHoldTime		=   20;
	        RTF.MoveSet.iClearActionType=   0;
	        RTF.MoveSet.iClearSpeed		=   4;
	        RTF.MoveSet.iFrameTime		=   20;

            if (-1 == User_AddRTF(g_iCardNum, ref RTF, g_iProgramIndex))
	        {
                MessageBox.Show("添加rtf文件失败！");
	        }
	        else
	        {
                MessageBox.Show("添加rtf文件成功！");
	        }
        }

        //添加单行文本
        private void button5_Click(object sender, EventArgs e)
        {
            User_SingleText SingleText  =   new User_SingleText();

            SingleText.BkColor      = 0;
            SingleText.chContent    = "欢迎使用EQ2008型控制卡动态库!";
            SingleText.PartInfo.iFrameMode = 0;
            SingleText.PartInfo.iHeight = 32;
            SingleText.PartInfo.iWidth = 64;
            SingleText.PartInfo.iX = 0;
            SingleText.PartInfo.iY = 0;
            SingleText.FontInfo.bFontBold = false;
            SingleText.FontInfo.bFontItaic = false;
            SingleText.FontInfo.bFontUnderline = false;
            SingleText.FontInfo.colorFont = 0xFF;
            SingleText.FontInfo.iFontSize = 16;
            SingleText.PartInfo.FrameColor = 0xFF;
            SingleText.FontInfo.strFontName = "宋体";
            SingleText.MoveSet.bClear = false;
            SingleText.MoveSet.iActionSpeed = 6;
            SingleText.MoveSet.iActionType = 2;
            SingleText.MoveSet.iHoldTime = 20;
            SingleText.MoveSet.iClearActionType = 0;
            SingleText.MoveSet.iClearSpeed = 0;
            SingleText.MoveSet.iFrameTime = 20;

            if (-1 == User_AddSingleText(g_iCardNum, ref SingleText, g_iProgramIndex))
            {
                MessageBox.Show("添加单行文本失败！");
            }
            else
            {
                MessageBox.Show("添加单行文本成功！");
            }
        }

        //添加时间
        private void button6_Click(object sender, EventArgs e)
        {
            User_DateTime DateTime  =   new User_DateTime();

            DateTime.bDay = false;
            DateTime.bHour = true;
            DateTime.BkColor = 0;
            DateTime.bMin = true;
            DateTime.bMouth = false;
            DateTime.bMulOrSingleLine = false;
            DateTime.bSec = true;
            DateTime.bWeek = false;
            DateTime.bYear = false;
            DateTime.bYearDisType = false;
            DateTime.chTitle = "";

            DateTime.PartInfo.iFrameMode = 1;
            DateTime.iDisplayType = 1;

            DateTime.PartInfo.FrameColor = 0xFFFF;
            DateTime.PartInfo.iHeight = 32;
            DateTime.PartInfo.iWidth = 64;
            DateTime.PartInfo.iX = 0;
            DateTime.PartInfo.iY = 0;

            DateTime.FontInfo.bFontBold = false;
            DateTime.FontInfo.bFontItaic = false;
            DateTime.FontInfo.bFontUnderline = false;
            DateTime.FontInfo.colorFont = 0xFF;
            DateTime.FontInfo.iAlignStyle = 1;
            DateTime.FontInfo.iFontSize = 12;
            DateTime.FontInfo.strFontName = "宋体";

            if (-1 == User_AddTime(g_iCardNum, ref DateTime, g_iProgramIndex))
            {
                MessageBox.Show("添加时间失败！");
            }
            else
            {
                MessageBox.Show("添加时间成功！");
            }
        }

        //添加计时
        private void button7_Click(object sender, EventArgs e)
        {
            User_Timer TimeCount    =   new User_Timer();

            TimeCount.bDay = true;
            TimeCount.bHour = false;
            TimeCount.BkColor = 0;
            TimeCount.bMin = false;
            TimeCount.bMulOrSingleLine = true;
            TimeCount.bSec = false;
            TimeCount.chTitle = "距离国庆";

            TimeCount.FontInfo.bFontBold = false;
            TimeCount.FontInfo.bFontItaic = false;
            TimeCount.FontInfo.bFontUnderline = false;
            TimeCount.FontInfo.colorFont = 0xFFFF;
            TimeCount.FontInfo.iAlignStyle = 2;
            TimeCount.FontInfo.iFontSize = 16;
            TimeCount.FontInfo.strFontName = "宋体";

            TimeCount.PartInfo.FrameColor = 0xFF00;
            TimeCount.PartInfo.iFrameMode = 0;
            TimeCount.PartInfo.iHeight = 32;
            TimeCount.PartInfo.iWidth = 64;
            TimeCount.PartInfo.iX = 0;
            TimeCount.PartInfo.iY = 0;

            TimeCount.ReachTimeYear = 2010;
            TimeCount.ReachTimeMonth = 10;
            TimeCount.ReachTimeDay = 1;
            TimeCount.ReachTimeHour = 12;
            TimeCount.ReachTimeMinute = 0;
            TimeCount.ReachTimeSecond = 0;

            if (-1 == User_AddTimeCount(g_iCardNum, ref TimeCount, g_iProgramIndex))
            {
                MessageBox.Show("添加计时失败！");
            }
            else
            {
                MessageBox.Show("添加计时成功！");
            }
        }

        //添加温度
        private void button8_Click(object sender, EventArgs e)
        {
            User_Temperature Temperature    =   new User_Temperature();

            Temperature.BkColor = 0;
            Temperature.chTitle = "";
            Temperature.DisplayType = 0;

            Temperature.PartInfo.FrameColor = 0xFF00;
            Temperature.PartInfo.iFrameMode = 1;
            Temperature.PartInfo.iHeight = 32;
            Temperature.PartInfo.iWidth = 64;
            Temperature.PartInfo.iX = 0;
            Temperature.PartInfo.iY = 0;

            Temperature.FontInfo.bFontBold = false;
            Temperature.FontInfo.bFontItaic = false;
            Temperature.FontInfo.bFontUnderline = false;
            Temperature.FontInfo.colorFont = 0xFFFF;
            Temperature.FontInfo.iAlignStyle = 0;
            Temperature.FontInfo.iFontSize = 12;
            Temperature.FontInfo.strFontName = "宋体";

            if (-1 == User_AddTemperature(g_iCardNum, ref Temperature, g_iProgramIndex))
            {
                MessageBox.Show("添加温度失败！");
            }
            else
            {
                MessageBox.Show("添加温度成功！");
            }
        }

        //添加图片
        private void button9_Click(object sender, EventArgs e)
        {
            User_Bmp BmpZone    =   new User_Bmp();
            User_MoveSet MoveSet    =   new User_MoveSet();
            int iBMPZoneNum = 0;

            BmpZone.PartInfo.iX = 0;
            BmpZone.PartInfo.iY = 0;
            BmpZone.PartInfo.iHeight = 32;
            BmpZone.PartInfo.iWidth = 64;
            BmpZone.PartInfo.FrameColor = 0xFF00;
            BmpZone.PartInfo.iFrameMode = 1;

            MoveSet.bClear = true ;
            MoveSet.iActionSpeed = 4;
            MoveSet.iActionType = 0;
            MoveSet.iHoldTime = 50;
            MoveSet.iClearActionType = 0;
            MoveSet.iClearSpeed = 4;
            MoveSet.iFrameTime = 20;

            iBMPZoneNum = User_AddBmpZone(g_iCardNum, ref BmpZone, g_iProgramIndex);

            string strBmpFile = ".\\风景06.bmp";
            if (false == User_AddBmpFile(g_iCardNum, iBMPZoneNum, strBmpFile, ref MoveSet, g_iProgramIndex))
            {
                MessageBox.Show("添加指定路径图片失败！");
            }
            else
            {
                MessageBox.Show("添加指定路径图片成功！");
            }

            Bitmap bmp  = new Bitmap(pictureBox2.Image,BmpZone.PartInfo.iWidth ,BmpZone.PartInfo.iHeight);
            IntPtr hBitmap = bmp.GetHbitmap();

            if (false == User_AddBmp(g_iCardNum, iBMPZoneNum, hBitmap, ref MoveSet, g_iProgramIndex))
            {
                MessageBox.Show("添加图片句柄失败！");
            }
            else
            {
                MessageBox.Show("添加图片句柄成功！");

            }
            DeleteObject(hBitmap);
            bmp.Dispose();
        }

        //发送数据
        private void button10_Click(object sender, EventArgs e)
        {
            if (User_SendToScreen(g_iCardNum) == false)
            {
                MessageBox.Show("发送节目失败！");
            }
            else
            {
                MessageBox.Show("发送节目成功！");
            }
        }

        //打开显示屏
        private void button11_Click(object sender, EventArgs e)
        {
            if (User_OpenScreen(g_iCardNum) == false)
            {
                MessageBox.Show("打开显示屏失败！");
            }
            else
            {
                MessageBox.Show("打开显示屏成功！");
            }
        }

        //关闭显示屏
        private void button12_Click(object sender, EventArgs e)
        {
            if (User_CloseScreen(g_iCardNum) == false)
            {
                MessageBox.Show("关闭显示屏失败！");
            }
            else
            {
                MessageBox.Show("关闭显示屏成功！");
            }
        }

        //校准时间
        private void button13_Click(object sender, EventArgs e)
        {
            if (User_AdjustTime(g_iCardNum) == false)
            {
                MessageBox.Show("校准时间失败！");
            }
            else
            {
                MessageBox.Show("校准时间成功！");
            }
        }

        //实时连接
        private void button14_Click(object sender, EventArgs e)
        {
            if (!User_RealtimeConnect(g_iCardNum))
            {
                MessageBox.Show("连接实时通信失败！");
            }
            else
            {
                MessageBox.Show("连接实时通信成功！");
            }

        }

        //实时关闭
        private void button15_Click(object sender, EventArgs e)
        {
            if (!User_RealtimeDisConnect(g_iCardNum))
            {
                MessageBox.Show("关闭实时通信失败！");
            } 
            else
            {
                MessageBox.Show("关闭实时通信成功！");
            }

        }

        //实时发送图片数据
        private void button16_Click(object sender, EventArgs e)
        {
            int iX = Convert.ToInt32(ZoneX.Text);
            int iY = Convert.ToInt32(ZoneY.Text);
            int iW = Convert.ToInt32(ZoneW.Text);
            int iH = Convert.ToInt32(ZoneH.Text);

            Bitmap bmp1 = new Bitmap(pictureBox1.Image, 64, 32);
            IntPtr hBitmap1 = bmp1.GetHbitmap();
            if (!User_RealtimeSendData(g_iCardNum, iX,iY,iW,iH, hBitmap1))
            {
                MessageBox.Show("实时发送图片1失败");
            }
            else
            {
                MessageBox.Show("实时发送图片1成功");
            }
            DeleteObject(hBitmap1);
            bmp1.Dispose();



            Bitmap bmp2 = new Bitmap(pictureBox2.Image, 64, 32);
            IntPtr hBitmap2 = bmp2.GetHbitmap();
            if (!User_RealtimeSendData(g_iCardNum, iX, iY, iW, iH, hBitmap2))
            {
                MessageBox.Show("实时发送图片2失败");
            }
            else
            {
                MessageBox.Show("实时发送图片2成功");
            }
            DeleteObject(hBitmap2);
            bmp2.Dispose();

        }

        //实时发送图片文件
        private void button17_Click(object sender, EventArgs e)
        {
            int iX = Convert.ToInt32(ZoneX.Text);
            int iY = Convert.ToInt32(ZoneY.Text);
            int iW = Convert.ToInt32(ZoneW.Text);
            int iH = Convert.ToInt32(ZoneH.Text);
            string strBmpFile = "BMP2.bmp";

            if (!User_RealtimeSendBmpData(g_iCardNum, iX, iY, iW, iH, strBmpFile))
            {
                MessageBox.Show("实时发送指定路径图片失败！");
            }
            else
            {
                MessageBox.Show("实时发送指定路径图片成功！");
            }

        }

        //实时发送文本内容
        private void button18_Click(object sender, EventArgs e)
        {
            int iX = Convert.ToInt32(ZoneX.Text);
            int iY = Convert.ToInt32(ZoneY.Text);
            int iW = Convert.ToInt32(ZoneW.Text);
            int iH = Convert.ToInt32(ZoneH.Text);
	        string   strText = "实时发送文本测试!";
	        User_FontSet FontInfo = new User_FontSet();

	        FontInfo.bFontBold = false;
            FontInfo.bFontItaic = false;
            FontInfo.bFontUnderline = false;
	        FontInfo.colorFont = 0xFFFF;
	        FontInfo.iFontSize = 12;
	        FontInfo.strFontName = "宋体";
	        FontInfo.iAlignStyle = 0;
	        FontInfo.iVAlignerStyle = 0;
	        FontInfo.iRowSpace = 0;

            if (!User_RealtimeSendText(g_iCardNum, iX, iY, iW, iH, strText, ref FontInfo))
	        {
                MessageBox.Show("发送实时文本失败！");
	        }
	        else
	        {
                MessageBox.Show("发送实时文本成功！");
	        }
        }
        
        //清空显示屏原有节目
        private void button19_Click(object sender, EventArgs e)
        {
            if (!User_RealtimeScreenClear(g_iCardNum))
            {
                MessageBox.Show("清空控制卡节目失败！");
            }
            else
            {
                MessageBox.Show("清空控制卡节目成功！");
            }
        }

        private void ZoneText_TextChanged(object sender, EventArgs e)
        {

        }




    }
}
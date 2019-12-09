VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "EQ2008动态库开发实例"
   ClientHeight    =   6780
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8460
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6780
   ScaleWidth      =   8460
   StartUpPosition =   2  '屏幕中心
   Begin VB.Frame 实时更新数据 
      Caption         =   "实时更新数据"
      Height          =   6495
      Left            =   5880
      TabIndex        =   22
      Top             =   120
      Width           =   2415
      Begin VB.CommandButton Command6 
         Caption         =   "清空显示屏节目"
         Height          =   375
         Left            =   480
         TabIndex        =   39
         Top             =   6000
         Width           =   1815
      End
      Begin VB.CommandButton Command5 
         Caption         =   "断开连接"
         Height          =   375
         Left            =   480
         TabIndex        =   38
         Top             =   3360
         Width           =   1815
      End
      Begin VB.CommandButton Command4 
         Caption         =   "实时发送文本"
         Height          =   375
         Left            =   480
         TabIndex        =   37
         Top             =   2760
         Width           =   1815
      End
      Begin VB.CommandButton Command3 
         Caption         =   "实时发送图片文件"
         Height          =   375
         Left            =   480
         TabIndex        =   36
         Top             =   2280
         Width           =   1815
      End
      Begin VB.CommandButton Command2 
         Caption         =   "实时发送图片句柄"
         Height          =   375
         Left            =   480
         TabIndex        =   35
         Top             =   1800
         Width           =   1815
      End
      Begin VB.CommandButton Command1 
         Caption         =   "建立连接"
         Height          =   375
         Left            =   480
         TabIndex        =   34
         Top             =   1320
         Width           =   1815
      End
      Begin VB.Frame Frame5 
         Caption         =   "更新区域"
         Height          =   975
         Left            =   120
         TabIndex        =   23
         Top             =   240
         Width           =   2175
         Begin VB.TextBox ZoneH 
            Height          =   270
            Left            =   1440
            TabIndex        =   30
            Text            =   "32"
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox ZoneW 
            Height          =   270
            Left            =   840
            TabIndex        =   29
            Text            =   "64"
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox ZoneY 
            Height          =   270
            Left            =   1440
            TabIndex        =   28
            Text            =   "0"
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox ZoneX 
            Height          =   270
            Left            =   840
            TabIndex        =   27
            Text            =   "0"
            Top             =   232
            Width           =   495
         End
         Begin VB.Label Label4 
            Caption         =   "宽高(WH)"
            Height          =   255
            Left            =   120
            TabIndex        =   26
            Top             =   600
            Width           =   735
         End
         Begin VB.Label Label3 
            Caption         =   "坐标(XY)"
            Height          =   255
            Left            =   120
            TabIndex        =   24
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.Label Label8 
         Caption         =   "3、"
         Height          =   255
         Left            =   120
         TabIndex        =   33
         Top             =   3360
         Width           =   255
      End
      Begin VB.Label Label7 
         Caption         =   "2、"
         Height          =   255
         Left            =   120
         TabIndex        =   32
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label Label6 
         Caption         =   "1、"
         Height          =   255
         Left            =   120
         TabIndex        =   31
         Top             =   1320
         Width           =   255
      End
   End
   Begin VB.ComboBox Combo1 
      Height          =   300
      ItemData        =   "Main.frx":0000
      Left            =   1920
      List            =   "Main.frx":0022
      TabIndex        =   21
      Text            =   "Combo1"
      Top             =   210
      Width           =   3855
   End
   Begin VB.Frame Frame4 
      Caption         =   "显示屏控制函数"
      Height          =   975
      Left            =   240
      TabIndex        =   3
      Top             =   5640
      Width           =   5535
      Begin VB.CommandButton AjustTime 
         Caption         =   "校正时间"
         Height          =   375
         Left            =   3840
         TabIndex        =   14
         Top             =   360
         Width           =   1335
      End
      Begin VB.CommandButton CloseScreen 
         Caption         =   "关闭显示屏"
         Height          =   375
         Left            =   2040
         TabIndex        =   13
         Top             =   360
         Width           =   1335
      End
      Begin VB.CommandButton OpenScreen 
         Caption         =   "打开显示屏"
         Height          =   375
         Left            =   240
         TabIndex        =   12
         Top             =   360
         Width           =   1335
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "3、向显示屏发送数据"
      Height          =   975
      Left            =   240
      TabIndex        =   2
      Top             =   4560
      Width           =   5535
      Begin VB.CommandButton Send 
         Caption         =   "发送数据"
         Height          =   375
         Left            =   240
         TabIndex        =   11
         Top             =   360
         Width           =   1335
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "2、向节目里面添加内容"
      Height          =   2415
      Left            =   240
      TabIndex        =   1
      Top             =   1920
      Width           =   5535
      Begin VB.PictureBox Picture2 
         Height          =   615
         Left            =   2040
         Picture         =   "Main.frx":0045
         ScaleHeight     =   555
         ScaleWidth      =   1275
         TabIndex        =   18
         Top             =   1560
         Width           =   1335
      End
      Begin VB.PictureBox Picture1 
         Height          =   615
         Left            =   240
         Picture         =   "Main.frx":1887
         ScaleHeight     =   555
         ScaleWidth      =   1275
         TabIndex        =   17
         Top             =   1560
         Width           =   1335
      End
      Begin VB.CommandButton AddBmp 
         Caption         =   "添加图片"
         Height          =   375
         Left            =   3840
         TabIndex        =   10
         Top             =   1680
         Width           =   1335
      End
      Begin VB.CommandButton AddTemperature 
         Caption         =   "添加温度"
         Height          =   375
         Left            =   3840
         TabIndex        =   9
         Top             =   960
         Width           =   1335
      End
      Begin VB.CommandButton AddTimeCount 
         Caption         =   "添加计时"
         Height          =   375
         Left            =   2040
         TabIndex        =   8
         Top             =   960
         Width           =   1335
      End
      Begin VB.CommandButton AddTime 
         Caption         =   "添加时间"
         Height          =   375
         Left            =   240
         TabIndex        =   7
         Top             =   960
         Width           =   1335
      End
      Begin VB.CommandButton AddSingleText 
         Caption         =   "添加单行文本"
         Height          =   375
         Left            =   3840
         TabIndex        =   6
         Top             =   360
         Width           =   1335
      End
      Begin VB.CommandButton AddRTF 
         Caption         =   "添加RTF文件"
         Height          =   375
         Left            =   2040
         TabIndex        =   5
         Top             =   360
         Width           =   1335
      End
      Begin VB.CommandButton AddText 
         Caption         =   "添加文本"
         Height          =   375
         Left            =   240
         TabIndex        =   4
         Top             =   360
         Width           =   1335
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "1、添加节目索引"
      Height          =   975
      Left            =   240
      TabIndex        =   0
      Top             =   720
      Width           =   5535
      Begin VB.CommandButton DelProgram 
         Caption         =   "删除节目"
         Height          =   375
         Left            =   2040
         TabIndex        =   16
         Top             =   360
         Width           =   1335
      End
      Begin VB.CommandButton AddProgram 
         Caption         =   "添加节目"
         Height          =   375
         Left            =   240
         TabIndex        =   15
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label Label1 
         Caption         =   "请首先添加节目！"
         ForeColor       =   &H000000FF&
         Height          =   255
         Left            =   3720
         TabIndex        =   19
         Top             =   480
         Width           =   1695
      End
   End
   Begin VB.Label Label5 
      Caption         =   "宽高(WH)"
      Height          =   255
      Left            =   6120
      TabIndex        =   25
      Top             =   840
      Width           =   735
   End
   Begin VB.Label Label2 
      Caption         =   "控制卡地址选择:"
      Height          =   255
      Left            =   240
      TabIndex        =   20
      Top             =   240
      Width           =   1455
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim m_iCardNum As Integer
Dim m_iProgramIndex As Integer
Dim m_iProgramCount As Integer



'初始化
Private Sub Form_Load()
    m_iCardNum = 1
    m_iProgramIndex = -1
    m_iProgramCount = 0
    Combo1.Text = "1"
End Sub

'控制卡地址选择
Private Sub Combo1_Click()
    m_iCardNum = Combo1.Text
End Sub

'添加节目索引
Private Sub AddProgram_Click()
    m_iProgramIndex = User_AddProgram(m_iCardNum, EQ_FALSE, 10)
    
    '提示信息
    m_iProgramCount = m_iProgramCount + 1
    Me.Label1.Caption = "当前节目号为：" & m_iProgramCount
End Sub

'添加文本
Private Sub AddText_Click()
    Dim Text As User_Text

    Text.BkColor = 0
    Text.chContent = "欢迎使用EQ2008型控制卡动态库!"

    Text.PartInfo.FrameColor = 0
    Text.PartInfo.iFrameMode = 0
    Text.PartInfo.iHeight = 32
    Text.PartInfo.iWidth = 64
    Text.PartInfo.iX = 0
    Text.PartInfo.iY = 0

    Text.FontInfo.bFontBold = EQ_FALSE
    Text.FontInfo.bFontItaic = EQ_FALSE
    Text.FontInfo.bFontUnderline = EQ_FALSE
    Text.FontInfo.colorFont = YELLOW
    Text.FontInfo.iFontSize = 16
    Text.FontInfo.strFontName = ""
    Text.FontInfo.iAlignStyle = 1
    Text.FontInfo.iVAlignerStyle = 0
    Text.FontInfo.iRowSpace = 0

    Text.MoveSet.bClear = 0
    Text.MoveSet.iActionSpeed = 4
    Text.MoveSet.iActionType = 0
    Text.MoveSet.iHoldTime = 10
    Text.MoveSet.iClearActionType = 0
    Text.MoveSet.iClearSpeed = 4
    Text.MoveSet.iFrameTime = 10

    If (User_AddText(m_iCardNum, Text, m_iProgramIndex) = -1) Then
        MsgBox ("添加文本失败！")
    Else
        MsgBox ("添加文本成功！")
    End If
        
End Sub

'添加RTF文件
Private Sub AddRTF_Click()

    Dim strPath As String
    strPath = CurDir() & "\EQ2008_RTF.rtf"

    Dim RTF As User_RTF

    RTF.strFileName = strPath

    RTF.PartInfo.FrameColor = 0
    RTF.PartInfo.iFrameMode = 0
    RTF.PartInfo.iHeight = 32
    RTF.PartInfo.iWidth = 64
    RTF.PartInfo.iX = 0
    RTF.PartInfo.iY = 0

    RTF.MoveSet.bClear = 0
    RTF.MoveSet.iActionSpeed = 4
    RTF.MoveSet.iActionType = 0
    RTF.MoveSet.iHoldTime = 10
    RTF.MoveSet.iClearActionType = 0
    RTF.MoveSet.iClearSpeed = 4
    RTF.MoveSet.iFrameTime = 10

    If (User_AddRTF(m_iCardNum, RTF, m_iProgramIndex) = -1) Then
        MsgBox ("添加rtf文件失败！")
    Else
         MsgBox ("添加rtf文件成功！")
    End If
End Sub

'添加单行文本
Private Sub AddSingleText_Click()

    Dim SingleText As User_SingleText

    SingleText.BkColor = 0
    SingleText.chContent = "欢迎使用EQ2008型控制卡动态库!"
    SingleText.PartInfo.iFrameMode = 0
    SingleText.PartInfo.iHeight = 32
    SingleText.PartInfo.iWidth = 64
    SingleText.PartInfo.iX = 0
    SingleText.PartInfo.iY = 0
    SingleText.FontInfo.bFontBold = 0
    SingleText.FontInfo.bFontItaic = 0
    SingleText.FontInfo.bFontUnderline = 0
    SingleText.FontInfo.colorFont = RED
    SingleText.FontInfo.iFontSize = 16
    SingleText.PartInfo.FrameColor = 0
    SingleText.FontInfo.strFontName = ""
    SingleText.MoveSet.bClear = 0
    SingleText.MoveSet.iActionSpeed = 0
    SingleText.MoveSet.iActionType = 20
    SingleText.MoveSet.iHoldTime = 0
    SingleText.MoveSet.iClearActionType = 0
    SingleText.MoveSet.iClearSpeed = 4
    SingleText.MoveSet.iFrameTime = 50
    
    If (User_AddSingleText(m_iCardNum, SingleText, m_iProgramIndex) = -1) Then
        MsgBox ("添加单行文本失败！")
    Else
         MsgBox ("添加单行文本成功！")
    End If
   
End Sub

'添加图片
Private Sub AddBmp_Click()
    Dim BmpZone As User_Bmp
    Dim MoveSet As User_MoveSet
    Dim iBMPZoneNum  As Integer
    iBMPZoneNum = -1

    BmpZone.PartInfo.iX = 0
    BmpZone.PartInfo.iY = 0
    BmpZone.PartInfo.iHeight = 32
    BmpZone.PartInfo.iWidth = 64
    BmpZone.PartInfo.FrameColor = GREEN
    BmpZone.PartInfo.iFrameMode = 1

    MoveSet.bClear = 1
    MoveSet.iActionSpeed = 4
    MoveSet.iActionType = 0
    MoveSet.iHoldTime = 50
    MoveSet.iClearActionType = 0
    MoveSet.iClearSpeed = 4
    MoveSet.iFrameTime = 10

    iBMPZoneNum = User_AddBmpZone(m_iCardNum, BmpZone, m_iProgramIndex)

    If (User_AddBmp(m_iCardNum, iBMPZoneNum, Picture1.Picture.Handle, MoveSet, m_iProgramIndex) = -1) Then
        MsgBox ("添加图片失败！")
    End If
    

    If (User_AddBmp(m_iCardNum, iBMPZoneNum, Picture2.Picture.Handle, MoveSet, m_iProgramIndex) = -1) Then
        MsgBox ("添加图片失败！")
    End If
    
End Sub

'添加时间
Private Sub AddTime_Click()
    Dim DateTime As User_DateTime
  
    DateTime.bDay = 0
    DateTime.bHour = 1
    DateTime.BkColor = 0
    DateTime.bMin = 1
    DateTime.bMouth = 0
    DateTime.bMulOrSingleLine = 0
    DateTime.bSec = 1
    DateTime.bWeek = 0
    DateTime.bYear = 0
    DateTime.bYearDisType = 0
    DateTime.chTitle = ""
  
    DateTime.PartInfo.iFrameMode = 1
    DateTime.iDisplayType = 1
    DateTime.PartInfo.FrameColor = YELLOW
    DateTime.PartInfo.iHeight = 32
    DateTime.PartInfo.iWidth = 64
    DateTime.PartInfo.iX = 0
    DateTime.PartInfo.iY = 0
    DateTime.FontInfo.bFontBold = 0
    DateTime.FontInfo.bFontItaic = 0
    DateTime.FontInfo.bFontUnderline = 0
    DateTime.FontInfo.colorFont = RED
    DateTime.FontInfo.iAlignStyle = 1
    DateTime.FontInfo.iFontSize = 12
    DateTime.FontInfo.strFontName = "宋体"
  
    If (User_AddTime(m_iCardNum, DateTime, m_iProgramIndex) = -1) Then
        MsgBox ("添加时间失败！")
    Else
        MsgBox ("添加时间成功！")
    End If
    
End Sub

'添加计时
Private Sub AddTimeCount_Click()
    Dim TimeCount As User_Timer

    TimeCount.bDay = 1
    TimeCount.bHour = 0
    TimeCount.BkColor = 0
    TimeCount.bMin = 0
    TimeCount.bMulOrSingleLine = 1
    TimeCount.bSec = 0
    TimeCount.chTitle = "距离五一"
    TimeCount.FontInfo.bFontBold = 0
    TimeCount.FontInfo.bFontItaic = 0
    TimeCount.FontInfo.bFontUnderline = 0
    TimeCount.FontInfo.colorFont = YELLOW
    TimeCount.FontInfo.iAlignStyle = 2
    TimeCount.FontInfo.iFontSize = 16
    TimeCount.FontInfo.strFontName = ""
    TimeCount.PartInfo.FrameColor = GREEN
    TimeCount.PartInfo.iFrameMode = 0
    TimeCount.PartInfo.iHeight = 32
    TimeCount.PartInfo.iWidth = 64
    TimeCount.PartInfo.iX = 0
    TimeCount.PartInfo.iY = 0
    TimeCount.ReachTimeYear = 2007
    TimeCount.ReachTimeMonth = 5
    TimeCount.ReachTimeDay = 1
    TimeCount.ReachTimeHour = 10
    TimeCount.ReachTimeMinute = 0
    TimeCount.ReachTimeSecond = 0

    If (User_AddTimeCount(m_iCardNum, TimeCount, m_iProgramIndex) = -1) Then
        MsgBox ("添加计时失败！")
    Else
        MsgBox ("添加计时成功！")
    End If
    
End Sub

'添加温度
Private Sub AddTemperature_Click()
    Dim Temperature As User_Temperature

    Temperature.BkColor = 0
    Temperature.chTitle = ""
    Temperature.iDisplayType = 0
    Temperature.PartInfo.FrameColor = GREEN
    Temperature.PartInfo.iFrameMode = 1
    Temperature.PartInfo.iHeight = 32
    Temperature.PartInfo.iWidth = 64
    Temperature.PartInfo.iX = 0
    Temperature.PartInfo.iY = 0
    Temperature.FontInfo.bFontBold = 0
    Temperature.FontInfo.bFontItaic = 0
    Temperature.FontInfo.bFontUnderline = 0
    Temperature.FontInfo.colorFont = YELLOW
    Temperature.FontInfo.iAlignStyle = 0
    Temperature.FontInfo.iFontSize = 12
    Temperature.FontInfo.strFontName = "宋体"

    If (User_AddTemperature(m_iCardNum, Temperature, m_iProgramIndex) = -1) Then
        MsgBox ("添加温度失败！")
    Else
        MsgBox ("添加温度成功！")
    End If
    
End Sub

'删除节目
Private Sub DelProgram_Click()
    If (User_DelAllProgram(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("删除节目失败！")
    Else
        '提示信息
        m_iProgramCount = 0
        Me.Label1.Caption = "请首先添加节目！"
    End If
End Sub


'发送数据
Private Sub Send_Click()
    If (User_SendToScreen(m_iCardNum) = False) Then
        MsgBox ("数据发送失败！")
    Else
        MsgBox ("数据发送成功！")
    End If
End Sub

'打开显示屏
Private Sub OpenScreen_Click()
    If (User_OpenScreen(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("打开显示屏失败！")
    Else
        MsgBox ("打开显示屏成功！")
    End If
End Sub

'关闭显示屏
Private Sub CloseScreen_Click()
    If (User_CloseScreen(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("关闭显示屏失败！")
    Else
        MsgBox ("关闭显示屏成功！")
    End If
End Sub

'校正时间
Private Sub AjustTime_Click()
    If (User_AdjustTime(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("校准时间失败！")
    Else
        MsgBox ("校准时间成功！")
    End If
End Sub

'实时发送建立连接
Private Sub Command1_Click()
    If (User_RealtimeConnect(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("实时发送数据建立连接失败！")
    Else
        MsgBox ("实时发送数据建立连接成功！")
    End If
End Sub

'实时发送图片句柄
Private Sub Command2_Click()
    If (User_RealtimeSendData(m_iCardNum, ZoneX.Text, ZoneY.Text, ZoneW.Text, ZoneH.Text, Picture1.Picture.Handle) = EQ_FALSE) Then
        MsgBox ("发送实时图片句柄失败！")
    Else
        MsgBox ("发送实时图片句柄成功！")
    End If
End Sub

'实时发送图片文件
Private Sub Command3_Click()
    If (User_RealtimeSendBmpData(m_iCardNum, ZoneX.Text, ZoneY.Text, ZoneW.Text, ZoneH.Text, "BMP2.bmp") = EQ_FALSE) Then
        MsgBox ("发送实时图片路径失败！")
    Else
        MsgBox ("发送实时图片路径成功！")
    End If
End Sub

'实时发送文本信息
Private Sub Command4_Click()
    Dim strText As String
    Dim FontInfo As User_FontSet
    
    strText = "实时发送文本测试!"

    FontInfo.bFontBold = False
    FontInfo.bFontItaic = False
    FontInfo.bFontUnderline = False
    FontInfo.colorFont = YELLOW
    FontInfo.iFontSize = 12
    FontInfo.strFontName = "宋体"
    FontInfo.iAlignStyle = 0
    FontInfo.iVAlignerStyle = 0
    FontInfo.iRowSpace = 0

    If (User_RealtimeSendText(m_iCardNum, ZoneX.Text, ZoneY.Text, ZoneW.Text, ZoneH.Text, strText, FontInfo) = EQ_FALSE) Then
        MsgBox ("发送实时文本失败！")
    Else
        MsgBox ("发送实时文本成功！")
    End If
End Sub

'实时发送断开连接
Private Sub Command5_Click()
    If (User_RealtimeDisConnect(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("实时发送数据断开连接失败！")
    Else
        MsgBox ("实时发送数据断开连接成功！")
    End If
End Sub

'清空控制卡节目
Private Sub Command6_Click()
    If (User_RealtimeScreenClear(m_iCardNum) = EQ_FALSE) Then
        MsgBox ("清空控制卡节目失败！")
    Else
        MsgBox ("清空控制卡节目成功！")
    End If
End Sub


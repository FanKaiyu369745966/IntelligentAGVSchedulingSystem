Attribute VB_Name = "DataStruct"
  '节目区域结构
  Type User_PartInfo
        iX As Long               '窗口的起点X
        iY As Long               '窗口的起点Y
        iWidth As Long           '窗体的宽度
        iHeight As Long          '窗体的高度
        iFrameMode As Long       '边框的样式
        FrameColor As Long       '边框颜色
  End Type
  
  '字体设置
  Type User_FontSet
        strFontName As String    '字体的名称
        iFontSize As Long        '字体的大小
        bFontBold As Long        '字体是否加粗
        bFontItaic As Long       '字体是否是斜体
        bFontUnderline As Long   '字体是否带下划线
        colorFont As Long        '字体的颜色
        iAlignStyle As Long      '对齐方式
                                 '0－ 左对齐
                                 '1－居中
                                 '2－右对齐
        iVAlignerStyle As Long   '上下对齐方式
                                 '0-顶对齐
                                 '1-上下居中
                                 '2-底对齐
        iRowSpace As Long        '行间距
    End Type

    '动作方式设置
    Type User_MoveSet
        iActionType As Long      '节目变换方式
        iActionSpeed As Long     '节目的播放速度
        bClear As Long           '是否需要清除背景
        iHoldTime As Long        '在屏幕上停留的时间
        iClearSpeed As Long      '清除显示屏的速度
        iClearActionType As Long '节目清除的变换方式
        iFrameTime As Long
    End Type


    '“图文框”
    Type User_Bmp
        PartInfo As User_PartInfo   '分区信息
    End Type

    '“单行文本框”
    Type User_SingleText
        chContent As String         '显示内容
        PartInfo As User_PartInfo   '分区信息
        BkColor As Long          '背景颜色
        FontInfo As User_FontSet    '字体设置
        MoveSet As User_MoveSet     '动作方式设置
    End Type

    '“文本框”
    Type User_Text
        chContent As String         '显示内容
        PartInfo As User_PartInfo   '分区信息
        BkColor As Long          '背景颜色
        FontInfo As User_FontSet    '字体设置
        MoveSet As User_MoveSet     '动作方式设置
    End Type

    '“RTF文件”
    Type User_RTF
        strFileName As String        'RTF文件名
        PartInfo As User_PartInfo    '分区信息
        MoveSet As User_MoveSet      '动作方式设置
    End Type
    
     '“计时窗口”
    Type User_Timer
        PartInfo As User_PartInfo   '分区信息
        BkColor As Long          '背景颜色
        FontInfo As User_FontSet    '字体设置
        ReachTimeYear As Long    '到达年
        ReachTimeMonth As Long   '到达月
        ReachTimeDay As Long     '到达日
        ReachTimeHour As Long    '到达时
        ReachTimeMinute As Long  '到达分
        ReachTimeSecond As Long  '到达秒
        bDay As Long             '是否显示天 0－不显示 1－显示
        bHour As Long            '是否显示小时
        bMin As Long             '是否显示分钟
        bSec As Long             '是否显示秒
        bMulOrSingleLine As Long '单行还是多行
        chTitle As String           '添加显示文字
    End Type


    '“温度窗口”
    Type User_Temperature
        PartInfo As User_PartInfo   '分区信息
        BkColor As Long          '背景颜色
        FontInfo As User_FontSet    '字体设置
        chTitle As String           '标题
        iDisplayType As Long     '显示格式：0－度 1－C
    End Type

    '“日期时间窗口”
    Type User_DateTime
        PartInfo As User_PartInfo   '分区信息
        BkColor As Long          '背景颜色
        FontInfo As User_FontSet    '字体设置
        iDisplayType As Long     '显示风格
        chTitle As String           '添加显示文字
        bYearDisType As Long     '年份位数0 －4；1－2位
        bMulOrSingleLine As Long '单行还是多行
        bYear As Long
        bMouth As Long
        bDay As Long
        bWeek As Long
        bHour As Long
        bMin As Long
        bSec As Long
    End Type

Attribute VB_Name = "DataStruct"

  Structure User_FontSet              '字体设置
        Dim strFontName As String       '字体的名称
        Dim iFontSize As Integer        '字体的大小
        Dim bFontBold As Integer        '字体是否加粗
        Dim bFontItaic As Integer       '字体是否是斜体
        Dim bFontUnderline As Integer   '字体是否带下划线
        Dim colorFont As Integer        '字体的颜色
        Dim iAlignStyle As Integer      '对齐方式
        '0－ 左对齐
        '1－居中
        '2－右对齐
        Dim iVAlignerStyle As Integer   '上下对齐方式
        '0-顶对齐
        '1-上下居中
        '2-底对齐
        Dim iRowSpace As Integer        '行间距
    End Structure

    Structure User_PartInfo             '字体设置
        Dim iX As Integer               '窗口的起点X
        Dim iY As Integer               '窗口的起点Y
        Dim iWidth As Integer           '窗体的宽度
        Dim iHeight As Integer          '窗体的高度
        Dim iFrameMode As Integer       '边框的样式
        Dim FrameColor As Integer       '边框颜色
    End Structure


    Structure User_MoveSet
        Dim iActionType As Integer      '节目变换方式
        Dim iActionSpeed As Integer     '节目的播放速度
        Dim bClear As Integer           '是否需要清除背景
        Dim iHoldTime As Integer        '在屏幕上停留的时间
        Dim iClearSpeed As Integer      '清除显示屏的速度
        Dim iClearActionType As Integer '节目清除的变换方式
        Dim iFrameTime As Integer
    End Structure

    '“计时窗口”
    Structure User_Timer
        Dim PartInfo As User_PartInfo   '分区信息
        Dim BkColor As Integer          '背景颜色
        Dim FontInfo As User_FontSet    '字体设置
        Dim ReachTimeYear As Integer    '到达年
        Dim ReachTimeMonth As Integer   '到达月
        Dim ReachTimeDay As Integer     '到达日
        Dim ReachTimeHour As Integer    '到达时
        Dim ReachTimeMinute As Integer  '到达分
        Dim ReachTimeSecond As Integer  '到达秒
        Dim bDay As Integer             '是否显示天 0－不显示 1－显示
        Dim bHour As Integer            '是否显示小时
        Dim bMin As Integer             '是否显示分钟
        Dim bSec As Integer             '是否显示秒
        Dim bMulOrSingleLine As Integer '单行还是多行
        Dim chTitle As String           '添加显示文字
    End Structure


    '“温度窗口”
    Structure User_Temperature
        Dim PartInfo As User_PartInfo   '分区信息
        Dim BkColor As Integer          '背景颜色
        Dim FontInfo As User_FontSet    '字体设置
        Dim chTitle As String           '标题
        Dim iDisplayType As Integer     '显示格式：0－度 1－C
    End Structure

    '“日期时间窗口”
    Structure User_DateTime
        Dim PartInfo As User_PartInfo   '分区信息
        Dim BkColor As Integer          '背景颜色
        Dim FontInfo As User_FontSet    '字体设置
        Dim iDisplayType As Integer     '显示风格
        Dim chTitle As String           '添加显示文字
        Dim bYearDisType As Integer     '年份位数0 －4；1－2位
        Dim bMulOrSingleLine As Integer '单行还是多行
        Dim bYear As Integer
        Dim bMouth As Integer
        Dim bDay As Integer
        Dim bWeek As Integer
        Dim bHour As Integer
        Dim bMin As Integer
        Dim bSec As Integer
    End Structure

    '“图文框”
    Structure User_Bmp
        Dim PartInfo As User_PartInfo   '分区信息
    End Structure

    '“单行文本框”
    Structure User_SingleText
        Dim chContent As String         '显示内容
        Dim PartInfo As User_PartInfo   '分区信息
        Dim BkColor As Integer          '背景颜色
        Dim FontInfo As User_FontSet    '字体设置
        Dim MoveSet As User_MoveSet     '动作方式设置
    End Structure

    '“文本框”
    Structure User_Text
        Dim chContent As String         '显示内容
        Dim PartInfo As User_PartInfo   '分区信息
        Dim BkColor As Integer          '背景颜色
        Dim FontInfo As User_FontSet    '字体设置
        Dim MoveSet As User_MoveSet     '动作方式设置
    End Structure

    '“RTF文件”
    Structure User_RTF
        Dim strFileName As String        'RTF文件名
        Dim PartInfo As User_PartInfo    '分区信息
        Dim MoveSet As User_MoveSet      '动作方式设置
    End Structure

/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file GraphicAGV.h
 *
 * @brief AGV图标属性的文件
 *
 * @date 2019/11/20
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef GRAPHICAGV_H
#define GRAPHICAGV_H

#include "GraphicMember.h"

typedef class GraphicAGV IconAGV;

/*!
 * @class GraphicAGV
 * @brief 描述AGV图标的基本属性以及功能的类
 * @date 2019/11/20
 * @author FanKaiyu
 */
class GraphicAGV :
	public GraphicMember
{
public:
	explicit GraphicAGV(const quint32& unNo);
protected:
	quint32 m_unNo;             /*!< 编号 */
	bool m_bShow;				/*!< 显示标识 */
	bool m_bAuto;				/*!< 自动模式标识 */
	bool m_bRun;				/*!< 移动标识 */
	bool m_bUnVol;				/*!< 电量不足标识 */
	bool m_bObs;				/*!< 避障标识 */
	bool m_bError;				/*!< 异常标识 */

protected:
	static quint32 g_unWidth;   /*!< 图标宽 */
	static quint32 g_unHeight;  /*!< 图标高 */
protected:
	static QImage g_icon;		/*!< AGV图标 */
	static QImage g_handicon;	/*!< AGV手动模式图标 */
	static QImage g_runIcon;	/*!< 移动标识图标 */
	static QImage g_unvolIcon;	/*!< 电量不足标识图标 */
	static QImage g_errorIcon;	/*!< 异常标识图标 */
	static QImage g_obsIcon;	/*!< 避障标识图标 */
public:
	/*!
	 * @brief 获取编号
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	quint32 GetNo() const { return m_unNo; }

	/*!
	 * @brief 设置图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetIcon(const QString& path);

	/*!
	 * @brief 设置手动模式图标
	 * @date 2019/11/28
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetHandIcon(const QString& path);

	/*!
	 * @brief 设置移动图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetRunIcon(const QString& path);

	/*!
	 * @brief 设置电量不足图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetUnvolIcon(const QString& path);

	/*!
	 * @brief 设置异常图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetErrorIcon(const QString& path);

	/*!
	 * @brief 设置避障图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetObsIcon(const QString& path);

	/*!
	 * @brief 获取图标宽
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	static quint32 GetWidth() { return g_unWidth; }

	/*!
	 * @brief 设置图标宽
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const quint32 & 图标宽
	 */
	static void SetWidth(const quint32& width) { g_unWidth = width; return; }

	/*!
	 * @brief 获取图标高
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	static quint32 GetHeight() { return g_unHeight; }

	/*!
	 * @brief 设置图标高
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const quint32 & 图标高
	 */
	static void SetHeight(const quint32& height) { g_unHeight = height; return; }

public:
	/*!
	 * @brief 显示
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const bool & 值true显示图标,false隐藏图标
	 */
	void Show(const bool& show) { m_bShow = show; return; }

	/*!
	 * @brief 设置状态
	 * @date 2019/11/20
	 * @author FanKaiyu
	  * @param const bool & 模式标识
	 * @param const bool & 移动标识
	 * @param const bool & 电量不足标识
	 * @param const bool & 避障标识
	 * @param const bool & 异常标识
	 * @return void
	 */
	void SetStatus(const bool& bAuto, const bool& bRun, const bool& bUnVol, const bool& bObs, const bool& bError);

	/*!
	 * @brief 绘制图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param QPainter & painter
	 */
	void Draw(QPainter& painter);
};
#endif // GRAPHICAGV_H

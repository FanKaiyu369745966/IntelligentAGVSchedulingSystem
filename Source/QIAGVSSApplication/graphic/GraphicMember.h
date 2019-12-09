#ifndef GRAPHICMEMBER_H
#define GRAPHICMEMBER_H

#include "Graphic.h"
#include <chrono>

class GraphicMember
	: public Graphic
{
public:
	GraphicMember();
protected:
	QPoint m_ptCenter;		/*!< 中心坐标 */
protected:
	static std::chrono::steady_clock::time_point m_tmptFlash;	/*!< 闪烁开始时间 */
	static bool g_bFlash;										/*!< 闪烁标识 */
public:
	/*!
	 * @brief 开始闪烁
	 * @date 2019/11/19
	 * @author FanKaiyu
	 */
	static void StartFlash();
public:
	/*!
	 * @brief 获取中心坐标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::QPoint	中心坐标
	 */
	QPoint GetCenterPoint() const { return m_ptCenter; }

	/*!
	 * @brief 设置中心坐标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QPoint & 新的中心坐标
	 */
	void SetCenterPoint(const QPoint& ptCenter);
public:
	/*!
	 * @brief 绘制图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param QPainter & painter
	 */
	virtual void Draw(QPainter& painter) = 0;
};

#endif // GRAPHICMEMBER_H

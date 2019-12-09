#ifndef GRAPHICTRACK_H
#define GRAPHICTRACK_H

#include "GraphicMember.h"

typedef class GraphicTrack IconTrack;

class GraphicTrack :
	public GraphicMember
{
public:
	GraphicTrack(const bool& bArc);

public:
	static const double PI;
protected:
	QPoint m_ptEnd;         /*!< 终点坐标 */
	bool m_bArc;            /*!< 弧线标识 */

	static float g_fWidth;	/*!< 宽 */
public:
	/**
	 * @brief setStartPoint 设置起点坐标
	 * @param ptStart 起点坐标(矢量坐标)
	 */
	void SetStartPoint(const QPoint& pt);

	/**
	 * @brief setEndPoint 设置终点坐标
	 * @param ptEnd 终点坐标(矢量坐标)
	 */
	void SetEndPoint(const QPoint& pt);

	/**
	 * @brief setType 设置轨道类型
	 * @param bArc true 弧线, false 直线
	 */
	void SetArc(const bool& bArc) { m_bArc = bArc; return; }

	/*!
	 * @brief 获取起点坐标(矢量坐标)
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::QPoint 起点坐标
	 */
	QPoint GetStartPoint() const { return m_ptCenter; }

	/*!
	 * @brief 获取终点坐标(矢量坐标)
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::QPoint 终点坐标
	 */
	QPoint GetEndPoint() const { return m_ptEnd; }

	/*!
	 * @brief 是否时弧线
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return bool true 弧形, false 直线
	 */
	bool IsArc() const { return m_bArc; }

	/**
	 * @brief draw      绘制
	 * @param painter   绘画
	 */
	void Draw(QPainter& painter);

	/**
	 * @brief setWidth  设置轨道宽度
	 * @param fWidth 宽度
	 */
	static void SetWidth(const float& fWidth) { g_fWidth = fWidth; return; }

	/**
	 * @brief getWidth 获取轨道宽度
	 * @return 宽度
	 */
	static float GetWidth() { return g_fWidth; }

	/**
	 * @brief radianToAngle 弧度转角度
	 * @param dbRadian  弧度值
	 * @return 角度值
	 */
	static double RadianToAngle(const double& dbRadian);

	/**
	 * @brief angleToRadian 角度转弧度
	 * @param dbAngle 角度值
	 * @return 弧度值
	 */
	static double AngleToRadian(const double& dbAngle);
};
#endif // GRAPHICTRACK_H

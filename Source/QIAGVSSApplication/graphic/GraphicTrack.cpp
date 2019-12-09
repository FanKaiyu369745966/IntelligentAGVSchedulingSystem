#include "GraphicTrack.h"

const double GraphicTrack::PI = 3.14159265;
float GraphicTrack::g_fWidth = 5.0f;

GraphicTrack::GraphicTrack(const bool& bArc)
	: m_ptEnd(0, 0)
	, m_bArc(bArc)
{
}

void GraphicTrack::SetStartPoint(const QPoint& pt)
{
	return SetCenterPoint(pt);
}

void GraphicTrack::SetEndPoint(const QPoint& pt)
{
	m_ptEnd = pt;

	return;
}

void GraphicTrack::Draw(QPainter& painter)
{
	QPoint ptStartWin = vecToWin(m_ptCenter);
	QPoint ptEndWin = vecToWin(m_ptEnd);

	// 默认画笔
	QPen pen;
	pen.setColor(QColor(Qt::black));
	pen.setWidth(g_fWidth * g_fScale);
	pen.setStyle(Qt::DashLine);
	painter.setPen(pen);

	// 弧
	if (m_bArc)
	{
		if ((ptEndWin.x() < ptStartWin.x() && ptEndWin.y() < ptStartWin.y()) ||
			(ptEndWin.x() > ptStartWin.x() && ptEndWin.y() < ptStartWin.y()) ||
			(ptEndWin.x() < ptStartWin.x() && ptEndWin.y() > ptStartWin.y()) ||
			(ptEndWin.x() > ptStartWin.x() && ptEndWin.y() > ptStartWin.y()))
		{
			QPainterPath path;
			path.moveTo(ptStartWin);
			path.cubicTo(ptStartWin, QPoint(ptEndWin.x(), ptStartWin.y()), ptEndWin);

			// 画黑线
			painter.drawPath(path);

			// 画黄线
			pen.setWidth((g_fWidth / 2) * g_fScale);
			pen.setColor(QColor(Qt::yellow));
			pen.setStyle(Qt::DashLine);
			painter.setPen(pen);
			painter.drawPath(path);
		}
		else
		{
			painter.drawLine(ptStartWin.x(), ptStartWin.y(), ptEndWin.x(), ptEndWin.y());
			pen.setWidth((g_fWidth / 2) * g_fScale);
			pen.setColor(QColor(Qt::yellow));
			pen.setStyle(Qt::DashLine);
			painter.setPen(pen);
			painter.drawLine(ptStartWin.x(), ptStartWin.y(), ptEndWin.x(), ptEndWin.y());
		}
	}
	// 直线
	else
	{
		painter.drawLine(ptStartWin.x(), ptStartWin.y(), ptEndWin.x(), ptEndWin.y());
		pen.setWidth((g_fWidth / 2) * g_fScale);
		pen.setColor(QColor(Qt::yellow));
		pen.setStyle(Qt::DashLine);
		painter.setPen(pen);
		painter.drawLine(ptStartWin.x(), ptStartWin.y(), ptEndWin.x(), ptEndWin.y());
	}

	return;
}

double GraphicTrack::RadianToAngle(const double& dbRadian)
{
	return dbRadian * 180 / PI;
}

double GraphicTrack::AngleToRadian(const double& dbAngle)
{
	return dbAngle * PI / 180;
}

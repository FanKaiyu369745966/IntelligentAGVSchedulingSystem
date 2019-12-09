#include "GraphicRFID.h"

quint32 GraphicRFID::g_unWidth = 10;	// 宽
quint32 GraphicRFID::g_unHeight = 10;	// 高

GraphicRFID::GraphicRFID(const quint32& id)
	: m_unNo(id)
{

}

void GraphicRFID::Draw(QPainter& painter)
{
	QPoint ptWindow = vecToWin(m_ptCenter);
	QRect rect;
	rect.setLeft(ptWindow.x() - (g_unWidth / 2) * g_fScale);
	rect.setTop(ptWindow.y() - (g_unHeight / 2) * g_fScale);
	rect.setRight(rect.left() + g_unWidth * g_fScale);
	rect.setBottom(rect.top() + g_unHeight * g_fScale);

	QPen pen;

	// 绘制外圈
	pen.setBrush(QColor(Qt::black));
	pen.setWidth(2);
	pen.setStyle(Qt::SolidLine);
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::white));
	painter.drawEllipse(QRect(rect.left(), rect.top(), rect.width(), rect.height()));

	// 绘制编号
	QFont font("Microsoft YaHei", (g_unWidth / 3) * g_fScale, QFont::Bold);
	painter.setFont(font);
	painter.drawText(rect.left(), rect.top(), rect.width(), rect.height(), Qt::AlignCenter, QString("%1").arg(m_unNo));

	return;
}

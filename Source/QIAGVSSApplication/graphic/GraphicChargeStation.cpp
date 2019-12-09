#include "GraphicChargeStation.h"

quint32 GraphicChargeStation::g_unWidth = 20;
quint32 GraphicChargeStation::g_unHeight = 20;
QImage GraphicChargeStation::g_icon("./res/image/charger.png");
QImage GraphicChargeStation::g_powerIcon("./res/image/power.png");

GraphicChargeStation::GraphicChargeStation(const quint32& unNo, const QString& name)
	: GraphicStation(unNo, name)
	, m_bPower(false)
{
}

void GraphicChargeStation::SetIcon(const QString& path)
{
	g_icon.load(path);
	return;
}

void GraphicChargeStation::SetPowerIcon(const QString& path)
{
	g_powerIcon.load(path);
	return;
}

void GraphicChargeStation::Draw(QPainter& painter)
{
	QPoint ptWindow = vecToWin(m_ptCenter);
	QRect rect;
	rect.setLeft(ptWindow.x() - (g_unWidth / 2) * g_fScale);
	rect.setTop(ptWindow.y() - (g_unHeight / 2) * g_fScale);
	rect.setRight(rect.left() + g_unWidth * g_fScale);
	rect.setBottom(rect.top() + g_unHeight * g_fScale);

	QPixmap icon = QPixmap::fromImage(g_icon);

	// 绘制底图
	painter.drawPixmap(QRect(rect.left(), rect.top(), rect.width(), rect.height()), icon);

	// 绘制通电标识
	if (m_bPower)
	{
		StartFlash();

		if (g_bFlash)
		{
			icon = QPixmap::fromImage(g_powerIcon);

			painter.drawPixmap(QRect(rect.left(), rect.top(), rect.width(), rect.height()), icon);
		}
	}

	// 绘制编号
	QFont font("Microsoft YaHei", (g_unWidth / 10) * g_fScale, QFont::Bold);
	painter.setFont(font);
	painter.drawText(QRectF(rect.left(), rect.top() - rect.width(), rect.width(), rect.height()), Qt::AlignCenter | Qt::AlignBottom, QString("NO.%1").arg(m_unNo));

	// 绘制名称
	painter.drawText(QRectF(rect.left(), rect.top() + rect.width(), rect.width(), rect.height()), Qt::AlignHCenter, QString::fromLocal8Bit("%1").arg(m_strName));

	return;
}

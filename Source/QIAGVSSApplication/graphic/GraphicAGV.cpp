#include "GraphicAGV.h"

quint32 GraphicAGV::g_unWidth = 20;
quint32 GraphicAGV::g_unHeight = 20;

QImage GraphicAGV::g_icon("./res/image/agv.png");
QImage GraphicAGV::g_handicon("./res/image/dagv.png");
QImage GraphicAGV::g_runIcon("./res/image/run.png");
QImage GraphicAGV::g_unvolIcon("./res/image/unvol.png");
QImage GraphicAGV::g_errorIcon("./res/image/error.png");
QImage GraphicAGV::g_obsIcon("./res/image/obs.png");


GraphicAGV::GraphicAGV(const quint32& unNo)
	: m_unNo(unNo)
	, m_bShow(false)
	, m_bAuto(false)
	, m_bRun(false)
	, m_bUnVol(false)
	, m_bError(false)
	, m_bObs(false)
{
}

void GraphicAGV::SetIcon(const QString& path)
{
	g_icon.load(path);

	return;
}

void GraphicAGV::SetHandIcon(const QString& path)
{
	g_handicon.load(path);

	return;
}

void GraphicAGV::SetRunIcon(const QString& path)
{
	g_runIcon.load(path);

	return;
}

void GraphicAGV::SetUnvolIcon(const QString& path)
{
	g_unvolIcon.load(path);

	return;
}

void GraphicAGV::SetErrorIcon(const QString& path)
{
	g_errorIcon.load(path);

	return;
}

void GraphicAGV::SetObsIcon(const QString& path)
{
	g_obsIcon.load(path);

	return;
}

void GraphicAGV::SetStatus(const bool& bAuto, const bool& bRun, const bool& bUnVol, const bool& bObs, const bool& bError)
{
	m_bAuto = bAuto;
	m_bRun = bRun;
	m_bUnVol = bUnVol;
	m_bObs = bObs;
	m_bError = bError;
	return;
}

void GraphicAGV::Draw(QPainter& painter)
{
	if (m_bShow == false)
	{
		return;
	}

	float fWidth = g_unWidth * g_fScale/*!< 实际宽 */
		, fHeight = g_unHeight * g_fScale /*!< 实际高 */;

	QPoint ptWindow = vecToWin(m_ptCenter);

	/*
	 * 货料贴图
	 *
	 * 底图
	 * 状态贴图
	 * 底图
	 *
	 * 文字贴图
	*/

	QRect rectBK/*!< 背景贴图 */,
		rectStatus /*!< 状态贴图 */,
		rectText /*!< 文本贴图 */;
	//rectOther /*!< 其他贴图 */;

	rectBK.setLeft(ptWindow.x() - fWidth / 2.0f);
	rectBK.setTop(ptWindow.y() - fHeight / 2.0f);
	rectBK.setWidth(fWidth);
	rectBK.setHeight(fHeight);

	rectStatus.setLeft(rectBK.x() + fWidth / 4.0f);
	rectStatus.setTop(rectBK.y() + fHeight / 4.0f);
	rectStatus.setWidth(rectBK.width() / 2.0f);
	rectStatus.setHeight(rectBK.height() / 2.0f);

	rectText.setTop(rectBK.bottom());
	rectText.setLeft(rectBK.left());
	rectText.setWidth(rectBK.width());
	rectText.setHeight(rectBK.height());

	/*
	rectOther.setLeft(rectBK.x() + fWidth / 4.0f);
	rectOther.setTop(rectBK.y() - rectBK.height() / 2.0f);
	rectOther.setWidth(rectBK.width() / 2.0f);
	rectOther.setHeight(rectBK.height() / 2.0f);
	*/

	QPixmap icon;

	if (m_bAuto)
	{
		icon = QPixmap::fromImage(g_icon);
	}
	else
	{
		icon = QPixmap::fromImage(g_handicon);
	}

	// 绘制AGV贴图
	painter.drawPixmap(rectBK, icon);

	/*!
	 * 绘制AGV状态
	*/
	bool bFlash = false;

	// 移动
	if (m_bRun)
	{
		icon = QPixmap::fromImage(g_runIcon);

		bFlash = true;
	}

	// 避障
	if (m_bObs)
	{
		icon = QPixmap::fromImage(g_obsIcon);

		bFlash = true;
	}

	/*
	// 动作
	switch (m_byAction)
	{
	case _GRAPHICAGVACTSTA_LOAD:
		pImage = g_pImageLoad;

		bFlash = true;
		break;
	case _GRAPHICAGVACTSTA_UNLOAD:
		pImage = g_pImageUnload;

		bFlash = true;
		break;
	}
	*/

	// 电量不足
	if (m_bUnVol)
	{
		icon = QPixmap::fromImage(g_unvolIcon);

		bFlash = true;
	}

	// 异常
	if (m_bError)
	{
		icon = QPixmap::fromImage(g_errorIcon);

		bFlash = true;
	}

	if (bFlash)
	{
		StartFlash();

		if (g_bFlash)
		{
			if (icon.toImage() == g_unvolIcon)
			{
				rectStatus.setTop(rectStatus.y() + rectBK.height() / 8.0f);
				rectStatus.setHeight(rectBK.height() / 4.0f);
			}

			painter.drawPixmap(rectStatus, icon);
		}
	}

	/*
	// 载货
	if (m_byAction == _GRAPHICAGVACTSTA_NONE && m_bCargo)
	{
		graphic->DrawImage(g_pImageCargo, rectOther);
	}
	*/

	// 绘制AGV型号
	QFont font("Microsoft YaHei", (g_unWidth / 10) * g_fScale, QFont::Bold);
	painter.setFont(font);
	painter.drawText(rectText, Qt::AlignHCenter | Qt::AlignTop, QString("NO.%1").arg(m_unNo));

	return;
}

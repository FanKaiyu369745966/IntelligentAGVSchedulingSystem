#include "GraphicCaller.h"

quint32 GraphicCaller::g_unWidth = 20;
quint32 GraphicCaller::g_unHeight = 20;

QImage GraphicCaller::g_enableIcon("./res/image/ecaller.png");
QImage GraphicCaller::g_disableIcon("./res/image/dcaller.png");
QImage GraphicCaller::g_screamIcon("./res/image/sigscream.png");
QImage GraphicCaller::g_callIcon("./res/image/sigcall.png");
QImage GraphicCaller::g_passIcon("./res/image/sigpass.png");
QImage GraphicCaller::g_cancelIcon("./res/image/signgc.png");

GraphicCaller::GraphicCaller(const quint32& unNo)
	: m_unNo(unNo)
	, m_bEnable(true)
	, m_bSigScream(false)
	, m_bSigCall(false)
	, m_bSigCancel(false)
	, m_bSigPass(false)
{
}

void GraphicCaller::SetEnableIcon(const QString& path)
{
	g_enableIcon.load(path);

	return;
}

void GraphicCaller::SetDisableIcon(const QString& path)
{
	g_disableIcon.load(path);

	return;
}

void GraphicCaller::SetScreamIcon(const QString& path)
{
	g_screamIcon.load(path);

	return;
}

void GraphicCaller::SetCallIcon(const QString& path)
{
	g_callIcon.load(path);

	return;
}

void GraphicCaller::SetPassIcon(const QString& path)
{
	g_passIcon.load(path);

	return;
}

void GraphicCaller::SetCancelIcon(const QString& path)
{
	g_cancelIcon.load(path);

	return;
}

void GraphicCaller::SetSignal(const bool& scream, const bool& call, const bool& pass, const bool& cancel)
{
	m_bSigScream = scream;
	m_bSigCall = call;
	m_bSigPass = pass;
	m_bSigCancel = cancel;

	return;
}

void GraphicCaller::SetEnable(const bool& enable)
{
	m_bEnable = enable;

	return;
}

void GraphicCaller::Draw(QPainter& painter)
{
	QPoint ptWindow = vecToWin(m_ptCenter);

	float fWidth = g_unWidth * g_fScale/*!< 实际宽 */
		, fHeight = g_unHeight * g_fScale /*!< 实际高 */;

	QRect rectBK/*!< 背景贴图 */,
		rectStatus /*!< 状态贴图 */,
		rectText /*!< 文本贴图 */;

	rectBK.setLeft(ptWindow.x() - fWidth / 2.0f);
	rectBK.setTop(ptWindow.y() - fHeight / 2.0f);
	rectBK.setWidth(fWidth);
	rectBK.setHeight(fHeight);

	rectText.setTop(rectBK.bottom());
	rectText.setLeft(rectBK.left());
	rectText.setWidth(rectBK.width());
	rectText.setHeight(rectBK.height());

	rectStatus.setLeft(rectBK.x() + fWidth / 4.0f);
	rectStatus.setTop(rectBK.y() + fHeight / 4.0f);
	rectStatus.setWidth(rectBK.width() / 2.0f);
	rectStatus.setHeight(rectBK.height() / 2.0f);

	/*!
	 * 绘制呼叫器贴图
	*/
	QPixmap icon = QPixmap::fromImage(g_enableIcon);

	if (m_bEnable == false)
	{
		icon = QPixmap::fromImage(g_disableIcon);
	}

	painter.drawPixmap(rectBK, icon);

	/*!
	 * 绘制呼叫器状态
	*/
	bool bFlash = false;

	// 呼叫
	if (m_bSigCall)
	{
		icon = QPixmap::fromImage(g_callIcon);

		bFlash = true;
	}

	// 放行
	if (m_bSigPass != 0)
	{
		icon = QPixmap::fromImage(g_passIcon);

		bFlash = true;
	}

	// 取消
	if (m_bSigCancel != 0)
	{
		icon = QPixmap::fromImage(g_passIcon);

		bFlash = true;
	}

	// 急停
	if (m_bSigScream)
	{
		icon = QPixmap::fromImage(g_cancelIcon);

		bFlash = true;
	}

	if (bFlash)
	{
		StartFlash();

		if (g_bFlash)
		{
			painter.drawPixmap(rectStatus, icon);
		}
	}

	/*!
	 * 绘制呼叫器型号
	*/
	QFont font("Microsoft YaHei", (g_unWidth / 10) * g_fScale, QFont::Bold);
	painter.setFont(font);
	painter.drawText(rectText, Qt::AlignHCenter | Qt::AlignTop, QString("NO.%1").arg(m_unNo));

	return;
}

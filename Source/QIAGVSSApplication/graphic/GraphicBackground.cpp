#include "GraphicBackground.h"

GraphicBackground::GraphicBackground(const QColor& colorLine, const float& fWidth, const QString& strPath)
	: m_bMove(false)
	, m_ptPrepoint(0, 0)
	, m_ptLastpoint(0, 0)
	, m_ptLocal(0, 0)
	, m_pImage(strPath)
{
}

void GraphicBackground::SetImage(const QString& strPath)
{
	std::lock_guard<std::mutex> locker(m_mutexLock);

	m_pImage.load(strPath);

	return;
}

void GraphicBackground::Scale(bool bShrink)
{
	if (bShrink)
	{
		if (g_fScale >= MIN_SCALE)
		{
			g_fScale -= 0.1f;
		}
	}
	else
	{
		if (g_fScale <= MAX_SCALE)
		{
			g_fScale += 0.1f;
		}
	}

	if (g_fScale == 0)
	{
		if (bShrink)
		{
			g_fScale -= 0.1f;
		}
		else
		{
			g_fScale += 0.1f;
		}
	}

	return;
}

void GraphicBackground::Reset()
{
	g_fScale = DEFALUT_SCALE;
	g_ptOffset.setX(0);
	g_ptOffset.setY(0);

	return;
}

QCursor GraphicBackground::Drag(const QPoint& ptWinPoint)
{
	QCursor cursor;
	m_ptLastpoint = ptWinPoint;

	if (m_bMove)
	{
		g_ptOffset = m_ptLastpoint - m_ptPrepoint + m_ptLocal;

		QPoint ptLeftTop = winToVec(QPoint(g_rcClient.left(), g_rcClient.top()));
		QPoint ptRightBottom = winToVec(QPoint(g_rcClient.right(), g_rcClient.bottom()));

		if (ptLeftTop.x() < -g_sizeVector.width() / 2
			|| ptLeftTop.y() > g_sizeVector.height() / 2
			|| ptRightBottom.x() > g_sizeVector.width() / 2
			|| ptRightBottom.y() < -g_sizeVector.height() / 2)
		{
			g_ptOffset = m_ptLocal;
		}
		cursor = Qt::OpenHandCursor;
	}
	else
	{
		m_ptLocal = g_ptOffset;
		cursor = Qt::ArrowCursor;
	}

	return cursor;
}

void GraphicBackground::Select(const QPoint& ptWinPoint)
{
	m_ptPrepoint = ptWinPoint;
	m_ptLastpoint = ptWinPoint;

	m_bMove = true;

	return;
}

void GraphicBackground::Cancel()
{
	g_ptOffset = m_ptLocal;

	m_bMove = false;

	return;
}

void GraphicBackground::Confirm()
{
	m_bMove = false;

	return;
}

void GraphicBackground::Draw(QPainter& painter)
{
	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	QPixmap icon = QPixmap::fromImage(m_pImage);

	painter.drawPixmap(QRect(g_rcClient.left(), g_rcClient.top(), g_rcClient.width(), g_rcClient.height()), icon);

	m_mutexLock.unlock();

	return;
}

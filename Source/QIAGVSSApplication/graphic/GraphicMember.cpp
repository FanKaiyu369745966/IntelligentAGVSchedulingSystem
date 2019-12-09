#include "GraphicMember.h"

std::chrono::steady_clock::time_point GraphicMember::m_tmptFlash = std::chrono::steady_clock::time_point();
bool GraphicMember::g_bFlash = false;

GraphicMember::GraphicMember()
	: m_ptCenter(0, 0)
{
}

void GraphicMember::StartFlash()
{
	if (m_tmptFlash != std::chrono::steady_clock::time_point())
	{
		auto _dis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_tmptFlash);

		if (_dis.count() > 1000)
		{
			g_bFlash = !g_bFlash;
			m_tmptFlash = std::chrono::steady_clock::now();
		}
	}
	else
	{
		g_bFlash = true;
		m_tmptFlash = std::chrono::steady_clock::now();
	}

	return;
}

void GraphicMember::SetCenterPoint(const QPoint& ptCenter)
{
	m_ptCenter = ptCenter;

	return;
}

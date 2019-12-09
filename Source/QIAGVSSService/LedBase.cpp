#include "LedBase.h"

LedBase::LedBase()
	: m_usId(0)
	, m_usWidth(0)
	, m_usHeight(0)
{
}

LedBase::LedBase(const unsigned short& id, const unsigned short& width, const unsigned short& height)
	: m_usId(id)
	, m_usWidth(width)
	, m_usHeight(height)
{
}

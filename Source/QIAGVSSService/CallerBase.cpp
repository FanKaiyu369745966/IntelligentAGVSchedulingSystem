#include "CallerBase.h"

CallerBase::CallerBase()
	: m_usId(0)
	, m_byType(CallerType::Type_None)
	, m_usStation(0)
	, m_bUse(false)
{
}

CallerBase::CallerBase(const CallerType& type, const unsigned short& id, const unsigned short& station, const bool& use)
	: m_usId(id)
	, m_byType(type)
	, m_usStation(station)
	, m_bUse(use)
{
}

CallerBase::CallerBase(const CallerBase& caller)
{
	m_usId = caller.m_usId;
	m_byType = caller.m_byType;
	m_usStation = caller.m_usStation;
	m_bUse = caller.m_bUse;
}

bool CallerBase::IsNull() const
{
	if (m_usId == 0)
	{
		return true;
	}

	return false;
}

CallerBase::operator bool() const
{
	return !(IsNull());
}

bool CallerBase::operator==(const bool& _bool) const
{
	return (!IsNull()) && _bool;
}

bool CallerBase::operator!=(const bool&) const
{
	return false;
}

void CallerBase::operator=(const CallerBase& caller)
{
	m_usId = caller.m_usId;
	m_byType = caller.m_byType;
	m_usStation = caller.m_usStation;
	m_bUse = caller.m_bUse;

	return;
}

#include "StationBase.h"

StationBase::StationBase()
	: m_usId(0)
	, m_location(0)
	, m_byType(StationType::Type_None)
	, m_strName("")
{
}

StationBase::StationBase(const unsigned short& id, const StationType& type, const rfid_t& location, const QString& name)
	: m_usId(id)
	, m_location(location)
	, m_byType(type)
	, m_strName(name)
{
}

StationBase::StationBase(const Station& station)
{
	m_usId = station.m_usId;
	m_location = station.m_location;
	m_byType = station.m_byType;
	m_strName = station.m_strName;
}

StationBase::operator bool() const
{
	return !IsNull();
}

bool StationBase::operator==(const bool& _bool) const
{
	return ((!IsNull()) == _bool);
}

bool StationBase::operator!=(const bool& _bool) const
{
	return !((!IsNull()) == _bool);
}

void StationBase::operator=(const Station& station)
{
	m_usId = station.m_usId;
	m_location = station.m_location;
	m_byType = station.m_byType;
	m_strName = station.m_strName;

	return;
}

bool StationBase::IsNull() const
{
	if (m_usId == 0
		|| m_byType == StationType::Type_None)
	{
		return true;
	}

	return false;
}

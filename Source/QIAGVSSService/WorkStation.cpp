#include "WorkStation.h"

WorkStation::WorkStation()
{
}

WorkStation::WorkStation(const unsigned short& id, const rfid_t& location, const QString& name)
	: StationBase(id, StationType::Type_WorkStation, location, name)
{
	if (name.isNull() || name.isEmpty())
	{
		m_strName = QString::fromLocal8Bit("工作站%1").arg(id);
	}
}

WorkStation::WorkStation(const WorkStation& station)
{
	this->m_usId = station.m_usId;
	this->m_location = station.m_location;
	this->m_strName = station.m_strName;
	this->m_byType = station.m_byType;
}

void WorkStation::operator=(const WorkStation& station)
{
	this->m_usId = station.m_usId;
	this->m_location = station.m_location;
	this->m_strName = station.m_strName;
	this->m_byType = station.m_byType;
}

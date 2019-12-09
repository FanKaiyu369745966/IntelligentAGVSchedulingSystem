#include "ChargeStation.h"

ChargeStation::ChargeStation()
	: m_usCtrler(0)
{
}

ChargeStation::ChargeStation(const unsigned short& id, const rfid_t& location, const QString& name)
	: StationBase(id, StationType::Type_ChargeStation, location, name)
	, m_usCtrler(0)
{
	if (name.isNull() || name.isEmpty())
	{
		m_strName = QString::fromLocal8Bit("充电站%1").arg(id);
	}
}

ChargeStation::ChargeStation(const unsigned short& id, const rfid_t& location, const unsigned short& ctrler, const QString& name)
	: StationBase(id, StationType::Type_ChargeStation, location, name)
	, m_usCtrler(ctrler)
{
	if (name.isNull() || name.isEmpty())
	{
		m_strName = QString::fromLocal8Bit("充电站%1").arg(id);
	}
}

ChargeStation::ChargeStation(const ChargeStation& charger)
{
	this->m_usId = charger.m_usId;
	this->m_strName = charger.m_strName;
	this->m_location = charger.m_location;
	this->m_byType = charger.m_byType;
	this->m_usCtrler = charger.m_usCtrler;
}

void ChargeStation::operator=(const ChargeStation& charger)
{
	this->m_usId = charger.m_usId;
	this->m_strName = charger.m_strName;
	this->m_location = charger.m_location;
	this->m_byType = charger.m_byType;
	this->m_usCtrler = charger.m_usCtrler;

	return;
}

void ChargeStation::SetControler(const unsigned short& id)
{
	m_usCtrler = id;

	return;
}

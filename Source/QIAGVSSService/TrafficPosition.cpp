#include "TrafficPosition.h"

bool TrafficPosition::IsNull() const
{
	if (m_rfidCtrl == 0)
	{
		return true;
	}

	return false;
}

void TrafficPosition::LoadLockList(const QJsonArray& json)
{
	if (json.isEmpty())
	{
		return;
	}

	m_setLock.clear();

	for (QJsonArray::const_iterator it = json.begin(); it != json.end(); ++it)
	{
		m_setLock.insert(it->toInt());
	}

	return;
}

void TrafficPosition::LoadCompareList(const QJsonArray& json)
{
	if (json.isEmpty())
	{
		return;
	}

	m_setCompare.clear();

	for (QJsonArray::const_iterator it = json.begin(); it != json.end(); ++it)
	{
		m_setCompare.insert(it->toInt());
	}

	return;
}

TrafficPosition::TrafficPosition()
	: m_rfidCtrl(0)
{
}

TrafficPosition::TrafficPosition(const rfid_t& ctrl)
	: m_rfidCtrl(ctrl)
{
}

TrafficPosition::TrafficPosition(const TrafficPosition& pos)
{
	m_rfidCtrl = pos.m_rfidCtrl;
	m_setCompare = pos.m_setCompare;
	m_setLock = pos.m_setLock;
}

TrafficPosition::operator bool() const
{
	return !IsNull();
}

bool TrafficPosition::operator==(const bool& _bool) const
{
	return (!IsNull()) == _bool;
}

bool TrafficPosition::operator!=(const bool& _bool) const
{
	return !(!IsNull()) == _bool;
}

void TrafficPosition::operator=(const TrafficPosition& pos)
{
	m_rfidCtrl = pos.m_rfidCtrl;
	m_setCompare = pos.m_setCompare;
	m_setLock = pos.m_setLock;
}

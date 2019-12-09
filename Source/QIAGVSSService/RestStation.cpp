#include "RestStation.h"

RestStation::RestStation()
	: m_usQueueId(0)
{
}

RestStation::RestStation(const unsigned short& id, const rfid_t& location, const QString& name)
	: StationBase(id, StationType::Type_RestStation, location, name)
	, m_usQueueId(0)
{
	if (name.isNull() || name.isEmpty())
	{
		m_strName = QString::fromLocal8Bit("待机站%1").arg(id);
	}
}

RestStation::RestStation(const RestStation& rest)
{
	this->m_usId = rest.m_usId;
	this->m_strName = rest.m_strName;
	this->m_location = rest.m_location;
	this->m_byType = rest.m_byType;
	this->m_usQueueId = rest.m_usQueueId;
	this->m_chainLock = rest.m_chainLock;
}

void RestStation::operator=(const RestStation& rest)
{
	this->m_usId = rest.m_usId;
	this->m_strName = rest.m_strName;
	this->m_location = rest.m_location;
	this->m_byType = rest.m_byType;
	this->m_usQueueId = rest.m_usQueueId;
	this->m_chainLock = rest.m_chainLock;

	return;
}

void RestStation::LoadLockArea(const QJsonArray& json)
{
	if (json.isEmpty())
	{
		return;
	}

	m_chainLock.Clear();

	for (QJsonArray::const_iterator it = json.begin(); it != json.end(); ++it)
	{
		m_chainLock.PushBack(it->toInt());
	}

	return;
}

void RestStation::SetQueueID(const unsigned short& id)
{
	m_usQueueId = id;

	return;
}

LocatArray RestStation::GetLockArea() const
{
	LocatArray arr;

	for (int i = 0; i < m_chainLock.Size(); ++i)
	{
		arr.insert(m_chainLock.Get(i));
	}

	return arr;
}

rfid_t RestStation::GetNextLock(const rfid_t& rfid) const
{
	int index = m_chainLock.FindNode(rfid);

	if (index == -1)
	{
		return 0;
	}

	return m_chainLock.Get(++index);
}

rfid_t RestStation::GetFirstLock() const
{
	return m_chainLock.GetFirst();
}

bool RestStation::IsInside(const rfid_t& rfid) const
{
	if (rfid == m_location)
	{
		return true;
	}

	if (m_chainLock.FindNode(rfid) != -1)
	{
		return true;
	}

	return false;
}

RestStationQueue::RestStationQueue()
	:m_usId(0)
{
}

RestStationQueue::RestStationQueue(const unsigned short& id)
	: m_usId(id)
{
}

RestStationQueue::RestStationQueue(const RestStationQueue& queue)
{
	m_usId = queue.m_usId;
	m_chainRest = queue.m_chainRest;
}

RestStationQueue::operator bool() const
{
	return !IsNull();
}

bool RestStationQueue::operator==(const bool& _bool) const
{
	return (!IsNull()) == _bool;
}

bool RestStationQueue::operator!=(const bool& _bool) const
{
	return !((!IsNull()) == _bool);
}

void RestStationQueue::operator=(const RestStationQueue& queue)
{
	m_usId = queue.m_usId;
	m_chainRest = queue.m_chainRest;
}

bool RestStationQueue::IsNull() const
{
	if (m_usId == 0
		|| m_chainRest.Size() == 0)
	{
		return true;
	}

	return false;
}

void RestStationQueue::LoadQueue(const QJsonArray& json)
{
	m_chainRest.Clear();

	for (QJsonArray::const_iterator it = json.begin(); it != json.end(); ++it)
	{
		m_chainRest.PushBack(it->toInt());
	}

	return;
}

unsigned short RestStationQueue::GetPrevStation(const unsigned short& rest) const
{
	int index = m_chainRest.FindNode(rest);

	if (index == -1)
	{
		return 0;
	}

	return m_chainRest.Get(--index);
}

unsigned short RestStationQueue::GetNextStation(const unsigned short& rest) const
{
	int index = m_chainRest.FindNode(rest);

	if (index == -1)
	{
		return 0;
	}

	return m_chainRest.Get(++index);
}

bool RestStationQueue::IsInside(const unsigned short& rest) const
{
	int index = m_chainRest.FindNode(rest);

	if (index == -1)
	{
		return false;
	}

	return true;
}

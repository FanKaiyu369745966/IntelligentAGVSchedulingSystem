#include "ZigbeeCaller.h"

ZigbeeCaller::ZigbeeCaller()
	: CallerBase(CallerType::Type_Zigbee, 0)
	, m_strAddr("")
{
}

ZigbeeCaller::ZigbeeCaller(const unsigned short& id, std::string addr, const unsigned short& station, const bool& use)
	: CallerBase(CallerType::Type_Zigbee, id, station, use)
	, m_strAddr(addr)
{
}

ZigbeeCaller::ZigbeeCaller(const ZigbeeCaller& zig)
{
	m_usId = zig.m_usId;
	m_byType = zig.m_byType;
	m_usStation = zig.m_usStation;
	m_bUse = zig.m_bUse;
	m_strAddr = zig.m_strAddr;
	m_attribute = zig.m_attribute;
}

void ZigbeeCaller::operator=(const ZigbeeCaller& zig)
{
	m_usId = zig.m_usId;
	m_byType = zig.m_byType;
	m_usStation = zig.m_usStation;
	m_bUse = zig.m_bUse;
	m_strAddr = zig.m_strAddr;
	m_attribute = zig.m_attribute;

	return;
}
void ZigbeeCaller::SetAddress(const std::string& addr)
{
	m_strAddr = addr;

	return;
}

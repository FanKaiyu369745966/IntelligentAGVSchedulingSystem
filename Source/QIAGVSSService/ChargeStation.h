#ifndef CHARGESTATION_H
#define CHARGESTATION_H

#include "StationBase.h"

class ChargeStation;
typedef std::shared_ptr<ChargeStation> ChargerPtr;
typedef std::map<unsigned short, ChargerPtr> ChargerArray;

class ChargeStation :
	public StationBase
{
public:
	ChargeStation();
	explicit ChargeStation(const unsigned short& id, const rfid_t& location, const QString& name = "");
	explicit ChargeStation(const unsigned short& id, const rfid_t& location, const unsigned short& ctrler, const QString& name = "");
	ChargeStation(const ChargeStation& charger);

public:
	void operator=(const ChargeStation& charger);

public:
	unsigned short m_usCtrler;	/*!< 充电桩编号 */

public:
	unsigned short GetControler() const { return m_usCtrler; }

	void SetControler(const unsigned short& id);
};

#endif // CHARGESTATION_H

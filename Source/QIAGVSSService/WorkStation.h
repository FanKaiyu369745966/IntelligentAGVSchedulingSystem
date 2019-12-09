/*!
 * @file WorkStation
 * @breif 描述工作工站的属性及功能的文件
 * @date 2019-11-01
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef WORKSTATION_H
#define WORKSTATION_H

#include "StationBase.h"

class WorkStation;
typedef std::shared_ptr<WorkStation> WorkerPtr;
typedef std::map<unsigned short, WorkerPtr> WorkerArray;

/*!
 * @class WorkStation
 * @brief 描述工作工站属性及功能的文件
*/
class WorkStation :
	public StationBase
{
public:
	WorkStation();
	explicit WorkStation(const unsigned short& id, const rfid_t& location, const QString& name = "");
	WorkStation(const WorkStation& station);

public:
	void operator=(const WorkStation& station);
};

#endif //WORKSTATION_H

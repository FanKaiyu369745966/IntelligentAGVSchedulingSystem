/*!
 * @file TrafficPosition
 * @brief 描述交通管制的属性与功能的文件
 * @date 2019-11-05
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef TRAFFICPOSITION_H
#define TRAFFICPOSITION_H

#include <set>
#include <map>
#include <QJsonObject>
#include <QJsonArray>
#include "RfidBase.h"

typedef class TrafficPosition TrafficPos;
typedef std::map < rfid_t, TrafficPos> TrafficArray;

/*!
 * @class TrafficPosition
 * @brief 描述交通管制点得属性与功能的类
*/
class TrafficPosition
{
protected:
	rfid_t m_rfidCtrl;				/*!< 交通管制RFID地标卡编号 */
	std::set<rfid_t> m_setLock;		/*!< 交通管制锁定RFID地标卡集合:此集合中的RFID卡存在AGV时,交通管制RFID地标卡上的AGV不能放行 */
	std::set<rfid_t> m_setCompare;	/*!< 交通管制比对RFID地标卡集合:交通管制RFID卡与比对地标卡集合进行比对,依照先到先行的规则放行 */

public:
	TrafficPosition();
	explicit TrafficPosition(const rfid_t& ctrl);
	TrafficPosition(const TrafficPosition& pos);

public:
	operator bool() const;
	bool operator==(const bool& _bool) const;
	bool operator!=(const bool& _bool) const;
	void operator=(const TrafficPosition& pos);

public:
	bool IsNull() const;
	rfid_t GetCtrlPos() const { return m_rfidCtrl; }
	LocatArray GetLockList() const { return m_setLock; }
	LocatArray GetCompareList() const { return m_setCompare; }

	void LoadLockList(const QJsonArray& json);
	void LoadCompareList(const QJsonArray& json);
};

#endif // TRAFFICPOSITION_H

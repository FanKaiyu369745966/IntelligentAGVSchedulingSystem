/*!
 * @file RestStation
 * @breif 描述待机工站的属性及功能的文件
 * @date 2019-11-04
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef RESTSTATION_H
#define RESTSTATION_H

#include <QJsonObject>
#include <QJsonArray>
#include "StationBase.h"
#include "ChainTable.h"

class RestStation;
typedef class RestStationQueue RestQueue;
typedef std::shared_ptr<RestStation> ResterPtr;
typedef std::map<unsigned short, ResterPtr> ResterArray;
typedef std::map<unsigned short, RestQueue> RestQueueArray;

class RestStation :
	public StationBase
{
protected:
	unsigned short m_usQueueId;			/*!< 队列编号 */
	DoubleChain<rfid_t> m_chainLock;	/*!< 锁定区域:此区域RFID卡上存在AGV时,不能够向此待机站移动 */

public:
	RestStation();
	explicit RestStation(const unsigned short& id, const rfid_t& location, const QString& name = "");
	RestStation(const RestStation& rest);

public:
	void operator=(const RestStation& rest);

public:
	/*!
	 * @brief 读取锁定区域
	 * @param const QJsonArray&
	*/
	void LoadLockArea(const QJsonArray& json);

	/*!
	 * @brief 设置队列编号
	 * @param const unsigned short& 队列编号
	*/
	void SetQueueID(const unsigned short& id);

	/*!
	 * @brief 获取队列编号
	 * @return unsigned short 队列编号
	*/
	unsigned short GetQueueID() const { return m_usQueueId; }

	/*!
	 * @brief 获取锁定区域
	 * @return LocatArray 锁定区域
	*/
	LocatArray GetLockArea() const;

	/*!
	 * @brief 获取下一锁
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const rfid_t & 当前锁FID地标卡
	 * @return rfid_t 下一锁RFID地标卡
	 */
	rfid_t GetNextLock(const rfid_t&) const;

	/*!
	 * @brief 获取第一个锁
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return rfid_t 锁RFID地标卡
	 */
	rfid_t GetFirstLock() const;

	int GetLockSize() const { return m_chainLock.Size(); }

	bool IsInside(const rfid_t&) const;
};

/*!
 * @class RestStationList
 * @brief 描述待机队列的属性与功能
*/
class RestStationQueue
{
protected:
	unsigned short m_usId;						/*!< 编号 */
	DoubleChain<unsigned short> m_chainRest;	/*!< 待机链表 */

public:
	RestStationQueue();
	explicit RestStationQueue(const unsigned short& id);
	RestStationQueue(const RestStationQueue& queue);

public:
	operator bool() const;
	bool operator== (const bool& _bool) const;
	bool operator!= (const bool& _bool) const;
	void operator=(const RestStationQueue& queue);

public:
	bool IsNull() const;

public:
	unsigned short GetId() const { return m_usId; }

	/*!
	 * @brief 读取队列
	 * @param const QJsonArray&
	*/
	void LoadQueue(const QJsonArray& json);

	/*!
	 * @brief 获取上一待机站
	 * @param const unsigned short& 当前待机站
	 * @return unsigned short 返回待机站编号
	*/
	unsigned short GetPrevStation(const unsigned short& rest) const;

	/*!
	 * @brief 获取下一待机站
	 * @param const unsigned short& 当前待机站
	 * @return unsigned short 返回待机站编号
	*/
	unsigned short GetNextStation(const unsigned short& rest) const;

	unsigned short GetHead() const { return m_chainRest.GetFirst(); }
	unsigned short GetTail() const { return m_chainRest.GetLast(); }

	/*!
	 * @brief 判断待机点是否包含在待机队列内
	*/
	bool IsInside(const unsigned short& rest) const;
};

#endif // RESTSTATION_H

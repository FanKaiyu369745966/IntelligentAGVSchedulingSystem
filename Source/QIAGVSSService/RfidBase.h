/*!
 * @file RfidBase
 * @brief 描述RFID地标卡基本属性与功能的文件
 * @date 2019-10-30
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef RFIDBASE_H
#define RFIDBASE_H

#include <chrono>
#include <memory>
#include <map>
#include <set>

typedef class RfidBase RFID;
typedef std::shared_ptr<RFID> RFIDPtr;
typedef unsigned short rfid_t;
typedef std::map < rfid_t, RFIDPtr> RFIDArray;
typedef std::set<rfid_t> LocatArray;

/*!
 * @class RfidBase
 * @brief 描述RFID地标卡的基本属性与功能的类
*/
class RfidBase
{
public:
	RfidBase();
	explicit RfidBase(const rfid_t& id);
	RfidBase(const RfidBase& rfid);

public:
	operator bool() const;
	bool operator==(const bool& _bool) const;
	bool operator!=(const bool& _bool) const;
	void operator=(const RfidBase& rfid);
	bool operator>(const RfidBase& rfid) const;
	bool operator<(const RfidBase& rfid) const;

protected:
	rfid_t m_id;                                        /*!< 编号 */
	std::chrono::steady_clock::time_point m_lockTime;   /*!< 锁定RFID卡的时间 */
	const void* m_pLocker;                              /*!< 锁定RFID卡的对象 */
	const void* m_pPeerLocker;                          /*!< 远程锁定RFID卡的对象:即对象不在RFID卡上,提前锁定RFID卡 */

public:
	/*!
	 * @brief 获取RFID地标卡编号
	 * @return rifd_t RFID地标卡编号
	*/
	rfid_t GetId() const { return m_id; }

	/*!
	 * @brief 获取RFID卡的锁定者
	 * @return void* 指向RFID卡锁定者的指针
	*/
	const void* GetLocker() const { return m_pLocker; }

	/*!
	 * @brief 获取RFID卡的预锁定者
	 * @return void* 指向RFID卡预锁定者的指针
	*/
	const void* GetPeerLocker() const { return m_pPeerLocker; }

	/*!
	 * @brief 锁定RFID卡
	 * @param void* 指向锁定者的指针
	 * @return bool 锁定成功返回true,否则返回false
	*/
	bool Lock(const void* _locker);

	/*!
	 * @brief 解除RFID卡的锁定
	 * @param void* 指向锁定者的指针
	 * @return bool 解除成功返回true,否则返回false
	*/
	bool Free(const void* _locker = nullptr);

	/*!
	 * @brief 预锁定RFID卡的锁定
	 * @param void* 指向预锁定者的指针
	 * @return bool 解除成功返回true,否则返回false
	*/
	bool PeerLock(const void* _locker);

	/*!
	 * @brief 取消RFID卡的预锁定
	 * @param void* 指向预锁定者的指针
	 * @return bool 接触成功返回true,否则返回false
	*/
	bool Cancel(const void* _locker = nullptr);

	/*!
	 * @brief 解除RFID卡的所有锁定
	 * @param void* 指向预锁定者的指针
	*/
	void Clear(const void* _locker = nullptr);
};

#endif // RFIDBASE_H

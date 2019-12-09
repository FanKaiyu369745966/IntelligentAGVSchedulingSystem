/*!
 * @file StationBase
 * @brief 描述工站基础属性与功能的文件
 * @date 2019-10-31
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef STATIONBASE_H
#define STATIONBASE_H

#include <QString>
#include <memory>
#include <map>
#include "RfidBase.h"

typedef class StationBase Station;

enum class StationType : unsigned char
{
	Type_None,
	Type_WorkStation = 1,	/*!< 工站 */
	Type_RestStation,		/*!< 待机站/休息站 */
	Type_ChargeStation,		/*!< 充电站 */
};

/*!
 * @class StationBase
 * @brief 描述工站基础属性与功能的类
*/
class StationBase
{
public:
	StationBase();
	explicit StationBase(const unsigned short& id, const StationType& type, const rfid_t& location, const QString& name = "");
	StationBase(const Station& station);

protected:
	unsigned short m_usId;		/*!< 编号 */
	QString m_strName;		/*!< 名称 */
	StationType m_byType;		/*!< 类型 */
	rfid_t m_location;	/*!< 坐标 */

public:
	operator bool() const;
	bool operator==(const bool& _bool) const;
	bool operator!=(const bool& _bool) const;
	void operator=(const Station& station);

public:
	bool IsNull() const;

	/*!
	 * @brief 获取编号
	 * @return unsigend short 编号
	*/
	unsigned short GetId() const { return m_usId; }

	/*!
	 * @brief 获取名称
	 * @return std::string 名称
	*/
	QString GetName() const { return m_strName; }

	/*!
	 * @breif 获取类型
	 * @return unsigend char 类型
	*/
	StationType GetType() const { return m_byType; }

	/*!
	 * @brief 获取坐标
	 * @return rfid_t 坐标RFID地标卡编号
	*/
	rfid_t GetLocation() const { return m_location; }

	/*!
	 * @brief 设置名称
	 * @param const string& 新的名称
	*/
	void SetName(const QString& name) { m_strName = name; return; }

	/*!
	 * @brief 设置坐标
	 * @param const rfid_t& 新的坐标
	*/
	void SetLocation(const rfid_t& rfid) { m_location = rfid; return; }
};

#endif // STATIONBASE_H

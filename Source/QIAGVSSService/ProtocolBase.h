/*!
 * @file ProtocolBase
 * @brief 协议类型的基础属性与功能文件
 * @date 2019-10-28
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef PROTOCOLBASE_H
#define PROTOCOLBASE_H

#include <QObject>
#include <cstring>
#include "ByteArray.h"

enum class ProtocolType : unsigned char
{
	Protocol_Stm32,		/*!< 与STM32的通信协议 */
	Protocol_Plc,		/*!< 与PLC的通信协议 */
	Protocol_Zigbee,	/*!< 与Zigbee的通信协议 */
};

/*!
 * @class ProtocolBase
 * @brief 协议类型的基础属性与功能的类
*/
class ProtocolBase
{
public:
	explicit ProtocolBase(const ProtocolType& byType);

public:
	typedef short crc16_t;

protected:
	static unsigned char auchCRCHi[];
	static unsigned char auchCRCLo[];

protected:
	/*!
	 * @brief CRC16校验
	 * @param const char* 数据首地址指针
	 * @param unsigned int 数据大小
	 * @return crc16_t 返回校验码
	*/
	static crc16_t CRC16(const ByteArray&);
	static crc16_t CRC16(const unsigned char*, const int&);

	/*!
	 * @brief 数据和校验
	 * @date 2019/12/03
	 * @author FanKaiyu
	 * @param const char* 数据首地址指针
	 * @param unsigned int 数据大小
	 * @return ProtocolBase::crc16_t 返回校验码
	 */
	static crc16_t SUM16(const ByteArray&);
	static crc16_t SUM16(const unsigned char*, const int&);

protected:
	ProtocolType m_byType;	/*!< 协议类型 */

public:
	/*!
	 * @brief 获取协议类型
	 * @return unsigned char 协议类型(ProtocolType枚举)
	*/
	ProtocolType GetType() const;

public:
	/*!
	 * @brief 合成报文包
	 * @param const QByteArray& 数据体内容
	 * @return QByteArray 合成后报文包
	*/
	virtual ByteArray Create(const ByteArray&) const = 0;
	virtual ByteArray Create(const unsigned char*, const int&) const = 0;

	/*!
	 * @brief 处理数据
	 * @param QByteArray& 数据内容,在数据解析完成后,返回剩余的未处理的数据
	 * @return QByteArrayList 解析后的数据体内容列表
	*/
	virtual ByteArrayList Process(ByteArray&) const = 0;
};

#endif // PROTOCOLBASE_H

#ifndef PROTOCOLPLC_H
#define PROTOCOLPLC_H

#include "ProtocolBase.h"

class ProtocolPlc :
	public ProtocolBase
{
public:
	ProtocolPlc();

public:
protected:
	static const unsigned char g_head;		/*!< 报文头 */
	static const unsigned char g_tail;		/*!< 报文尾 */
	static const unsigned char g_encode;	/*!< 转义符 */

public:
	typedef unsigned short length_t;

protected:
	unsigned char m_byCode;	/*!< 设备编码 */

public:
	/*!
	 * @brief 合成报文包
	 * @param const QByteArray& 数据体内容
	 * @return QByteArray 合成后报文包
	*/
	ByteArray Create(const ByteArray& data) const;
	ByteArray Create(const unsigned char* data, const int& maxSize) const;

	/*!
	 * @brief 处理数据
	 * @param QByteArray& 数据内容,在数据解析完成后,返回剩余的未处理的数据
	 * @return QByteArrayList 解析后的数据体内容列表
	*/
	ByteArrayList Process(ByteArray& data) const;
};
#endif // PROTOCOLPLC_H

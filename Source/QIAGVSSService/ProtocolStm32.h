/*!
 * @file ProtocolStm32
 * @brief 描述与STM32通信协议的属性以及方法的文件
 * @date 2019-10-29
 * @author FanKaiyu
 * @version 1.0
*/
#ifndef PROTOCOLSTM32_H
#define PROTOCOLSTM32_H

#include "ProtocolBase.h"

/*!
 * @class ProtocolStm32
 * @brief 描述与STM32通信西医的属性以及方法的类
*/
class ProtocolStm32 :
	public ProtocolBase
{
public:
	ProtocolStm32();

protected:
	static const unsigned char g_head;		/*!< 报文头 */
	static const unsigned char g_tail;		/*!< 报文尾 */
	static const unsigned char g_encode;	/*!< 转义符 */

public:
	typedef unsigned short length_t;

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

protected:
	/*!
	 * @brief 报文转义
	 *
	 * 0xBA 转义为 0xB0 0x01
	 * 0xBE 转义为 0xB0 0x02
	 * 0xB0 转义为 0xB0 0x00
	 * @param const char* 指向数据体首地址的指针
	 * @param const int& 数据大小
	 * @return QByteArray 返回转义后的数据
	*/
	ByteArray Encoding(const unsigned char* data, const int& maxSize) const;

	/*!
	 * @brief 报文反转义
	 *
	 * 0xB0 0x01 转义为 0xBA
	 * 0xB0 0x02 转义为 0xBE
	 * 0xB0 0x00 转义为 0xB0
	 * @param const char* 指向数据体首地址的指针
	 * @param const int& 数据大小
	 * @return QByteArray 返回反转义后的数据
	*/
	ByteArray Decoding(const unsigned char* data, const int& maxSize) const;
};

#endif // PROTOCOLSTM32_H

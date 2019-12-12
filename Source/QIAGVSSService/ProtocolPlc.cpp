#include "ProtocolPlc.h"
#include <QDebug>

const unsigned char ProtocolPlc::g_head = 0xBA;
const unsigned char ProtocolPlc::g_tail = 0xBE;
const unsigned char ProtocolPlc::g_encode = 0xB0;

ProtocolPlc::ProtocolPlc()
	: ProtocolBase(ProtocolType::Protocol_Plc)
{
}

ByteArray ProtocolPlc::Create(const ByteArray& data) const
{
	return Create(data.get(), data.size());
}

ByteArray ProtocolPlc::Create(const unsigned char* data, const int& maxSize) const
{
	size_t len = 6 + maxSize;	/*!< 报文长度 */

	std::unique_ptr<unsigned char[]> source(new unsigned char[len]);	/*!< 源数据 */

	// 初始化报文包
	std::memset(source.get(), 0, len);

	int index = 0;	/*!< 下标 */

	// 存储报文头
	source[index++] = g_head;

	// 存储报文长度
	source[index++] = (len >> 8) & 0xFF;
	source[index++] = len & 0xFF;

	// 存储数据体
	std::memcpy(source.get() + index, data, maxSize);

	crc16_t crc = CRC16(source.get() + 1, 2 + maxSize);	/*!< 校验码 */

	index += maxSize;

	// 存储校验码
	source[index++] = (crc >> 8) & 0xFF;
	source[index++] = crc & 0xFF;

	// 存储报文尾
	source[index++] = g_tail;

	//QString _debug = "Create Packet:";
	//for (int i = 0; i < index; ++i)
	//{
	//	_debug += QString::fromLocal8Bit("%1 ").arg(*(source.get() + i) & 0xFF, 2, 16, QLatin1Char('0'));
	//}

	//qDebug() << _debug << endl;

	//return Encoding(source.get(), index);
	return ByteArray(source.get(), index);
}

ByteArrayList ProtocolPlc::Process(ByteArray& data) const
{
	ByteArrayList list;				/*!< 数据包列表 */


	const unsigned char* begin = data.get();	/*!< 数据首地址指针 */
	int size = data.size();						/*!< 数据长度 */

	const unsigned char* remain = begin;		/*!< 指向待处理数据首地址的指针 */
	int reminSize = size;						/*!< 待处理数据大小 */

	const int min_size = sizeof(g_head) + sizeof(g_tail) + sizeof(crc16_t) + sizeof(length_t);	/*!< 最小数据长度 */

	while (1)
	{
		// 查找报文头
		const unsigned char* head = reinterpret_cast<const unsigned char*>(memchr(remain, g_head, reminSize));		/*!< 报文头 */

		if (head == nullptr)
		{
			// 未找到报文头

			remain = nullptr;
			reminSize = 0;

			break;
		}

		remain = head;
		reminSize = size - (remain - begin);

		if (reminSize < min_size)
		{
			// 待处理数据长度不足
			break;
		}

		// 查找报文尾
		const unsigned char* tail = reinterpret_cast<const unsigned char*>(memchr(head + sizeof(g_head), g_tail, reminSize - sizeof(g_head)));

		if (tail == nullptr)
		{
			// 未找到报文尾

			break;
		}

		remain = tail + sizeof(g_tail);
		reminSize = size - (remain - begin);

		// 反转义
		ByteArray source = Decoding(head, (tail + sizeof(g_tail)) - head);

		// 源数据:报文头 + 数据长度(包含报文头尾以及校验位) + 数据体 + 校验位(校验数据体+数据长度) + 报文尾

		const unsigned char* sourcePtr = source.get();		/*!< 源数据首地址指针 */
		int sourceLen = source.size();					/*!< 源数据长度 */

		// 获取报文长度
		length_t packetLen = 0;							/*!< 报文包上传的数据长度 */

		// 指向数据长度首地址
		const unsigned char* packetData = sourcePtr + sizeof(g_head);	/*!< 指向数据包首地址的指针 */

		// 获取报文长度
		for (int i = 0; i < sizeof(length_t); ++i)
		{
			unsigned char tmp = *(packetData + i);
			packetLen |= (tmp << 8 * (sizeof(length_t) - i - 1));
		}

		// 校验数据长度
		if (packetLen != sourceLen)
		{
			QString _debug;
			for (int i = 0; i < (tail + sizeof(g_tail)) - head; ++i)
			{
				_debug += QString::fromLocal8Bit("%1 ").arg(*(head + i) & 0xFF, 2, 16, QLatin1Char('0'));
			}

			qDebug() << _debug << endl;

			// 数据长度不相符
			continue;
		}

		// 获取校验码

		// 指向校验位首地址
		packetData += (packetLen - min_size + sizeof(length_t));

		crc16_t packetCrc = 0;	/*!< 报文上传的校验码 */
		crc16_t sourceCrc = 0;	/*!< 源数据校验码 */

		// 获取校验码
		for (int i = 0; i < sizeof(crc16_t); ++i)
		{
			unsigned char tmp = *(packetData + i);
			packetCrc |= (tmp << 8 * (sizeof(length_t) - i - 1));
		}

		// 指向数据长度首地址
		packetData = sourcePtr + sizeof(g_head);

		// 获取源数据校验码
		sourceCrc = CRC16(packetData, sourceLen - min_size + sizeof(length_t));

		if (sourceCrc != packetCrc)
		{
			QString _debug;
			for (int i = 0; i < (tail + sizeof(g_tail)) - head; ++i)
			{
				_debug += QString::fromLocal8Bit("%1 ").arg(*(head + i) & 0xFF, 2, 16, QLatin1Char('0'));
			}

			qDebug() << _debug << endl;

			// 校验码不符
			continue;
		}

		// 指向数据体首地址
		packetData += sizeof(length_t);

		// 存入列表
		list.push_back(ByteArray(packetData, sourceLen - min_size));
	}

	if (reminSize <= 0)
	{
		data.clear();
	}
	else
	{
		data = ByteArray(remain, reminSize);
	}

	return list;
}

ByteArray ProtocolPlc::Encoding(const unsigned char* data, const int& maxSize) const
{
	int len = 0, transf = 0;

	len = maxSize * 2;

	std::unique_ptr<unsigned char[]> encode(new unsigned char[len]);

	std::memset(encode.get(), 0, len);

	encode[transf++] = g_head;

	// 转义数据体
	for (int i = sizeof(g_head); i < maxSize - sizeof(g_tail); ++i, ++transf)
	{
		switch (data[i])
		{
		case g_head:
			encode[transf++] = g_encode;
			encode[transf] = 0x01;
			break;
		case g_tail:
			encode[transf++] = g_encode;
			encode[transf] = 0x02;
			break;
		case g_encode:
			encode[transf++] = g_encode;
			encode[transf] = 0x00;
			break;
		default:
			encode[transf] = data[i];
			break;
		}
	}

	encode[transf++] = g_tail;

	//qDebug() << "encode:" << endl;

	//for (int i = 0; i < transf; ++i)
	//{
	//	qDebug() << QString("%1").arg(encode[i]);
	//}

	//qDebug() << endl;

	return ByteArray(encode.get(), transf);
}

ByteArray ProtocolPlc::Decoding(const unsigned char* data, const int& maxSize) const
{
	int len = 0, transf = 0;

	len = maxSize;

	std::unique_ptr<unsigned char[]> decode(new unsigned char[len]);

	std::memset(decode.get(), 0, len);

	// 存储报文头
	decode[transf++] = g_head;

	// 反转义数据体
	for (int i = sizeof(g_head); i < maxSize - sizeof(g_tail); ++i, ++transf)
	{
		if (data[i] != g_encode)
		{
			decode[transf] = data[i];

			continue;
		}

		switch (data[++i])
		{
		case 0x00:
			decode[transf] = g_encode;
			break;
		case 0x01:
			decode[transf] = g_head;
			break;
		case 0x02:
			decode[transf] = g_tail;
			break;
		}
	}

	// 存储报文尾
	decode[transf++] = g_tail;

	//qDebug() << "decode:" << endl;

	//for (int i = 0; i < transf; ++i)
	//{
	//	qDebug() << QString("%1").arg(decode[i]);
	//}

	//qDebug() << endl;

	return ByteArray(decode.get(), transf);
}

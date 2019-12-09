#include "ProtocolPlc.h"

const unsigned char ProtocolPlc::g_head = 0x8E;
const unsigned char ProtocolPlc::g_tail = 0x8F;
const unsigned char ProtocolPlc::g_encode = 0xA1;

ProtocolPlc::ProtocolPlc()
	: ProtocolBase(ProtocolType::Protocol_Plc)
	, m_byCode(0xB1)
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
	source[index++] = g_encode;

	// 存储数据体
	std::memcpy(source.get() + index, data, maxSize);

	crc16_t crc = SUM16(source.get(), 2 + maxSize);	/*!< 校验码 */

	index += maxSize;

	// 存储校验码
	source[index++] = (crc >> 8) & 0xFF;
	source[index++] = crc & 0xFF;

	// 存储报文尾
	source[index++] = g_tail;

	return ByteArray(source.get(), index);
}

ByteArrayList ProtocolPlc::Process(ByteArray& data) const
{
	ByteArrayList list;				/*!< 数据包列表 */

	const unsigned char* begin = data.get();	/*!< 数据首地址指针 */
	int size = data.size();						/*!< 数据长度 */

	const unsigned char* remain = begin;		/*!< 指向待处理数据首地址的指针 */
	int reminSize = size;						/*!< 待处理数据大小 */

	const int min_size = 4 + sizeof(crc16_t);	/*!< 最小数据长度 */

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

		int nLeft = size - ((head + 1) - begin);	/*!< 剩余字节数 */

		if (nLeft <= 0)
		{// 报文头未接收完全,仅保留报文头，舍弃其余数据
			remain = head;
			reminSize = size - (remain - begin);

			break;
		}

		if (*(head + 1) != m_byCode)
		{// 未找到正确的报文头,除去报文头以前(包含报文头)的数据
			remain = head + 1;
			reminSize = size - (remain - begin);

			continue;
		}

		// 查找报文尾
		const unsigned char* tail = reinterpret_cast<const unsigned char*>(memchr(head + 2, g_tail, reminSize - 2));

		if (tail == nullptr)
		{
			// 未找到报文尾

			break;
		}

		nLeft = size - ((tail + 1) - begin);

		if (nLeft <= 0)
		{// 报文尾未接收完全
			break;
		}

		if (*(tail + 1) != m_byCode)
		{// 报文尾不正确,除去报文尾以前(包含报文尾)的数据
			remain = tail + 1;
			reminSize = size - (remain - begin);

			continue;
		}

		remain = tail + 2;
		reminSize = size - (remain - begin);

		// 获取校验码

		// 指向校验位首地址
		const unsigned char* packetData = tail - sizeof(crc16_t);

		crc16_t packetCrc = 0;	/*!< 报文上传的校验码 */
		crc16_t sourceCrc = 0;	/*!< 源数据校验码 */

		// 获取校验码
		for (int i = 0; i < sizeof(crc16_t); ++i)
		{
			unsigned char tmp = *(packetData + i);
			packetCrc |= (tmp << 8 * (sizeof(length_t) - i - 1));
		}

		// 获取源数据校验码
		sourceCrc = SUM16(head, packetData - head);

		if (sourceCrc != packetCrc)
		{
			// 校验码不符
			continue;
		}

		// 存入列表
		list.push_back(ByteArray(head + 2, packetData - head - 2));
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

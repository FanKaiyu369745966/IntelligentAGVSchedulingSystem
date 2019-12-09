#include "ByteArray.h"
#include <QDebug>

ByteArray::ByteArray(const char* data)
	: m_buffer(new unsigned char[strlen(data)])
	, m_size(strlen(data))
{
	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), data, strlen(data));
}

ByteArray::ByteArray(const char* data, const size_t& len)
	: m_buffer(new unsigned char[len])
	, m_size(len)
{
	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), data, len);
}

ByteArray::ByteArray(const unsigned char* data, const size_t& len)
	: m_buffer(new unsigned char[len])
	, m_size(len)
{
	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), data, len);
}

ByteArray::ByteArray(const ByteArray& buf)
	: m_buffer(new unsigned char[buf.m_size])
	, m_size(buf.m_size)
{
	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), buf.get(), buf.m_size);
}

ByteArray::ByteArray(const QByteArray& buf)
	: m_buffer(new unsigned char[buf.count()])
	, m_size(buf.count())
{
	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), buf.data(), buf.count());
}

void ByteArray::clear()
{
	m_size = 0;
	m_buffer.reset();

	return;
}

QByteArray ByteArray::toQByteAarray() const
{
	QByteArray arr;
	for (int i = 0; i < m_size; ++i)
	{
		arr.insert(i, m_buffer[i]);
	}

	//for (int i = 0; i < m_size; ++i)
	//{
	//	qDebug() << QString::fromLocal8Bit("0x%1").arg(arr.at(i), 2, 16, QLatin1Char('0'));
	//}

	return arr;
}

void ByteArray::operator=(const char* data)
{
	m_buffer.reset(new unsigned char[strlen(data)]);
	m_size = strlen(data);

	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), data, strlen(data));

	return;
}

void ByteArray::operator=(const ByteArray& buf)
{
	m_buffer.reset(new unsigned char[buf.m_size]);
	m_size = buf.m_size;

	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), buf.get(), buf.m_size);

	return;
}

void ByteArray::operator=(const QByteArray& buf)
{
	m_buffer.reset(new unsigned char[buf.count()]);
	m_size = buf.count();

	memset(m_buffer.get(), 0, m_size);

	memcpy(m_buffer.get(), buf.data(), buf.count());

	return;
}

bool ByteArray::operator==(const char* data) const
{
	if (m_size != strlen(data) + 1)
	{
		return false;
	}

	for (int i = 0; i < m_size; ++i)
	{
		if (m_buffer[i] != data[i])
		{
			return false;
		}
	}

	return true;
}

bool ByteArray::operator==(const ByteArray& buf) const
{
	if (m_size != buf.m_size)
	{
		return false;
	}

	for (int i = 0; i < m_size; ++i)
	{
		if (m_buffer[i] != buf.m_buffer[i])
		{
			return false;
		}
	}

	return true;
}

bool ByteArray::operator==(const QByteArray& buf) const
{
	if (m_size != buf.count())
	{
		return false;
	}

	for (int i = 0; i < m_size; ++i)
	{
		if (m_buffer[i] != buf.data()[i])
		{
			return false;
		}
	}

	return true;
}

ByteArray& ByteArray::operator+=(const char* data)
{
	size_t newSize = m_size + strlen(data);
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, data, strlen(data));

	m_buffer.reset(new unsigned char[newSize]);

	memcpy(m_buffer.get(), newBuf.get(), newSize);

	return *this;
}

ByteArray& ByteArray::operator+=(const ByteArray& buf)
{
	size_t newSize = m_size + buf.m_size;
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, buf.get(), buf.m_size);

	m_buffer.reset(new unsigned char[newSize]);

	memcpy(m_buffer.get(), newBuf.get(), newSize);

	return *this;
}

ByteArray& ByteArray::operator+=(const QByteArray& buf)
{
	size_t newSize = m_size + buf.count();
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, buf.data(), buf.count());

	m_buffer.reset(new unsigned char[newSize]);

	memcpy(m_buffer.get(), newBuf.get(), newSize);

	return *this;
}

ByteArray ByteArray::operator+(const char* data)
{
	size_t newSize = m_size + strlen(data);
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, data, strlen(data));

	return ByteArray(newBuf.get(), newSize);
}

ByteArray ByteArray::operator+(const ByteArray& buf)
{
	size_t newSize = m_size + buf.m_size;
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, buf.get(), buf.m_size);

	return ByteArray(newBuf.get(), newSize);
}

ByteArray ByteArray::operator+(const QByteArray& buf)
{
	size_t newSize = m_size + buf.count();
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), m_buffer.get(), m_size);

	memcpy(newBuf.get() + m_size, buf.data(), buf.count());

	return ByteArray(newBuf.get(), newSize);
}

ByteArray operator+(const char* data, const ByteArray& buf)
{
	size_t newSize = strlen(data) + buf.m_size;
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), data, strlen(data));

	memcpy(newBuf.get() + strlen(data), buf.get(), buf.m_size);

	return ByteArray(newBuf.get(), newSize);
}

ByteArray operator+(const QByteArray& data, const ByteArray& buf)
{
	size_t newSize = data.count() + buf.m_size;
	std::unique_ptr<unsigned char[]> newBuf(new unsigned char[newSize]);

	memcpy(newBuf.get(), data.data(), data.count());

	memcpy(newBuf.get() + data.count(), buf.get(), buf.m_size);

	return ByteArray(newBuf.get(), newSize);
}

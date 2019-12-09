#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <QByteArray>
#include <memory>
#include <list>

class ByteArray;

typedef std::list<ByteArray> ByteArrayList;

class ByteArray
{
public:
	explicit ByteArray(const char*);
	explicit ByteArray(const char*, const size_t&);
	explicit ByteArray(const unsigned char*, const size_t&);
	ByteArray(const ByteArray&);
	ByteArray(const QByteArray&);
protected:
	std::unique_ptr<unsigned char[]> m_buffer;
	size_t m_size;
public:
	unsigned char* get() const { return m_buffer.get(); }
	size_t size() const { return m_size; }
	void clear();
	unsigned char at(const size_t& index) const { return m_buffer[index]; }
	char* data() const { return reinterpret_cast<char*>(m_buffer.get()); }
	QByteArray toQByteAarray() const;
public:
	void operator=(const char*);
	void operator=(const ByteArray&);
	void operator=(const QByteArray&);
	bool operator==(const char*) const;
	bool operator==(const ByteArray&) const;
	bool operator==(const QByteArray&) const;
	ByteArray& operator+=(const char*);
	ByteArray& operator+=(const ByteArray&);
	ByteArray& operator+=(const QByteArray&);
	ByteArray operator+(const char*);
	ByteArray operator+(const ByteArray&);
	ByteArray operator+(const QByteArray&);
	friend ByteArray operator+(const char*, const ByteArray&);
	friend ByteArray operator+(const QByteArray&, const ByteArray&);
};

#endif // BYTEARRAY_H

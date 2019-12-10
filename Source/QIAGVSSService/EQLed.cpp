#include "EQLed.h"
#include <QException>
#include <QDebug>


EQLed::EQLed()
	: m_lib("EQ2008_Dll")
	, m_bConnect(false)
	, m_nMaxLine(0)
	, m_bSent(true)
	, m_strFont(QString::fromLocal8Bit("宋体"))
	, m_nSize(0)
	, m_x(0)
	, m_y(0)
{
	Initialize();
}

EQLed::EQLed(const unsigned short& id, const int& x, const int& y, const unsigned short& width, const unsigned short& height, const int& maxLine, const QString& font, const int& size)
	: LedBase(id, width, height)
	, m_lib("EQ2008_Dll")
	, m_bConnect(false)
	, m_nMaxLine(maxLine)
	, m_bSent(true)
	, m_strFont(font)
	, m_nSize(size)
	, m_x(x)
	, m_y(y)
{
	Initialize();
}

EQLed::~EQLed()
{
	if (m_font)
	{
		delete[] m_font;
	}

	m_lib.unload();
}

bool EQLed::Connect()
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_RealtimeConnect(m_usId);

	return m_bConnect;
}

bool EQLed::Disconnect()
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_RealtimeDisConnect(m_usId);

	return m_bConnect;
}

bool EQLed::OpenScreen()
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_OpenScreen(m_usId);

	return m_bConnect;
}

bool EQLed::CloseScreen()
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_CloseScreen(m_usId);

	return m_bConnect;
}

bool EQLed::AdjustTime()
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_AdjustTime(m_usId);

	return m_bConnect;
}

bool EQLed::SetScreenLight(int nLightDegreen)
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	m_bConnect = User_SetScreenLight(m_usId, nLightDegreen);

	return m_bConnect;
}

void EQLed::Initialize()
{
	QByteArray type = m_strFont.toLocal8Bit();
	m_font = new char[type.size() + 1];

	memset(m_font, 0, type.size() + 1);

	memcpy(m_font, type.constData(), type.size());

	m_fontInfo.bFontBold = false;
	m_fontInfo.bFontItaic = false;
	m_fontInfo.bFontUnderline = false;
	m_fontInfo.colorFont = 0xFFFF;
	m_fontInfo.iFontSize = m_nSize;
	m_fontInfo.strFontName = m_font;
	m_fontInfo.iAlignStyle = 0;
	m_fontInfo.iVAlignerStyle = 0;
	m_fontInfo.iRowSpace = 0;

	return;
}

bool EQLed::SetText(const int& key, const QString& value)
{
	if (m_bConnect == false)
	{
		return false;
	}

	if (m_mapText.find(key) != m_mapText.end())
	{
		if (m_mapText[key] == value)
		{
			return true;
		}

		m_mapBak[key] = m_mapText[key];
	}

	m_mapText[key] = value;

	return true;
}

bool EQLed::ClearText(const int& key)
{
	if (m_bConnect == false)
	{
		return false;
	}

	if (m_mapText.find(key) == m_mapText.end())
	{
		return true;
	}

	m_mapText.erase(m_mapText.find(key));

	return true;
}

bool EQLed::SendText()
{
	QString str;

	for (std::map<int, QString>::iterator it = m_mapText.begin(); it != m_mapText.end(); ++it)
	{
		str += it->second;
		str += '\n';

		int row = 1;	/*!< 字符串占用的行数 */

		if (m_nMaxLine > 0)
		{
			int nLen = it->second.toLocal8Bit().length();
			row = nLen / m_nMaxLine;
			if ((nLen % m_nMaxLine) > 0)
			{
				++row;
			}
		}

		int key = it->first; /*!< 键值 */

		for (int i = 1; i < row; ++i)
		{
			if (m_mapText.find(key + i) == m_mapText.end())
			{
				continue;
			}

			QString value = m_mapText.at(key + i);

			if (value.isEmpty() || value.isNull())
			{
				++it;
			}
		}
	}

	if (SendText(str))
	{
		return true;
	}

	for (std::map<int, QString>::iterator it = m_mapBak.begin(); it != m_mapBak.end(); it = m_mapBak.erase(it))
	{
		m_mapText[it->first] = it->second;
	}

	return false;
}

bool EQLed::SendText(const QString& str)
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	if (m_bSent == false)
	{
		return false;
	}

	m_bSent = false;

	if (Connect() == false)
	{
		m_bSent = true;

		return false;
	}

	if (str.isNull() || str.isEmpty())
	{
		m_bSent = true;

		Disconnect();

		return true;
	}

	if (m_usWidth == 0 || m_usHeight == 0)
	{
		m_bSent = true;

		Disconnect();

		return true;
	}

	char* ptr = new char[str.toLocal8Bit().size() + 1];

	memset(ptr, 0, str.toLocal8Bit().size() + 1);

	memcpy(ptr, str.toLocal8Bit().data(), str.toLocal8Bit().size());

	//发送文本
	m_bConnect = User_RealtimeSendText(m_usId, m_x, m_y, m_usWidth, m_usHeight, ptr, &m_fontInfo);

	delete[] ptr;

	Disconnect();

	m_bSent = true;

	return true;
}

QString EQLed::GetValue(const int& index) const
{
	if (m_mapText.find(index) == m_mapText.end())
	{
		return QString();
	}

	return m_mapText.at(index);
}

#include "EQLed.h"

#include <QDebug>


EQLed::EQLed()
	: m_lib("EQ2008_Dll")
	, m_bConnect(false)
	, m_nMaxLine(0)
	, m_bSent(true)
	, m_strFont(QString::fromLocal8Bit("宋体"))
	, m_nSize(0)
{
}

EQLed::EQLed(const unsigned short& id, const unsigned short& width, const unsigned short& height, const int& maxLine, const QString& font, const int& size)
	: LedBase(id, width, height)
	, m_lib("EQ2008_Dll")
	, m_bConnect(false)
	, m_nMaxLine(maxLine)
	, m_bSent(true)
	, m_strFont(font)
	, m_nSize(size)
{
}

EQLed::~EQLed()
{
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

bool EQLed::SendText(char* strText)
{
	if (m_usId == 0)
	{
		m_bConnect = false;
		return false;
	}

	if (strText == nullptr)
	{
		return false;
	}

	//设置字体属性
	User_FontSet FontInfo;
	FontInfo.bFontBold = false;
	FontInfo.bFontItaic = false;
	FontInfo.bFontUnderline = false;
	FontInfo.colorFont = 0xFFFF;
	FontInfo.iFontSize = m_nSize;
	FontInfo.strFontName = m_strFont.toLocal8Bit().data();
	FontInfo.iAlignStyle = 0;
	FontInfo.iVAlignerStyle = 1;
	FontInfo.iRowSpace = 0;

	//发送文本
	m_bConnect = User_RealtimeSendText(m_usId, 0, 0, m_usWidth, m_usHeight, strText, &FontInfo);

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

bool EQLed::SetText(const int& key, const QString& value)
{
	if (m_bConnect == false)
	{
		return false;
	}

	QString bak = "";
	if (m_mapText.find(key) != m_mapText.end())
	{
		if (m_mapText[key] == value)
		{
			return true;
		}

		bak = m_mapText[key];
	}

	m_mapText[key] = value;

	int row = 1;	/*!< 字符串占用的行数 */

	if (m_nMaxLine > 0)
	{
		int nLen = value.toLocal8Bit().length();
		row = nLen / m_nMaxLine;
		if ((nLen % m_nMaxLine) > 0)
		{
			++row;
		}
	}

	QString str;

	for (std::map<int, QString>::iterator it = m_mapText.begin(); it != m_mapText.end(); ++it)
	{
		if ((it->first > key&& it->first < key + row) && (it->second.isEmpty() || it->second.isNull()))
		{
			continue;
		}

		str += it->second;
		str += '\n';
	}

	if (SendText(str) == false)
	{
		m_mapText[key] = bak;

		return false;
	}

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

	QString str;

	for (std::map<int, QString>::iterator it = m_mapText.begin(); it != m_mapText.end(); ++it)
	{
		str += it->second;
		str += '\n';
	}

	return SendText(str);
}

bool EQLed::SendText(const QString& str)
{
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

	if (SendText(str.toLocal8Bit().data()) == false)
	{
		m_bSent = true;
		return false;
	}

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

#ifndef EQLED_H
#define EQLED_H

#include "LedBase.h"
#include <QLibrary>
#include "res/led/EQ2008_Dll.h"
#pragma comment(lib,"res/led/EQ2008_Dll.lib")

#include <map>

enum class EqType : int
{
	EQ3002_I,
	EQ3002_II,
	EQ3002_III,
	EQ2008_I,
	EQ2010_I,
	EQ2008_IE,
	EQ2011 = 7,
	EQ2012,
	EQ2008_M,
	EQ2013 = 21,
	EQ2023,
	EQ2033
};

class EQLed :
	public LedBase
{
public:
	EQLed();
	explicit EQLed(const unsigned short&, const int& x, const int& y, const unsigned short&, const unsigned short&, const int&, const QString&, const int&);
	~EQLed();
private:
	QLibrary m_lib;
public:
	//实时更新操作////////////////////////////////////////////////////////////////////////
	/*!
	* @brief 建立连接
	* @return bool 成功 true, 失败 false
	*/
	bool Connect();

	/*!
	* @brief 断开连接
	* @return bool 成功 true, 失败 false
	*/
	bool Disconnect();

	/*!
	* @brief 发送文本
	* @param strText 需要发送的字符串
	* @return bool 成功 true, 失败 false
	*/
	bool SendText(const QString& strText);

	//显示屏控制操作////////////////////////////////////////////////////////////////////////
	/*!
	* @brief 开屏
	* @return bool 成功 true, 失败 false
	*/
	bool OpenScreen();

	/*!
	* @brief 关屏
	* @return bool 成功 true, 失败 false
	*/
	bool CloseScreen();

	/*!
	* @brief 校正时间
	* @return bool 成功 true, 失败 false
	*/
	bool AdjustTime();

	/*!
	* @brief 亮度调节
	* @param nLightDegreen 亮度值，范围是 0～15，取值越大，显示屏亮度越高
	* @return bool 成功 true, 失败 false
	*/
	bool SetScreenLight(int nLightDegreen);

private:
	std::map<int, QString> m_mapText;
	std::map<int, QString> m_mapBak;
	bool m_bConnect;
	int m_nMaxLine;			/*!< 单行最大字节数 */
	bool m_bSent;
	QString	m_strFont;
	int m_nSize;
	User_FontSet m_fontInfo;
	char* m_font;
	int m_x;
	int m_y;
private:
	void Initialize();
public:
	bool SetText(const int& key, const QString& value);
	bool ClearText(const int& key);
	bool SendText();
	bool IsConnected() const { return m_bConnect; }
	QString GetValue(const int&) const;
};

#endif // EQLED_H

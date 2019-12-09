/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file CallerAttribute.h
 *
 * @brief 描述呼叫器属性的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef CALLERATTRIBUTE_H
#define CALLERATTRIBUTE_H

#include <QString>

typedef class CallerAttribute CallerAttr;

/*!
 * @brief 描述发出的信号的枚举
 * @date 2019/11/16
 * @author FanKaiyu
 */
enum class Signal : unsigned char
{
	Signal_Off,			/*!< 无信号 */
	Signal_On,			/*!< 有信号/取回空托盘 */
	Signal_ToStation,	/*!< 取成品入站 */
	Signal_ToLib,		/*!< 取成品入库 */
	Signal_PutBlank1,	/*!< 运送毛坯至线1 */
	Signal_PutBlank2,	/*!< 运送毛坯至线2 */
	Signal_PutBlank3,	/*!< 运送毛坯至线3 */
	Signal_PutTray1,	/*!< 运送空托盘至线1 */
	Signal_PutTray2,	/*!< 运送空托盘至线2 */
	Signal_PutTray3,	/*!< 运送空托盘至线3 */
};

/*!
 * @brief 描述信号类型的枚举
 * @date 2019/11/16
 * @author FanKaiyu
 */
enum class SignalType : unsigned char
{
	Signal_Scream,		/*!< 急停信号 */
	Signal_Call,		/*!< 呼叫信号 */
	Signal_Cancel,		/*!< 取消信号 */
};

/*!
 * @class CallerAttribute
 * @brief 描述呼叫器属性的类
 * @date 2019/11/16
 * @author FanKaiyu
 */
class CallerAttribute
{
public:
	/*!
	 * @brief CallerAttribute
	 *
	 * CallerAttribute::CallerAttribute
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return
	 */
	CallerAttribute();

	/*!
	 * @brief CallerAttribute
	 *
	 * overload CallerAttribute::CallerAttribute
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned char & 急停信号
	 * @param const unsigned char & 呼叫信号
	 * @param const unsigned char & 取消信号
	 */
	explicit CallerAttribute(const unsigned char& scream, const unsigned char& call, const unsigned char& cancel);
	CallerAttribute(const CallerAttribute& attr);
public:
	unsigned char m_sigScream;		/*!< 急停信号 */
	unsigned char m_sigCall;		/*!< 呼叫信号 */
	unsigned char m_sigCancel;		/*!< 取消信号 */
public:
	void operator = (const CallerAttribute& attr);
	bool operator == (const CallerAttribute& attr);
	bool operator != (const CallerAttribute& attr);
public:
	/*!
	 * @brief 更新信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateSignal(const CallerAttribute& attr);

	/*!
	 * @brief 更新信号
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const SignalType & 信号类型
	 * @param const Signal & 信号
	 * @return bool 更新成功返回true,否则返回false
	 */
	bool UpdateSignal(const SignalType& type, const Signal& signal);

	/*!
	 * @brief 更新呼叫信号
	 * @param const unsigned char& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateCallSignal(const unsigned char& signal);

	/*!
	 * @brief 更新呼叫信号
	 * @param const SignalType& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateCallSignal(const Signal& signal) { return UpdateCancelSignal(static_cast<unsigned char>(signal)); }

	/*!
	 * @brief 获取呼叫信号
	 * @return const SignalType& 信号
	*/
	Signal GetCallSignal() const { return static_cast<Signal>(m_sigCall); }

	/*!
	 * @brief 更新取消信号
	 * @return const unsigned char& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateCancelSignal(const unsigned char& signal);

	/*!
	 * @brief 更新取消信号
	 * @return const SignalType& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateCancelSignal(const Signal& signal) { return UpdateCancelSignal(static_cast<unsigned char>(signal)); }

	/*!
	 * @brief 获取取消信号
	 * @return const SignalType& 信号
	*/
	Signal GetCancelSignal() const { return static_cast<Signal>(m_sigCancel); }

	/*!
	 * @brief 更新急停信号
	 * @return const unsigned char& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateScreamSignal(const unsigned char& signal);

	/*!
	 * @brief 更新急停信号
	 * @return const bool& 信号
	 * @return bool 信号发生改变返回true,否则返回false
	*/
	bool UpdateScreamSignal(const Signal& signal) { return UpdateScreamSignal(static_cast<unsigned char>(signal)); }

	/*!
	 * @brief 获取急停信号
	 * @return bool 存在急停信号返回true,否则返回false
	*/
	Signal GetScreamSignal() const { return static_cast<Signal>(m_sigScream); }

	/*!
	 * @brief 获取信号的详细信息
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const SignalType & 信号类型
	 * @return std::string 信号信息
	 */
	QString GetSignalInfo(const Signal&) const;
};

#endif // CALLERATTRIBUTE_H

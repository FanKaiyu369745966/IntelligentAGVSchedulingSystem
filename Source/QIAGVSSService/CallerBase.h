/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file CallerBase.h
 *
 * @brief 描述呼叫器基础属性和功能的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */

#ifndef CALLERBASE_H
#define CALLERBASE_H

 /*!
  * @brief 呼叫器类型
  * @date 2019/11/16
  * @author FanKaiyu
  */
enum class CallerType : unsigned char
{
	Type_None,
	Type_Zigbee,	/*!< zigbee呼叫器 */
};

/*!
 * @class CallerBase
 * @brief 描述呼叫器基础属性和功能的类
 * @date 2019/11/16
 * @author FanKaiyu
 */
class CallerBase
{
public:
	/*!
	 * @brief CallerBase
	 *
	 * CallerBase::CallerBase
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	CallerBase();

	/*!
	 * @brief CallerBase
	 *
	 * CallerBase::CallerBase
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const CallerType & 类型
	 * @param const unsigned short & 编号
	 * @param const unsigned short & 绑定工作站
	 * @param const bool & 设备状态
	 */
	explicit CallerBase(const CallerType&, const unsigned short&, const unsigned short& = 0, const bool& = true);
	CallerBase(const CallerBase&);
protected:
	unsigned short m_usId;		/*!< 编号 */
	CallerType m_byType;		/*!< 类型 */
	unsigned short m_usStation;	/*!< 绑定的工作站编号 */
	bool m_bUse;				/*!< 启用标识 */
public:
	operator bool() const;
	bool operator==(const bool&) const;
	bool operator!=(const bool&) const;
	void operator=(const CallerBase&);
public:
	/*!
	 * @brief 判断呼叫器是否为空
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return bool 为空返回true,否则返回false
	 */
	bool IsNull() const;

	/*!
	 * @brief 获取编号
	 * @date 2019/11/16
	 * @return unsigned short
	 */
	unsigned short GetId() const { return m_usId; }

	/*!
	 * @brief 获取呼叫器类型
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return CallerType 呼叫器类型
	 */
	CallerType GetType() const { return m_byType; }

	/*!
	 * @brief 启用呼叫器
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	void Enable() { m_bUse = true; return; }
	/*!
	 * @brief 禁用呼叫器
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	void Disable() { m_bUse = false; return; }

	/*!
	 * @brief 判断呼叫器的使用状态
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return bool 启用返回true,否则返回false
	 */
	bool IsEnabled() const { return m_bUse; }

	/*!
	 * @brief 设置呼叫器使用状态
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const bool & true为启动,false为禁用
	 */
	void SetEnableUse(const bool& flag) { m_bUse = flag; return; }

	/*!
	 * @brief 绑定工作站
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @return void
	 */
	void BindStation(const unsigned short& station) { m_usStation = station; return; }

	/*!
	 * @brief 获取呼叫器绑定的工作站编号
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return unsigned short 工作站编号
	 */
	unsigned short GetStation() const { return m_usStation; }
};

#endif // CALLERBASH_H

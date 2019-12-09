/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file AgvAttribute.h
 *
 * @brief 描述AGV属性的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef AGVATTRIBUTE_H
#define AGVATTRIBUTE_H

#include <QString>
#include "RfidBase.h"

typedef class AgvAttribute AgvAttr;

#pragma region 枚举
/*! @brief 描述AGV模式的枚举　*/
enum class AgvMode : unsigned char
{
	Mode_Hand,				/*!< 自动模式 */
	Mode_Auto,				/*!< 手动模式 */
};

/*! @brief 描述AGV状态的枚举 */
enum class AgvStatus : unsigned char
{
	Sta_Wait,				/*!< 待机 */
	Sta_Run,				/*!< 运行 */
	Sta_Stop,				/*!< 停止 */
	Sta_Scream,				/*!< 急停:使用AGV上的急停按钮,紧急停止AGV时,AGV上报此状态 */
	Sta_Find,				/*!< 寻磁 */
	Sta_ObsDown,			/*!< 避障减速:通过光电避障或激光避障等非接触式避障器,检测到移动前方有障碍物,减速慢行时,AGV上报此状态 */
	Sta_Traffic,			/*!< 交通管制停止 */
	Sta_Sleep,				/*!< 休眠 */
	Sta_Charging,			/*!< 充电 */
	Sta_RmtScream,			/*!< 远程急停:使用系统上的急停指令,紧急停止AGV时,AGV上报此状态 */
	Sta_AllScream,			/*!< 权限急停:使用系统外接设备上的急停按钮,紧急停止AGV时,AGV上报此状态 */
	Sta_SpeedUp,			/*!< 加速 */
	Sta_SpeedDown,			/*!< 减速 */
	Sta_Pause,				/*!< 暂停 */
};

/*! @brief 描述AGV速度的枚举 */
enum class AgvSpeed : char
{
	Speed_Stop,					/*!< 停止移动 */
	Speed_MaxForward = 100,		/*!< 正向速度最大值 */
	Speed_MaxReverse = -100,	/*!< 反向速度最大值 */
};

/*! @brief 描述AGV电量级别的枚举 */
enum class AgvBattery : unsigned char
{
	Power_Off,				/*!< 电源处于关闭状态 */
	Power_Low = 20,			/*!< 电量过低 */
	Power_Safe = 90,		/*!< 安全电量 */
	Power_Full = 100,		/*!< 满电 */
};

/*! @brief 描述AGV载货数量的枚举 */
enum class AgvCargo : unsigned char
{
	Cargo_Empty,			/*!< 空载 */
	Cargo_Full = UCHAR_MAX,	/*!< 满载 */
};

/*! @brief 描述AGV异常信息的枚举 */
enum class AgvError : unsigned char
{
	Error_None,				/*!< 无异常 */
	Error_Miss,				/*!< 脱磁异常 */
	Error_Obs,				/*!< 避障停车:通过光电避障或激光避障等非接触式避障器,检测到移动前方有障碍物,停止移动时,AGV上报此状态 */
	Error_MObs,				/*!< 避障停车:通过挡条、挡板等避障器,检测AGV碰撞到了未知物体时,AGV上报此状态 */
};

/*! @brief 描述AGV动作的枚举 */
enum class AgvAction : unsigned char
{
	Act_None,				/*!< 无动作 */
	Act_Move = UCHAR_MAX - 1,	/*!< 移动动作 */
};

/*! @brief 描述AGV动作执行状态的枚举 */
enum class AgvExecution : unsigned char
{
	Act_None,				/*!< 未执行动作 */
	Act_Exe,				/*!< 正在执行动作 */
	Act_Fin,				/*!< 已执行完动作 */
};
#pragma endregion

class AgvAttribute
{
public:
	typedef unsigned char mode_t;
	typedef unsigned char status_t;
	typedef char speed_t;
	typedef unsigned char battery_t;
	typedef unsigned char cargo_t;
	typedef unsigned char error_t;
	typedef unsigned char action_t;
	typedef unsigned char execute_t;

public:
	mode_t m_mode;				/*!< 模式 */
	status_t m_status;			/*!< 状态 */
	speed_t m_speed;			/*!< 速度(%):正向移动时速度为正数,反向移动时速度为负数 */
	battery_t m_battery;		/*!< 电量(%) */
	cargo_t m_cargo;			/*!< 载货数量 */
	error_t m_error;			/*!< 异常信息 */
	action_t m_action;			/*!< 动作信息 */
	execute_t m_execute;		/*!< 动作的执行状态信息 */

public:
	rfid_t m_curLocat;	/*!< 当前坐标 */
	rfid_t m_endLocat;	/*!< 终点坐标 */
	rfid_t m_oldLocat;	/*!< 历史坐标 */
	rfid_t m_oldEndLocat;	/*!< 历史终点坐标 */

protected:
	std::chrono::steady_clock::time_point m_timeActCount;	/*!< 动作执行时间计数器 */

public:
	AgvAttribute();
	explicit AgvAttribute(const status_t& status, const action_t& action, const execute_t& execuet, const rfid_t& curLocat, const rfid_t& endLocat);
	AgvAttribute(const AgvAttribute& arrribute);

public:
	void operator=(const AgvAttribute& arrribute);

public:
	/*!
	 * @brief 判断属性是否为空
	 * @return bool 属性为空,即属性成员全部初始化时返回true,否则返回false
	*/
	bool IsNull() const;

public:
	/*!
	 * @brief 更新属性
	 * @return bool 属性发生改变时返回true,否则返回false
	*/
	bool Update(const AgvAttribute& attr);
	bool UpdateMode(const mode_t& mode);
	bool UpdateStatus(const status_t& status);
	bool UpdateSpeed(const speed_t& speed);
	bool UpdateBattery(const battery_t& battery);
	bool UpdateCargo(const cargo_t& cargo);
	bool UpdateError(const error_t& error);
	bool UpdateAction(const action_t& action, const execute_t& execute);
	bool UpdateCueLocation(const rfid_t& rfid);
	bool UpdateEndLocation(const rfid_t& rfid);

	/*!
	 * @brief 获取属性
	*/
	AgvMode GetMode() const { return static_cast<AgvMode>(m_mode); }
	AgvStatus GetStatus() const { return static_cast<AgvStatus>(m_status); }
	AgvSpeed GetSpeed() const { return static_cast<AgvSpeed>(m_speed); }
	AgvBattery GetBattery() const { return static_cast<AgvBattery>(m_battery); }
	AgvCargo GetCargo() const { return static_cast<AgvCargo>(m_cargo); }
	AgvError GetError() const { return static_cast<AgvError>(m_error); }
	AgvAction GetAction() const { return static_cast<AgvAction>(m_action); }
	AgvExecution GetExecution() const { return static_cast<AgvExecution>(m_execute); }
	rfid_t GetCurLocation() const { return m_curLocat; }
	rfid_t GetOldLocation() const { return m_oldLocat; }
	rfid_t GetEndLocation() const { return m_endLocat; }
	rfid_t GetOldEndLocation() const { return m_oldEndLocat; }

public:
	/*!
	 * @brief 获取动作状态信息
	 * @param action_t& 动作信息
	 * @param execute_t& 动作执行状态信息
	 * @return size_t 返回动作执行时间(ms)
	*/
	size_t GetAction(AgvAction& action, AgvExecution& execute) const;

	/*!
	 * @brief 获取动作执行时间
	 * @return size_t 动作执行时间(ms)
	*/
	size_t GetActionExecuteTime() const;

	/*!
	 * @brief 设置动作执行时间
	 * @param const size_t& 动作执行时间(ms)
	*/
	void SetActionExecuteTime(const size_t& time);

	/*!
	 * @brief 获取状态详细信息
	 * @return string 状态详细信息
	*/
	QString GetStatusInfo() const;

	/*!
	 * @brief 获取异常详细信息
	 * @return string 异常详细信息
	*/
	QString GetErrorInfo() const;

	QString GetCargoInfo() const;

	QString GetModeInfo() const;
};

#endif // AGVATTRIBUTE_H

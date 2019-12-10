#include "AgvAttribute.h"

AgvAttribute::AgvAttribute()
{
	m_mode = static_cast<mode_t>(AgvMode::Mode_Hand);
	m_status = static_cast<status_t>(AgvStatus::Sta_Wait);
	m_speed = static_cast<speed_t>(AgvSpeed::Speed_Stop);
	m_battery = static_cast<battery_t>(AgvBattery::Power_Off);
	m_cargo = static_cast<cargo_t>(AgvCargo::Cargo_Empty);
	m_error = static_cast<error_t>(AgvError::Error_None);
	m_action = static_cast<action_t>(AgvAction::Act_None);
	m_execute = static_cast<execute_t>(AgvExecution::Act_None);

	m_curLocat = 0;
	m_endLocat = 0;
	m_oldLocat = 0;
	m_oldEndLocat = 0;
}

AgvAttribute::AgvAttribute(const status_t& status, const action_t& action, const execute_t& execuet, const rfid_t& curLocat, const rfid_t& endLocat)
{
	m_mode = static_cast<mode_t>(AgvMode::Mode_Hand);
	m_status = status;
	m_speed = static_cast<speed_t>(AgvSpeed::Speed_Stop);
	m_battery = static_cast<battery_t>(AgvBattery::Power_Off);
	m_cargo = static_cast<cargo_t>(AgvCargo::Cargo_Empty);
	m_error = static_cast<error_t>(AgvError::Error_None);
	m_action = action;
	m_execute = execuet;

	m_curLocat = curLocat;
	m_endLocat = endLocat;
	m_oldLocat = 0;
	m_oldEndLocat = 0;
}

AgvAttribute::AgvAttribute(const AgvAttribute& arrribute)
{
	m_mode = arrribute.m_mode;
	m_status = arrribute.m_status;
	m_speed = arrribute.m_speed;
	m_battery = arrribute.m_battery;
	m_cargo = arrribute.m_cargo;
	m_error = arrribute.m_error;
	m_action = arrribute.m_action;
	m_execute = arrribute.m_execute;

	m_curLocat = arrribute.m_curLocat;
	m_endLocat = arrribute.m_endLocat;
	m_oldLocat = arrribute.m_oldLocat;
	m_oldEndLocat = arrribute.m_oldEndLocat;

	m_timeActCount = arrribute.m_timeActCount;
}

void AgvAttribute::operator=(const AgvAttribute& arrribute)
{
	m_mode = arrribute.m_mode;
	m_status = arrribute.m_status;
	m_speed = arrribute.m_speed;
	m_battery = arrribute.m_battery;
	m_cargo = arrribute.m_cargo;
	m_error = arrribute.m_error;
	m_action = arrribute.m_action;
	m_execute = arrribute.m_execute;

	m_curLocat = arrribute.m_curLocat;
	m_endLocat = arrribute.m_endLocat;
	m_oldLocat = arrribute.m_oldLocat;
	m_oldEndLocat = arrribute.m_oldEndLocat;

	m_timeActCount = arrribute.m_timeActCount;

	return;
}

bool AgvAttribute::IsNull() const
{
	if (m_status == static_cast<status_t>(AgvStatus::Sta_Wait)
		&& m_speed == static_cast<speed_t>(AgvSpeed::Speed_Stop)
		&& m_error == static_cast<error_t>(AgvError::Error_None)
		&& m_action == static_cast<action_t>(AgvAction::Act_None)
		&& m_execute == static_cast<execute_t>(AgvExecution::Act_None)
		&& m_curLocat == 0
		&& m_endLocat == 0)
	{
		return true;
	}

	return false;
}

bool AgvAttribute::Update(const AgvAttribute& attr)
{
	bool update = false;

	if (UpdateMode(attr.m_mode))
	{
		update = true;
	}

	if (UpdateAction(attr.m_action, attr.m_execute))
	{
		update = true;
	}

	if (UpdateBattery(attr.m_battery))
	{
		update = true;
	}

	if (UpdateCargo(attr.m_cargo))
	{
		update = true;
	}

	if (UpdateCueLocation(attr.m_curLocat))
	{
		update = true;
	}

	if (UpdateEndLocation(attr.m_endLocat))
	{
		update = true;
	}

	if (UpdateError(attr.m_error))
	{
		update = true;
	}

	if (UpdateSpeed(attr.m_speed))
	{
		update = true;
	}

	if (UpdateStatus(attr.m_status))
	{
		update = true;
	}

	return update;
}

bool AgvAttribute::UpdateMode(const mode_t& mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;

		return true;
	}

	return false;
}

bool AgvAttribute::UpdateStatus(const status_t& status)
{
	if (m_status != status)
	{
		m_status = status;

		return true;
	}
	return false;
}

bool AgvAttribute::UpdateSpeed(const speed_t& speed)
{
	if (m_speed != speed)
	{
		m_speed = speed;

		return true;
	}
	return false;
}

bool AgvAttribute::UpdateBattery(const battery_t& battery)
{
	if (m_battery != battery)
	{
		m_battery = battery;

		return true;
	}

	return false;
}

bool AgvAttribute::UpdateCargo(const cargo_t& cargo)
{
	if (m_cargo != cargo)
	{
		m_cargo = cargo;

		return true;
	}

	return false;
}

bool AgvAttribute::UpdateError(const error_t& error)
{
	if (m_error != error)
	{
		m_error = error;

		return true;
	}

	return false;
}

bool AgvAttribute::UpdateAction(const action_t& action, const execute_t& execute)
{
	bool update = false;

	if (m_action != action)
	{
		update = true;

		m_action = action;

		m_timeActCount = std::chrono::steady_clock::time_point();
	}

	if (m_execute != execute)
	{
		update = true;

		m_execute = execute;
	}

	if (m_execute == static_cast<execute_t>(AgvExecution::Act_Exe))
	{
		m_timeActCount = std::chrono::steady_clock::now();
	}
	else
	{
		m_timeActCount = std::chrono::steady_clock::time_point();
	}

	return update;
}

bool AgvAttribute::UpdateCueLocation(const rfid_t& rfid)
{
	if (m_curLocat != rfid)
	{
		m_oldLocat = m_curLocat;
		m_curLocat = rfid;

		return true;
	}

	return false; false;
}

bool AgvAttribute::UpdateEndLocation(const rfid_t& rfid)
{
	if (m_endLocat != rfid)
	{
		m_oldEndLocat = m_endLocat;
		m_endLocat = rfid;

		return true;
	}

	return false;
}

size_t AgvAttribute::GetAction(AgvAction& action, AgvExecution& execute) const
{
	action = static_cast<AgvAction>(m_action);
	execute = static_cast<AgvExecution>(m_execute);

	return GetActionExecuteTime();
}

size_t AgvAttribute::GetActionExecuteTime() const
{
	if (m_execute != static_cast<execute_t>(AgvExecution::Act_Exe))
	{
		return 0;
	}

	auto _dis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_timeActCount);

	long long _dis_t = _dis.count();

	if (_dis_t <= 0)
	{
		return 0;
	}

	return static_cast<size_t>(_dis_t);
}

void AgvAttribute::SetActionExecuteTime(const size_t& time)
{
	if (time == 0)
	{
		return;
	}

	m_timeActCount = std::chrono::steady_clock::now() - std::chrono::milliseconds(time);

	return;
}

QString AgvAttribute::GetStatusInfo() const
{
	QString _str = "unknown";

	switch (GetStatus())
	{
	case AgvStatus::Sta_Wait:
		_str = QString::fromLocal8Bit("待机");
		break;
	case AgvStatus::Sta_Run:
		_str = QString::fromLocal8Bit("运行");
		break;
	case AgvStatus::Sta_Stop:
		_str = QString::fromLocal8Bit("停止");
		break;
	case AgvStatus::Sta_Scream:
		_str = QString::fromLocal8Bit("急停");
		break;
	case AgvStatus::Sta_Find:
		_str = QString::fromLocal8Bit("寻磁中");
		break;
	case AgvStatus::Sta_ObsDown:
		_str = QString::fromLocal8Bit("避障减速行驶");
		break;
	case AgvStatus::Sta_Traffic:
		_str = QString::fromLocal8Bit("交通管制中");
		break;
	case AgvStatus::Sta_Sleep:
		_str = QString::fromLocal8Bit("休眠");
		break;
	case AgvStatus::Sta_Charging:
		_str = QString::fromLocal8Bit("充电中");
		break;
	case AgvStatus::Sta_RmtScream:
		_str = QString::fromLocal8Bit("远程急停");
		break;
	case AgvStatus::Sta_AllScream:
		_str = QString::fromLocal8Bit("全线急停");
		break;
	case AgvStatus::Sta_SpeedUp:
		_str = QString::fromLocal8Bit("加速中");
		break;
	case AgvStatus::Sta_SpeedDown:
		_str = QString::fromLocal8Bit("减速中");
		break;
	case AgvStatus::Sta_Pause:
		_str = QString::fromLocal8Bit("暂停");
		break;
	default:
		_str = "unknown";
		break;
	}
	return _str;
}

QString AgvAttribute::GetErrorInfo() const
{
	QString _str = "unknown";

	switch (GetError())
	{
	case AgvError::Error_None:
		_str = QString::fromLocal8Bit("无异常");
		break;
	case AgvError::Error_Miss:
		_str = QString::fromLocal8Bit("脱磁异常:未能在规定时间内找到磁轨");
		break;
	case AgvError::Error_Obs:
		_str = QString::fromLocal8Bit("避障停止:避障器检测到在行进路线上存在未知障碍物");
		break;
	case AgvError::Error_MObs:
		_str = QString::fromLocal8Bit("碰撞停止:避障器检测到在行进路线上碰撞未知障碍物");
		break;
	default:
		_str = "unknown";
		break;
	}

	return _str;
}

QString AgvAttribute::GetCargoInfo() const
{
	QString _str = QString::fromLocal8Bit("空载");

	switch (GetCargo())
	{
	case AgvCargo::Cargo_Empty:
		_str = QString::fromLocal8Bit("空载");
		break;
	case AgvCargo::Cargo_Full:
		_str = QString::fromLocal8Bit("满载");
		break;
	default:
		_str = QString::fromLocal8Bit("%1").arg(m_cargo);
		break;
	}

	return _str;
}

QString AgvAttribute::GetModeInfo() const
{
	QString _str = QString::fromLocal8Bit("unknow");

	switch (GetMode())
	{
	case AgvMode::Mode_Auto:
		_str = QString::fromLocal8Bit("自动");
		break;
	case AgvMode::Mode_Hand:
		_str = QString::fromLocal8Bit("手动");
		break;
	default:
		_str = QString::fromLocal8Bit("unknow");
		break;
	}
	return _str;
}

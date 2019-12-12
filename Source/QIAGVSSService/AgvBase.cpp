#include "AgvBase.h"

AgvBase::AgvBase(const shared_ptr<AgvType>& type, const id_t& id, const bool& bClient, const QString& qstrAddr, const unsigned short& usPort,
	const QString& localAddr, const unsigned short& localPort, QObject* parent)
	: QObject(parent)
	, m_pType(type)
	, m_id(id)
	, m_thread(this)
	, m_conn(bClient, qstrAddr, usPort, localAddr, localPort)
{
	if (type->IsNull())
	{
		throw("The AGV type cannot be null");
	}

	QObject::connect(&m_conn, &SlaveConnect::signalConnected, this, &AgvBase::slotConnected);
	QObject::connect(&m_conn, &SlaveConnect::signalConnectionBreak, this, &AgvBase::slotConnectionBreak);
	QObject::connect(&m_conn, &SlaveConnect::signalProcess, this, &AgvBase::slotProcess);
	QObject::connect(this, &AgvBase::signalWriteData, &m_conn, static_cast<bool (SlaveConnect::*)(QByteArray) const>(&SlaveConnect::WriteData));

	moveToThread(&m_thread);
	m_thread.start();

	QObject::connect(&m_timer, &QTimer::timeout, this, &AgvBase::slotSendPacket);
	m_timer.setInterval(100);
	m_timer.start();
}

AgvBase::~AgvBase()
{
	m_thread.quit();
	m_thread.wait(1000);
	m_timer.stop();
}

QString AgvBase::GetActionInfo() const
{
	QString _str = "unknown";

	switch (m_attribute.GetExecution())
	{
	case AgvExecution::Act_None:
		_str = QString::fromLocal8Bit("未开始执行:");
		break;
	case AgvExecution::Act_Exe:
		_str = QString::fromLocal8Bit("正在执行:");
		break;
	case AgvExecution::Act_Fin:
		_str = QString::fromLocal8Bit("已执行完成:");
		break;
	default:
		_str = "unknown";
		break;
	}

	_str.append(QString::fromLocal8Bit("动作(%1)").arg(GetActionName()));

	return _str;
}

void AgvBase::ProcessPacket(const ByteArrayList& list)
{
	ProtocolType type = m_pType->GetProtocol()->GetType();

	switch (type)
	{
	case ProtocolType::Protocol_Stm32:
		ProcessPacketStm32(list);
		break;
	case ProtocolType::Protocol_Plc:
		ProcessPacketPlc(list);
		break;
	}
	return;
}

void AgvBase::ProcessPacketStm32(const ByteArrayList& list)
{
	for (ByteArrayList::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		int index = 0;

		const unsigned char* data = it->get();

		// 获取ID
		id_t id = 0;	/*!< 编号 */

		for (int i = 0; i < sizeof(id_t); ++i, ++index)
		{
			unsigned char tmp = *(data + index);
			id |= (tmp << 8 * (sizeof(id_t) - i - 1));
		}

		if (id != m_id)
		{
			continue;
		}

		// 获取功能
		unsigned char func = *(data + index++) & 0xFF;	/*!< 功能 */

		switch (static_cast<AgvCmdFunc>(func))
		{
		case AgvCmdFunc::Func_Heartbeat:
		{
			AgvAttr attr;

			// 模式
			for (int i = 0; i < sizeof(AgvAttr::mode_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_mode |= (tmp << 8 * (sizeof(AgvAttr::mode_t) - i - 1));
			}

			// 状态
			for (int i = 0; i < sizeof(AgvAttr::status_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_status |= (tmp << 8 * (sizeof(AgvAttr::status_t) - i - 1));
			}

			// 速度
			for (int i = 0; i < sizeof(AgvAttr::speed_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_speed |= (tmp << 8 * (sizeof(AgvAttr::speed_t) - i - 1));
			}

			// 电量
			for (int i = 0; i < sizeof(AgvAttr::battery_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_battery |= (tmp << 8 * (sizeof(AgvAttr::battery_t) - i - 1));
			}

			// 当前坐标
			for (int i = 0; i < sizeof(rfid_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_curLocat |= (tmp << 8 * (sizeof(rfid_t) - i - 1));
			}

			// 终点坐标
			for (int i = 0; i < sizeof(rfid_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_endLocat |= (tmp << 8 * (sizeof(rfid_t) - i - 1));
			}

			// 载货数量
			for (int i = 0; i < sizeof(AgvAttr::cargo_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_cargo |= (tmp << 8 * (sizeof(AgvAttr::cargo_t) - i - 1));
			}

			// 异常信息
			for (int i = 0; i < sizeof(AgvAttr::error_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_error |= (tmp << 8 * (sizeof(AgvAttr::error_t) - i - 1));
			}

			// 动作信息
			for (int i = 0; i < sizeof(AgvAttr::action_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_action |= (tmp << 8 * (sizeof(AgvAttr::action_t) - i - 1));
			}

			// 动作状态
			for (int i = 0; i < sizeof(AgvAttr::execute_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_execute |= (tmp << 8 * (sizeof(AgvAttr::execute_t) - i - 1));
			}

			if (attr.m_curLocat == 0 && attr.GetMode() == AgvMode::Mode_Auto)
			{
				attr.m_curLocat = m_attribute.m_curLocat;
			}

			if (m_attribute.Update(attr))
			{
				emit signalUpdate(m_id);
			}

			break;
		}
		case AgvCmdFunc::Func_Move:
		{
			break;
		}
		case AgvCmdFunc::Func_Action:
		{
			break;
		}
		case AgvCmdFunc::Func_Traffic:
		{
			break;
		}
		case AgvCmdFunc::Func_Status:
		{
			break;
		}
		case AgvCmdFunc::Func_Speed:
		{
			break;
		}
		default:
			break;
		}
	}

	return;
}

void AgvBase::ProcessPacketPlc(const ByteArrayList& list)
{
	for (ByteArrayList::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		int index = 0;

		const unsigned char* data = it->get();

		// 获取ID
		id_t id = 0;	/*!< 编号 */

		for (int i = 0; i < sizeof(id_t); ++i, ++index)
		{
			unsigned char tmp = *(data + index);
			id |= (tmp << 8 * (sizeof(id_t) - i - 1));
		}

		if (id != m_id)
		{
			continue;
		}

		// 获取功能
		unsigned char func = *(data + index++) & 0xFF;	/*!< 功能 */

		switch (static_cast<AgvCmdFunc>(func))
		{
		case AgvCmdFunc::Func_Heartbeat:
		{
			AgvAttr attr;

			// 模式
			for (int i = 0; i < sizeof(AgvAttr::mode_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_mode |= (tmp << 8 * (sizeof(AgvAttr::mode_t) - i - 1));
			}

			// 状态
			for (int i = 0; i < sizeof(AgvAttr::status_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_status |= (tmp << 8 * (sizeof(AgvAttr::status_t) - i - 1));
			}

			// 速度
			for (int i = 0; i < sizeof(AgvAttr::speed_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_speed |= (tmp << 8 * (sizeof(AgvAttr::speed_t) - i - 1));
			}

			// 电量
			for (int i = 0; i < sizeof(AgvAttr::battery_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_battery |= (tmp << 8 * (sizeof(AgvAttr::battery_t) - i - 1));
			}

			// 当前坐标
			for (int i = 0; i < sizeof(rfid_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_curLocat |= (tmp << 8 * (sizeof(rfid_t) - i - 1));
			}

			// 终点坐标
			for (int i = 0; i < sizeof(rfid_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_endLocat |= (tmp << 8 * (sizeof(rfid_t) - i - 1));
			}

			// 载货数量
			for (int i = 0; i < sizeof(AgvAttr::cargo_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_cargo |= (tmp << 8 * (sizeof(AgvAttr::cargo_t) - i - 1));
			}

			// 异常信息
			for (int i = 0; i < sizeof(AgvAttr::error_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_error |= (tmp << 8 * (sizeof(AgvAttr::error_t) - i - 1));
			}

			// 动作信息
			for (int i = 0; i < sizeof(AgvAttr::action_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_action |= (tmp << 8 * (sizeof(AgvAttr::action_t) - i - 1));
			}

			// 动作状态
			for (int i = 0; i < sizeof(AgvAttr::execute_t); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_execute |= (tmp << 8 * (sizeof(AgvAttr::execute_t) - i - 1));
			}

			if (attr.m_curLocat == 0 && attr.GetMode() == AgvMode::Mode_Auto)
			{
				attr.m_curLocat = m_attribute.m_curLocat;
			}

			if (m_attribute.Update(attr))
			{
				emit signalUpdate(m_id);
			}

			break;
		}
		case AgvCmdFunc::Func_Move:
		{
			break;
		}
		case AgvCmdFunc::Func_Action:
		{
			break;
		}
		case AgvCmdFunc::Func_Traffic:
		{
			break;
		}
		case AgvCmdFunc::Func_Status:
		{
			break;
		}
		case AgvCmdFunc::Func_Speed:
		{
			break;
		}
		default:
			break;
		}
	}

	return;
}

void AgvBase::slotProcess(QByteArray data)
{
	m_byarrBuffer += data;
	ByteArray bf(m_byarrBuffer);
	m_byarrBuffer.clear();
	ProcessPacket(m_pType->GetProtocol()->Process(bf));
	m_byarrBuffer = bf.toQByteAarray();

	return;
}

void AgvBase::slotConnected()
{
	emit signalConnected(m_id);

	return;
}

void AgvBase::slotConnectionBreak()
{
	m_attribute.m_battery = 0;

	m_listSend.clear();

	emit signalConnectionBreak(m_id);

	return;
}

void AgvBase::slotSendPacket()
{
	m_conn.Connect();

	//m_timer.stop();

	Heartbeat();

	for (ByteArrayList::const_iterator it = m_listSend.begin(); it != m_listSend.end(); it = m_listSend.erase(it))
	{
		emit signalWriteData(it->toQByteAarray());

		QThread::msleep(100);
	}

	//m_timer.start();

	return;
}

CmdError AgvBase::WakeUp()
{
	if (m_attribute.GetStatus() != AgvStatus::Sta_Sleep)
	{
		// 未处于休眠状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_WakeUp);
}

CmdError AgvBase::Reset()
{
	AgvStatus status = m_attribute.GetStatus();

	if (status == AgvStatus::Sta_Wait)
	{
		// 已处于待机状态
		return CmdError::Cmd_StatusError;
	}

	if (status != AgvStatus::Sta_RmtScream
		&& status != AgvStatus::Sta_AllScream)
	{
		// 不处于可以复位的状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_Reset);
}

CmdError AgvBase::Restart()
{
	if (m_attribute.IsNull())
	{
		// 已经重置或在初始化状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_Restart);
}

CmdError AgvBase::Sleep()
{
	if (m_attribute.GetStatus() == AgvStatus::Sta_Sleep)
	{
		// 已处于休眠状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_Sleep);
}

CmdError AgvBase::Pause()
{
	if (m_attribute.GetStatus() == AgvStatus::Sta_Pause)
	{
		// 已处于暂停状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_Pause);
}

CmdError AgvBase::Continue()
{
	if (m_attribute.GetStatus() == AgvStatus::Sta_Pause)
	{
		// 未处于暂停状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_Continue);
}

CmdError AgvBase::RemoteScream()
{
	AgvStatus status = m_attribute.GetStatus();

	if (status == AgvStatus::Sta_RmtScream
		|| status == AgvStatus::Sta_Scream)
	{
		// 已经处于急停状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_RmtScream);
}

CmdError AgvBase::AllScream()
{
	AgvStatus status = m_attribute.GetStatus();

	if (status == AgvStatus::Sta_Scream
		|| status == AgvStatus::Sta_RmtScream
		|| status == AgvStatus::Sta_AllScream)
	{
		// 已处于急停状态
		return CmdError::Cmd_StatusError;
	}

	return StatusControl(AgvStatusCtrl::Ctrl_AllScream);
}

CmdError AgvBase::Shutdown()
{
	return StatusControl(AgvStatusCtrl::Ctrl_Shutdonw);
}

CmdError AgvBase::Move(const rfid_t& location)
{
	if (location == 0)
	{
		// 无效的坐标
		return CmdError::Cmd_ParamError;
	}

	if (m_attribute.GetStatus() != AgvStatus::Sta_Wait)
	{
		// 未处于待机状态
		return CmdError::Cmd_StatusError;
	}

	if (m_attribute.m_curLocat == location)
	{
		// 已到达的坐标
		return CmdError::Cmd_LocationError;
	}

	AgvAction act = m_attribute.GetAction();

	if ((act != AgvAction::Act_None && act != AgvAction::Act_Move) && m_attribute.GetExecution() != AgvExecution::Act_Fin)
	{
		// 有动作 并且 动作未完成
		return CmdError::Cmd_ActionError;
	}

	ProtocolType type = m_pType->GetProtocol()->GetType();

	switch (type)
	{
	case ProtocolType::Protocol_Stm32:
		return MoveStm32(location);
	case ProtocolType::Protocol_Plc:
		return MovePlc(location);
	}

	return CmdError::Cmd_Unknow;
}

CmdError AgvBase::Action(const AgvAction& action)
{
	if (action == AgvAction::Act_None)
	{
		// 无效的动作
		return CmdError::Cmd_ParamError;
	}

	if (m_attribute.GetStatus() != AgvStatus::Sta_Wait)
	{
		// 未处于待机状态
		return CmdError::Cmd_StatusError;
	}

	return ActionControl(action);
}

CmdError AgvBase::StopAction()
{
	if (m_attribute.GetAction() == AgvAction::Act_None)
	{
		// 无动作
		return CmdError::Cmd_ActionError;
	}

	if (m_attribute.GetExecution() == AgvExecution::Act_Fin)
	{
		// 动作已经执行完成
		return CmdError::Cmd_ActionError;
	}

	return ActionControl(AgvAction::Act_None);
}

CmdError AgvBase::TrafficPass()
{
	if (m_attribute.GetStatus() != AgvStatus::Sta_Traffic)
	{
		// 未处于交通管制状态
		return CmdError::Cmd_StatusError;
	}

	return TrafficControl(0x01);
}

CmdError AgvBase::TrafficStop()
{
	if (m_attribute.GetStatus() == AgvStatus::Sta_Traffic)
	{
		// 已处于交通管制状态
		return CmdError::Cmd_StatusError;
	}

	return TrafficControl(0x00);
}

CmdError AgvBase::SpeedControl(const AgvAttr::speed_t& speed)
{
	if (speed > static_cast<AgvAttr::speed_t>(AgvSpeed::Speed_MaxForward) || speed < static_cast<AgvAttr::speed_t>(AgvSpeed::Speed_MaxReverse))
	{
		// 参数取值错误
		return CmdError::Cmd_ParamError;
	}

	if (m_attribute.GetSpeed() > AgvSpeed::Speed_Stop)
	{
		// 正向行驶
		if (speed < static_cast<AgvAttr::speed_t>(AgvSpeed::Speed_Stop))
		{
			// 不能给予反向行驶的速度
			// 参数取值错误
			return CmdError::Cmd_ParamError;
		}
	}
	else if (m_attribute.GetSpeed() == AgvSpeed::Speed_Stop)
	{
		// 停止行驶
		// 参数取值错误
		return CmdError::Cmd_ParamError;
	}
	else
	{
		// 反向行驶
		if (speed > static_cast<AgvAttr::speed_t>(AgvSpeed::Speed_Stop))
		{
			// 不能给予正向行驶的速度
			// 参数取值错误
			return CmdError::Cmd_ParamError;
		}
	}

	int index = 0;
	int len = 1 + sizeof(rfid_t) + sizeof(AgvAttr::speed_t);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<unsigned char>(AgvCmdFunc::Func_Speed);

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	// 速度
	for (unsigned int i = sizeof(AgvAttr::speed_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((speed >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SentCommand(command.get(), len);
}

void AgvBase::Heartbeat()
{
	ProtocolType type = m_pType->GetProtocol()->GetType();

	switch (type)
	{
	case ProtocolType::Protocol_Stm32:
		HeartbeatStm32();
		break;
	case ProtocolType::Protocol_Plc:
		HeartbeatPlc();
		break;
	}
	return;
}

void AgvBase::HeartbeatStm32()
{
	int index = 0;
	int len = 1 + sizeof(AgvAttr::mode_t) + sizeof(AgvAttr::status_t) + sizeof(AgvAttr::speed_t) + sizeof(AgvAttr::battery_t)
		+ sizeof(rfid_t) + sizeof(rfid_t) + sizeof(AgvAttr::cargo_t) + sizeof(AgvAttr::error_t) + sizeof(AgvAttr::action_t)
		+ sizeof(AgvAttr::execute_t);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<unsigned char>(AgvCmdFunc::Func_Heartbeat);

	// 模式
	for (unsigned int i = sizeof(AgvAttr::mode_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_mode >> 8 * (i - 1)) & 0xFF);
	}

	// 状态
	for (unsigned int i = sizeof(AgvAttr::status_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_status >> 8 * (i - 1)) & 0xFF);
	}

	// 速度
	for (unsigned int i = sizeof(AgvAttr::speed_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_speed >> 8 * (i - 1)) & 0xFF);
	}

	// 电量
	for (unsigned int i = sizeof(AgvAttr::battery_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_battery >> 8 * (i - 1)) & 0xFF);
	}

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	//  终点RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_endLocat >> 8 * (i - 1)) & 0xFF);
	}

	//  载货数量
	for (unsigned int i = sizeof(AgvAttr::cargo_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_cargo >> 8 * (i - 1)) & 0xFF);
	}

	// 异常信息
	for (unsigned int i = sizeof(AgvAttr::error_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_error >> 8 * (i - 1)) & 0xFF);
	}

	// 动作信息
	for (unsigned int i = sizeof(AgvAttr::action_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_action >> 8 * (i - 1)) & 0xFF);
	}

	// 动作状态
	for (unsigned int i = sizeof(AgvAttr::execute_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_execute >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	SendData(command.get(), len);

	return;
}

void AgvBase::HeartbeatPlc()
{
	int index = 0;
	int len = 1 + sizeof(AgvAttr::mode_t) + sizeof(AgvAttr::status_t) + sizeof(AgvAttr::speed_t) + sizeof(AgvAttr::battery_t)
		+ sizeof(rfid_t) + sizeof(rfid_t) + sizeof(AgvAttr::cargo_t) + sizeof(AgvAttr::error_t) + sizeof(AgvAttr::action_t)
		+ sizeof(AgvAttr::execute_t);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<unsigned char>(AgvCmdFunc::Func_Heartbeat);

	// 模式
	for (unsigned int i = sizeof(AgvAttr::mode_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_mode >> 8 * (i - 1)) & 0xFF);
	}

	// 状态
	for (unsigned int i = sizeof(AgvAttr::status_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_status >> 8 * (i - 1)) & 0xFF);
	}

	// 速度
	for (unsigned int i = sizeof(AgvAttr::speed_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_speed >> 8 * (i - 1)) & 0xFF);
	}

	// 电量
	for (unsigned int i = sizeof(AgvAttr::battery_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_battery >> 8 * (i - 1)) & 0xFF);
	}

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	//  终点RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_endLocat >> 8 * (i - 1)) & 0xFF);
	}

	//  载货数量
	for (unsigned int i = sizeof(AgvAttr::cargo_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_cargo >> 8 * (i - 1)) & 0xFF);
	}

	// 异常信息
	for (unsigned int i = sizeof(AgvAttr::error_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_error >> 8 * (i - 1)) & 0xFF);
	}

	// 动作信息
	for (unsigned int i = sizeof(AgvAttr::action_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_action >> 8 * (i - 1)) & 0xFF);
	}

	// 动作状态
	for (unsigned int i = sizeof(AgvAttr::execute_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_execute >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	SendData(command.get(), len);

	return;
}

CmdError AgvBase::MoveStm32(const rfid_t& location)
{
	int index = 0;
	int len = 1 + sizeof(rfid_t) * 2;

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Move);

	// 当前坐标
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	// 终点坐标
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((location >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SentCommand(command.get(), len);
}

CmdError AgvBase::MovePlc(const rfid_t& location)
{
	int index = 0;
	int len = 13;

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Move);

	index += 4;

	// 当前坐标
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	// 终点坐标
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((location >> 8 * (i - 1)) & 0xFF);
	}

	return SentCommand(command.get(), len);
}

CmdError AgvBase::ActionControl(const AgvAction& action)
{
	AgvAttr::action_t act = static_cast<AgvAttr::action_t>(action);

	if (act >= UCHAR_MAX - 1)
	{
		// 无效的动作
		return CmdError::Cmd_ParamError;
	}

	if (m_attribute.GetAction() == action)
	{
		// 相同的动作
		return CmdError::Cmd_ActionError;
	}

	ProtocolType type = m_pType->GetProtocol()->GetType();

	switch (type)
	{
	case ProtocolType::Protocol_Stm32:
		return ActionControlStm32(action);
	case ProtocolType::Protocol_Plc:
		return ActionControlPlc(action);
	}

	return CmdError::Cmd_Unknow;
}

CmdError AgvBase::ActionControlStm32(const AgvAction& action)
{
	AgvAttr::action_t act = static_cast<AgvAttr::action_t>(action);

	int index = 0;
	int len = 1 + sizeof(rfid_t) + sizeof(AgvAttr::action_t);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Action);

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	// 动作
	for (unsigned int i = sizeof(AgvAttr::action_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((act >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SentCommand(command.get(), len);
}

CmdError AgvBase::ActionControlPlc(const AgvAction& action)
{
	AgvAttr::action_t act = static_cast<AgvAttr::action_t>(action);

	int index = 0;
	int len = 13;

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Action);

	index += 4;

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	index += 4;

	// 动作
	for (unsigned int i = sizeof(AgvAttr::action_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((act >> 8 * (i - 1)) & 0xFF);
	}

	return SentCommand(command.get(), len);
}

CmdError AgvBase::StatusControl(const AgvStatusCtrl& cmd)
{
	// 合成指令
	unsigned char command[2] = { static_cast<unsigned char>(AgvCmdFunc::Func_Status),static_cast<unsigned char>(cmd) };

	return SentCommand(command, 2);
}

CmdError AgvBase::TrafficControl(const unsigned char& cmd)
{
	ProtocolType type = m_pType->GetProtocol()->GetType();

	switch (type)
	{
	case ProtocolType::Protocol_Stm32:
		return TrafficControlStm32(cmd);
	case ProtocolType::Protocol_Plc:
		return TrafficControlPlc(cmd);
	}

	return CmdError::Cmd_Unknow;
}

CmdError AgvBase::TrafficControlStm32(const unsigned char& cmd)
{
	// 合成指令
	int index = 0;
	int len = 2 + sizeof(rfid_t);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Traffic);

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	command[index++] = cmd;

	assert(len == index);

	return SentCommand(command.get(), len);
}

CmdError AgvBase::TrafficControlPlc(const unsigned char& cmd)
{
	// 合成指令
	int index = 0;
	int len = 13;

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	command[index++] = static_cast<char>(AgvCmdFunc::Func_Traffic);

	index += 4;

	// 当前RFID
	for (unsigned int i = sizeof(rfid_t); i > 0; --i)
	{
		command[index++] = static_cast<unsigned char>((m_attribute.m_curLocat >> 8 * (i - 1)) & 0xFF);
	}

	command[index++] = cmd;

	return SentCommand(command.get(), len);
}

CmdError AgvBase::SentCommand(const unsigned char* data, const int& maxSize)
{
	if (m_attribute.GetMode() != AgvMode::Mode_Auto)
	{
		return CmdError::Cmd_ModeError;
	}

	if (m_attribute.m_battery <= static_cast<AgvAttr::battery_t>(AgvBattery::Power_Off))
	{
		return CmdError::Cmd_PowerError;
	}

	return SendData(data, maxSize);
}

CmdError AgvBase::SendData(const unsigned char* data, const int& maxSize)
{
	if (m_conn.IsConnected() == false)
	{
		return CmdError::Cmd_NetError;
	}

	int index = 0;
	int len = sizeof(id_t) + maxSize;

	std::unique_ptr<unsigned char[]> cmd(new unsigned char[len]);

	memset(cmd.get(), 0, len);

	for (int i = sizeof(id_t); i > 0; --i)
	{
		cmd[index++] = static_cast<char>((m_id >> 8 * (i - 1)) & 0xFF);
	}

	memcpy(cmd.get() + index, data, maxSize);

	m_listSend.push_back(m_pType->GetProtocol()->Create(cmd.get(), len));

	return CmdError::Cmd_Success;
}

#include "ZigbeeControler.h"
#include <QDebug>
#include <QEventLoop>

ProtocolZigbee ZigbeeControler::m_protocol;

ZigbeeControler::ZigbeeControler()
	: m_serialName()
	, m_baudBite(QSerialPort::UnknownBaud)
	, m_dataBite(QSerialPort::UnknownDataBits)
	, m_parityBite(QSerialPort::UnknownParity)
	, m_stopBite(QSerialPort::UnknownStopBits)
	, m_flowCtrl(QSerialPort::UnknownFlowControl)
{
}

ZigbeeControler::ZigbeeControler(const QString& name,
	const QSerialPort::BaudRate& baud,
	const QSerialPort::FlowControl& flowCtrl,
	const QSerialPort::Parity& parity,
	const QSerialPort::DataBits& data,
	const QSerialPort::StopBits& stop)
	: m_serialName(name)
	, m_baudBite(baud)
	, m_dataBite(data)
	, m_parityBite(parity)
	, m_stopBite(stop)
	, m_flowCtrl(flowCtrl)
{
}

ZigbeeControler::~ZigbeeControler()
{
	Close();
}

bool ZigbeeControler::LoadCaller(const QJsonDocument& doc)
{
	QString file = "Zigbee caller";
	if (doc.isEmpty())
	{
		qDebug() << "Load " << file << " config failed:The document is empty" << endl;
		return false;
	}

	if (doc.isObject() == false)
	{
		qDebug() << "Load " << file << " config failed:Can not find Object" << endl;

		return false;
	}

	QJsonObject obj = doc.object();	/*!< JSON对象 */

	if (obj.isEmpty())
	{
		// JSON对象为空

		qDebug() << "Load " << file << " config failed:The object is empty" << endl;
		return false;
	}

	QString key = "ZigbeeCaller";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadCaller(obj.value(key).toObject());
}

bool ZigbeeControler::LoadCaller(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		// JSON对象为空

		qDebug() << "Load Zigbee caller config failed:The object is empty" << endl;
		return false;
	}

	m_mapCallers.clear();

	foreach(const QString & key, obj.keys())
	{
		unsigned short id = static_cast<unsigned short>(key.toInt());

		QJsonObject attr = obj.value(key).toObject();

		std::string addr = attr.value("MAC").toString().toStdString();
		unsigned short station = static_cast<unsigned short>(attr.value("Bind").toInt());

		m_mapCallers[id] = std::make_shared<ZigbeeCaller>(id, addr, station);
	}

	return true;
}

CallerPtr ZigbeeControler::callerPtr(const unsigned short& id) const
{
	if (id == 0)
	{
		return nullptr;
	}

	if (m_mapCallers.find(id) == m_mapCallers.end())
	{
		return nullptr;
	}

	return m_mapCallers.at(id);
}

CallerPtr ZigbeeControler::GetCallerPtr(const std::string& addr) const
{
	if (addr.empty())
	{
		return nullptr;
	}

	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		if (it->second->GetAddress() == addr)
		{
			return it->second;
		}
	}

	return nullptr;
}

CallerPtr ZigbeeControler::GetCallerPtr(const unsigned short& id) const
{
	if (id == 0)
	{
		return nullptr;
	}

	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		if (it->second->GetStation() == id)
		{
			return it->second;
		}
	}

	return nullptr;
}

const ZgbCaller ZigbeeControler::caller(const unsigned short& id) const
{
	CallerPtr ptr = callerPtr(id);

	if (ptr == nullptr)
	{
		return ZgbCaller();
	}

	return *ptr;
}

const ZgbCaller ZigbeeControler::GetCaller(const std::string& addr) const
{
	CallerPtr ptr = GetCallerPtr(addr);

	if (ptr == nullptr)
	{
		return ZgbCaller();
	}

	return *ptr;
}

const ZgbCaller ZigbeeControler::GetCaller(const unsigned short& id)
{
	CallerPtr ptr = GetCallerPtr(id);

	if (ptr == nullptr)
	{
		return ZgbCaller();
	}

	return *ptr;
}

bool ZigbeeControler::Open()
{
	if (m_serialName.isNull() || m_serialName.isEmpty())
	{
		qDebug() << "Open serial port failed:" << "The port name cannot be empty" << endl;
		return false;
	}

	QString comName = m_serialName.toUpper();

	QSerialPortInfo comInfo;
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		if (info.portName() == comName || info.serialNumber() == comName)
		{
			comInfo = info;

			break;
		}
	}

	if (comInfo.isNull())
	{
		qDebug() << "Open serial port failed:" << "No serial port found" << endl;
		return false;
	}

	moveToThread(&m_thread);
	m_thread.start();
	m_timer.setInterval(100);

	QObject::connect(&m_serialPort, &QSerialPort::readyRead, this, &ZigbeeControler::slotReadyRead);
	QObject::connect(&m_timer, &QTimer::timeout, this, &ZigbeeControler::slotSentPacket,Qt::ConnectionType::DirectConnection);

	if (m_serialPort.isOpen())
	{
		Close();
	}

	m_serialPort.setPort(comInfo);

	if (m_serialPort.open(QIODevice::ReadWrite) == false)
	{
		qDebug() << "Open serial port failed:" << m_serialPort.errorString() << endl;

		emit signalSerialPortStateChange(QString::fromLocal8Bit("打开失败:%1").arg(m_serialPort.errorString()));

		return false;
	}

	m_serialPort.setBaudRate(m_baudBite);
	m_serialPort.setParity(m_parityBite);
	m_serialPort.setDataBits(m_dataBite);
	m_serialPort.setStopBits(m_stopBite);
	m_serialPort.setFlowControl(m_flowCtrl);

	m_serialPort.clearError();
	m_serialPort.clear();

	m_timer.start();

	emit signalSerialPortStateChange(QString::fromLocal8Bit("已打开(串口号:%1,波特率:%2)").arg(m_serialPort.portName()).arg(m_serialPort.baudRate()));

	return true;
}

bool ZigbeeControler::Open(const QString& name,
	const QSerialPort::BaudRate& baud,
	const QSerialPort::FlowControl& flowCtrl,
	const QSerialPort::Parity& parity,
	const QSerialPort::DataBits& data,
	const QSerialPort::StopBits& stop)
{
	m_serialName = name;
	m_baudBite = baud;
	m_dataBite = data;
	m_flowCtrl = flowCtrl;
	m_parityBite = parity;
	m_stopBite = stop;

	return Open();
}

void ZigbeeControler::Close()
{
	m_thread.quit();
	m_thread.wait(1000);
	m_timer.stop();

	if (m_serialPort.isOpen())
	{
		m_serialPort.close();

		emit signalSerialPortStateChange(QString::fromLocal8Bit("关闭"));
	}

	return;
}

bool ZigbeeControler::IsScream() const
{
	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		if (it->second->GetAttribute().GetScreamSignal() == Signal::Signal_On && it->second->IsEnabled())
		{
			return true;
		}
	}

	return false;
}

bool ZigbeeControler::LoadSignal(const unsigned short& id, const bool& use, const CallerAttr& attr)
{
	CallerPtr ptr = callerPtr(id);

	if (ptr == nullptr)
	{
		return false;
	}

	ptr->UpdateAttribute(attr);
	ptr->SetEnableUse(use);

	return true;
}

void ZigbeeControler::EnableUseAllCaller()
{
	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		EnableUseCaller(*(it->second));
	}

	return;
}

void ZigbeeControler::EnableUseCaller(const unsigned short& id)
{
	CallerPtr ptr = callerPtr(id);

	if (ptr == nullptr)
	{
		return;
	}

	return EnableUseCaller(*ptr);
}
void ZigbeeControler::EnableUseCaller(ZigbeeCaller& caller)
{
	caller.Enable();

	emit signalUpdateCallerEnableUse(caller.GetId());

	return;
}

void ZigbeeControler::DisableUseAllCaller()
{
	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		DisableUseCaller(*(it->second));
	}

	return;
}

void ZigbeeControler::DisableUseCaller(const unsigned short& id)
{
	CallerPtr ptr = callerPtr(id);

	if (ptr == nullptr)
	{
		return;
	}

	return DisableUseCaller(*ptr);
}

void ZigbeeControler::DisableUseCaller(ZigbeeCaller& caller)
{
	caller.Disable();

	emit signalUpdateCallerEnableUse(caller.GetId());

	return;
}

void ZigbeeControler::ProcessPacket(const ByteArrayList& list)
{
	for (ByteArrayList::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		int index = 0;

		const unsigned char* data = it->get();

		// 获取ID
		unsigned short id = 0;	/*!< 编号 */

		for (int i = 0; i < sizeof(id); ++i, ++index)
		{
			unsigned char tmp = *(data + index);
			id |= (tmp << 8 * (sizeof(id) - i - 1));
		}

		// 获取功能
		unsigned char func = *(data + index++) & 0xFF;	/*!< 功能 */

		switch (static_cast<ZigbeeCmdFunc>(func))
		{
		case ZigbeeCmdFunc::Func_Heartbeat:
		{
			CallerAttr attr;

			// 急停信号
			for (int i = 0; i < sizeof(attr.m_sigScream); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_sigScream |= (tmp << 8 * (sizeof(attr.m_sigScream) - i - 1));
			}

			// 呼叫信号
			for (int i = 0; i < sizeof(attr.m_sigCall); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_sigCall |= (tmp << 8 * (sizeof(attr.m_sigCall) - i - 1));
			}

			// 取消信号
			for (int i = 0; i < sizeof(attr.m_sigCancel); ++i, ++index)
			{
				unsigned char tmp = *(data + index);
				attr.m_sigCancel |= (tmp << 8 * (sizeof(attr.m_sigCancel) - i - 1));
			}

			UpdateCallerSignal(id, attr);

			break;
		}
		case ZigbeeCmdFunc::Func_Set:
			break;
		case ZigbeeCmdFunc::Func_Clear:
			break;
		default:
			break;
		}
	}

	return;
}

void ZigbeeControler::UpdateCallerSignal(const unsigned short& id, const CallerAttr& attr)
{
	CallerPtr ptr = callerPtr(id);

	if (ptr == nullptr)
	{
		return;
	}

	bool update = false;

	CallerAttr* attrPtr = ptr->GetAttributePtr();

	if (attrPtr->UpdateScreamSignal(attr.m_sigScream))
	{
		update = true;
	}

	if (attrPtr->UpdateCancelSignal(attr.m_sigCancel))
	{
		update = true;
	}

	if (attrPtr->UpdateCallSignal(attr.m_sigCall))
	{
		update = true;
	}

	if (update)
	{
		emit signalUpdateCallerSignal(id);
	}

	return;
}

void ZigbeeControler::slotReadyRead()
{
	if (m_serialPort.isReadable())
	{
		m_byarrBuffer += m_serialPort.readAll();

		ByteArray bf(m_byarrBuffer);
		m_byarrBuffer.clear();
		ProcessPacket(m_protocol.Process(bf));
		m_byarrBuffer = bf.toQByteAarray();
	}

	return;
}

void ZigbeeControler::slotSentPacket()
{
	//m_timer.stop();

	Heartbeat();

	for (ByteArrayList::iterator it = m_listSend.begin(); it != m_listSend.end(); it = m_listSend.erase(it))
	{
		quint64 res = m_serialPort.write(it->data(), it->size());

		if (m_serialPort.waitForBytesWritten(1000))
		{
			QThread::msleep(100);
		}

		if (res != it->size())
		{
			qDebug() << "Write date failed:" << res << endl;
		}
	}

	//m_timer.start();

	return;
}

void ZigbeeControler::Heartbeat()
{
	for (CallerArray::const_iterator it = m_mapCallers.begin(); it != m_mapCallers.end(); ++it)
	{
		Heartbeat(*(it->second));
	}

	return;
}

void ZigbeeControler::Heartbeat(const ZgbCaller& caller)
{
	const unsigned short id = caller.GetId();
	const CallerAttr attr = caller.GetAttribute();

	int index = 0;
	int len = sizeof(id) + 1 + sizeof(Signal) * 3;

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	// 编号
	for (int i = sizeof(id); i > 0; --i)
	{
		command[index++] = static_cast<char>((id >> 8 * (i - 1)) & 0xFF);
	}

	command[index++] = static_cast<char>(ZigbeeCmdFunc::Func_Heartbeat);

	// 急停信号
	for (unsigned int i = sizeof(Signal); i > 0; --i)
	{
		command[index++] = static_cast<char>((attr.m_sigScream >> 8 * (i - 1)) & 0xFF);
	}

	// 呼叫信号
	for (unsigned int i = sizeof(Signal); i > 0; --i)
	{
		command[index++] = static_cast<char>((attr.m_sigCall >> 8 * (i - 1)) & 0xFF);
	}

	// 取消信号
	for (unsigned int i = sizeof(Signal); i > 0; --i)
	{
		command[index++] = static_cast<char>((attr.m_sigCancel >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SendCommand(caller.GetAddress(), m_protocol.Create(command.get(), len));
}

void ZigbeeControler::signal(const unsigned short& id, const SignalType& type, const Signal& sig)
{
	if (id == 0)
	{
		return;
	}

	return SetSignal(caller(id), type, sig);
}

void ZigbeeControler::SetSignal(const std::string& addr, const SignalType& type, const Signal& sig)
{
	if (addr.empty())
	{
		return;
	}

	return SetSignal(GetCaller(addr), type, sig);
}

void ZigbeeControler::SetSignal(const unsigned short& id, const SignalType& type, const Signal& sig)
{
	if (id == 0)
	{
		return;
	}

	return SetSignal(GetCaller(id), type, sig);
}

void ZigbeeControler::SetSignal(const ZgbCaller& caller, const SignalType& type, const Signal& sig)
{
	if (caller.IsNull())
	{
		return;
	}

	const unsigned short id = caller.GetId();
	const CallerAttr attr = caller.GetAttribute();
	unsigned char sigType = static_cast<unsigned char>(type);
	unsigned char _sig = static_cast<unsigned char>(sig);

	switch (type)
	{
	case SignalType::Signal_Call:
	{
		if (attr.GetCallSignal() == sig)
		{
			return;
		}

		break;
	}
	case SignalType::Signal_Cancel:
	{
		if (attr.GetCancelSignal() == sig)
		{
			return;
		}

		if (sig != Signal::Signal_On && sig != Signal::Signal_Off)
		{
			// 无效的信号
			return;
		}
		break;
	}
	case SignalType::Signal_Scream:
		// 不能够设置急停信号
		return;
	default:
		// 无效的信号类型
		return;
	}

	int index = 0;
	int len = sizeof(id) + 1 + sizeof(SignalType) + sizeof(Signal);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	// 编号
	for (int i = sizeof(id); i > 0; --i)
	{
		command[index++] = static_cast<char>((id >> 8 * (i - 1)) & 0xFF);
	}

	command[index++] = static_cast<char>(ZigbeeCmdFunc::Func_Set);

	// 信号类型
	for (unsigned int i = sizeof(SignalType); i > 0; --i)
	{
		command[index++] = static_cast<char>((sigType >> 8 * (i - 1)) & 0xFF);
	}

	// 信号
	for (unsigned int i = sizeof(Signal); i > 0; --i)
	{
		command[index++] = static_cast<char>((_sig >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SendCommand(caller.GetAddress(), m_protocol.Create(command.get(), len));
}

void ZigbeeControler::clear(const unsigned short& id, const SignalType& type)
{
	if (id == 0)
	{
		return;
	}

	return ClearSignal(caller(id), type);
}

void ZigbeeControler::ClearSignal(const std::string& addr, const SignalType& type)
{
	if (addr.empty())
	{
		return;
	}

	return ClearSignal(GetCaller(addr), type);
}

void ZigbeeControler::ClearSignal(const unsigned short& id, const SignalType& type)
{
	if (id == 0)
	{
		return;
	}

	return ClearSignal(GetCaller(id), type);
}

void ZigbeeControler::ClearSignal(const ZgbCaller& caller, const SignalType& type)
{
	if (caller.IsNull())
	{
		return;
	}

	const unsigned short id = caller.GetId();
	const CallerAttr attr = caller.GetAttribute();
	unsigned char sigType = static_cast<unsigned char>(type);

	switch (type)
	{
	case SignalType::Signal_Call:
	{
		if (attr.GetCancelSignal() == Signal::Signal_Off)
		{
			return;
		}
		break;
	}
	case SignalType::Signal_Cancel:
	{
		if (attr.GetCancelSignal() == Signal::Signal_Off)
		{
			return;
		}
		break;
	}
	case SignalType::Signal_Scream:
		// 不能够清除急停信号
		return;
	default:
		// 无效的信号类型
		return;
	}

	int index = 0;
	int len = sizeof(id) + 1 + sizeof(SignalType);

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	memset(command.get(), 0, len);

	// 编号
	for (int i = sizeof(id); i > 0; --i)
	{
		command[index++] = static_cast<char>((id >> 8 * (i - 1)) & 0xFF);
	}

	command[index++] = static_cast<char>(ZigbeeCmdFunc::Func_Clear);

	// 信号类型
	for (unsigned int i = sizeof(SignalType); i > 0; --i)
	{
		command[index++] = static_cast<char>((sigType >> 8 * (i - 1)) & 0xFF);
	}

	assert(len == index);

	return SendCommand(caller.GetAddress(), m_protocol.Create(command.get(), len));
}

void ZigbeeControler::SendCommand(const std::string addr, const ByteArray& data)
{
	int index = 0;
	int len = addr.length() + data.size();

	std::unique_ptr<unsigned char[]> command(new unsigned char[len]);

	// 地址
	for (int i = 0; i < addr.length(); ++i)
	{
		command[index++] = addr.at(i);
	}

	// 数据
	std::memcpy(command.get() + index, data.get(), data.size());

	//assert(len == index);

	return m_listSend.push_back(ByteArray(command.get(), len));
}

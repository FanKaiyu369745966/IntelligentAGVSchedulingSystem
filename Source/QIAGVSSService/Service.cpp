#include "Service.h"

Service::Service(const QSqlDatabase& db)
	: ServiceBase(db)
	, m_bAllScream(false)
{
	QObject::connect(this, &ServiceBase::signalUpdateAGVStatus, this, &Service::slotUpdateAGVStatus, Qt::ConnectionType::DirectConnection);

	QObject::connect(&m_timer, &QTimer::timeout, this, &Service::slotThread);
	QObject::connect(&m_clock, &QTimer::timeout, this, &Service::slotUpdateClock);

	// 信号-槽 服务端接收客户端连接请求
	QObject::connect(&m_server, &QTcpServer::newConnection, this, &Service::slotNewConnection);
	// 信号-槽 服务端接收客户端请求异常
	QObject::connect(&m_server, &QTcpServer::acceptError, this, &Service::slotServerError);

	// 信号-槽 呼叫器状态更新
	QObject::connect(&m_zgbCtrler, &ZigbeeControler::signalUpdateCallerSignal, this, &Service::slotUpdateCallerSignal);
	// 信号-槽 呼叫器启用状态更新
	QObject::connect(&m_zgbCtrler, &ZigbeeControler::signalUpdateCallerEnableUse, this, &Service::slotUpdateCallerEnableUse);
	// 信号-槽 控制器状态更新
	QObject::connect(&m_zgbCtrler, &ZigbeeControler::signalSerialPortStateChange, this, &Service::slotSerialPortStateChange);

	moveToThread(&m_thread);
	m_thread.start();

	m_timer.setInterval(100);
	m_clock.setInterval(1000);
}

Service::~Service()
{
	delete this;
}

bool Service::Start(const QJsonDocument& doc)
{
	Initialize(doc);

	if (LoadConfig() == false)
	{
		return false;
	}

	return Open();
}

bool Service::Start(const QJsonDocument& doc, const QJsonDocument& map)
{
	Initialize(doc);

	if (LoadConfig(map) == false)
	{
		return false;
	}

	return Open();
}

void Service::Exit()
{
	if (m_server.isListening())
	{
		m_server.close();

		emit signalServerStateChange(QString::fromLocal8Bit("断开"));
	}

	if (m_zgbCtrler.IsOpened())
	{
		m_zgbCtrler.Close();
	}

	m_thread.quit();
	m_thread.wait();
	m_timer.stop();
	m_clock.stop();

	UpdateAGVConnection();
	UpdateAGVStatus();

	if (m_led == nullptr || m_led->IsConnected() == false)
	{
		return;
	}

	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(tp);
	tm* t = localtime(&tt);

	QString wday;

	switch (t->tm_wday)
	{
	case 1:
		wday = QString::QString::fromLocal8Bit("一");
		break;
	case 2:
		wday = QString::QString::fromLocal8Bit("二");
		break;
	case 3:
		wday = QString::QString::fromLocal8Bit("三");
		break;
	case 4:
		wday = QString::QString::fromLocal8Bit("四");
		break;
	case 5:
		wday = QString::QString::fromLocal8Bit("五");
		break;
	case 6:
		wday = QString::QString::fromLocal8Bit("六");
		break;
	case 0:
		wday = QString::QString::fromLocal8Bit("日");
		break;
	}

	m_led->SetText(98, QString::fromLocal8Bit("   %1年%2月%3日 星期%4")
		.arg(t->tm_year + 1900, 4, 10, QLatin1Char(' '))
		.arg(t->tm_mon + 1, 2, 10, QLatin1Char(' '))
		.arg(t->tm_mday, 2, 10, QLatin1Char(' '))
		.arg(wday));

	return;
}

void Service::Initialize(const QJsonDocument& doc)
{
	// 读取服务器配置
	LoadServerConfig(doc);

	// 读取Zigbee控制器配置
	LoadZgbeCtrlConfig(doc);

	// 读取LED配置
	LoadLedConfig(doc);

	if (m_dbAGV.databaseName().isNull() == false && m_dbAGV.databaseName().isEmpty() == false)
	{
		return;
	}

	LoadAGVDBConfig(doc);

	return;
}

bool Service::LoadConfig()
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load config failed:The database is not opened" << endl;
		return false;
	}

	// 读取RFID配置
	if (LoadRFIDConfig() == false)
	{
		qDebug() << "Open service failed:Load RFID config failed" << endl;
		return false;
	}

	// 读取工作站配置
	if (LoadWorkConfig() == false)
	{
		qDebug() << "Open service failed:Load work station config failed" << endl;
		return false;
	}

	// 读取AGV类型配置
	if (LoadAGVTypeConfig() == false)
	{
		qDebug() << "Open service failed:Load AGV type config failed" << endl;
		return false;
	}

	// 读取AGV配置
	if (LoadAGVConfig() == false)
	{
		qDebug() << "Open service failed:Load AGV config failed" << endl;
		return false;
	}

	// 读取待机站配置
	LoadRestConfig();

	// 读取充电站配置
	LoadChargeConfig();

	// 读取呼叫器配置
	LoadZigbeeCaller();

	// 读取待机队列
	LoadRestQueueConfig();

	return true;
}

bool Service::LoadConfig(const QJsonDocument& doc)
{
	// 读取RFID配置
	if (LoadRFIDConfig(doc) == false)
	{
		qDebug() << "Open service failed:Load RFID config failed" << endl;
		return false;
	}

	// 读取工作站配置
	if (LoadWorkConfig(doc) == false)
	{
		qDebug() << "Open service failed:Load work station config failed" << endl;
		return false;
	}

	// 读取AGV类型配置
	if (LoadAGVTypeConfig(doc) == false)
	{
		qDebug() << "Open service failed:Load AGV type config failed" << endl;
		return false;
	}

	// 读取AGV配置
	if (LoadAGVConfig(doc) == false)
	{
		qDebug() << "Open service failed:Load AGV config failed" << endl;
		return false;
	}

	// 读取待机站配置
	LoadRestConfig(doc);

	// 读取充电站配置
	LoadChargeConfig(doc);

	// 读取呼叫器配置
	LoadZigbeeCaller(doc);

	// 读取待机队列
	LoadRestQueueConfig(doc);

	return true;
}

bool Service::Open()
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Open service failed:The database is not opened" << endl;
		return false;
	}

	if (m_led)
	{
		m_led->OpenScreen();
	}

	BindRestQueue();
	LoadAGVStatus();
	LoadCallerSignal();

	m_timer.start();

	if (m_led == nullptr || m_led->IsConnected() == false)
	{
		return true;
	}

	m_led->SetScreenLight(4);

	//m_led.CloseScreen();

	m_led->SetText(0, QString::fromLocal8Bit("          欢迎莅临"));
	m_led->SetText(1, QString::fromLocal8Bit("   青岛三星精锻齿轮有限公司"));
	m_led->SetText(2, QString::fromLocal8Bit("---------------------------"));
	m_led->SetText(3, QString::fromLocal8Bit("AGV实时动态:"));

	if (m_led->GetValue(10).isNull() || m_led->GetValue(10).isEmpty())
	{
		m_led->SetText(10, QString::fromLocal8Bit(""));
	}

	if (m_led->GetValue(11).isNull() || m_led->GetValue(11).isEmpty())
	{
		m_led->SetText(11, QString::fromLocal8Bit(""));
	}
	m_led->SetText(97, QString::fromLocal8Bit("---------------------------"));
	slotUpdateClock();
	m_led->SetText(99, QString::fromLocal8Bit("      哈尔滨博乐恩承制"));

	m_clock.start();

	return true;
}

bool Service::ConnectAgv(QTcpSocket* socket)
{
	for (AgvArray::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		if (it->second->Connect(socket))
		{
			return true;
		}
	}

	return false;
}

bool Service::IsLocked(const LocatArray& list)
{
	for (LocatArray::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		RFID lock = GetRFID(*it);	/*!< 锁定点 */

		if (lock == false)
		{
			continue;
		}

		if (lock.GetLocker())
		{
			return true;
		}
	}

	return false;
}

void Service::ScreamControl()
{
	for (AgvArray::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		if (m_bAllScream)
		{
			it->second->AllScream();
		}
		else if (it->second->GetAttribute().GetStatus() == AgvStatus::Sta_AllScream)
		{
			it->second->Reset();
		}
	}

	return;
}

void Service::TrafficControl()
{
	for (TrafficArray::const_iterator it = m_mapTraffics.begin(); it != m_mapTraffics.end(); ++it)
	{
		TrafficControl(it->second);
	}

	return;
}

void Service::TrafficControl(const rfid_t& id)
{
	return TrafficControl(GetTrafficPosition(id));
}

void Service::TrafficControl(const RFID& rfid)
{
	return TrafficControl(GetTrafficPosition(rfid.GetId()));
}

void Service::TrafficControl(const TrafficPos& pos)
{
	if (pos.IsNull())
	{
		return;
	}

	RFID ctrl = GetRFID(pos.GetCtrlPos());	/*!< 管制点 */

	if (ctrl == false)
	{
		return;
	}

	Agv* agv = static_cast<Agv*>(const_cast<void*>(ctrl.GetLocker()));

	if (agv == nullptr)
	{
		return;
	}

	if (IsLocked(pos.GetLockList()))
	{
		agv->TrafficStop();
		return;
	}

	LocatArray arr = pos.GetCompareList();
	if (TrafficCompare(ctrl, arr))
	{
		agv->TrafficPass();
	}
	else
	{
		agv->TrafficStop();
	}

	return;
}

bool Service::TrafficCompare(const rfid_t& ctrl, LocatArray& list)
{
	return TrafficCompare(GetRFID(ctrl), list);
}

bool Service::TrafficCompare(const RFID& ctrl, LocatArray& list)
{
	if (ctrl == false)
	{
		return false;
	}

	if (list.size() == 0)
	{
		return true;
	}

	for (LocatArray::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		TrafficPos pos = GetTrafficPosition(*it);

		if (pos.IsNull())
		{
			continue;
		}

		if (IsLocked(pos.GetLockList()))
		{
			return false;
		}

		RFID compare = GetRFID(pos.GetCtrlPos());

		if (compare == false)
		{
			continue;
		}

		if (!(ctrl < compare))
		{
			return false;
		}
	}

	return true;
}

void Service::RestStationControl()
{
	for (ResterArray::const_iterator it = m_mapResters.begin(); it != m_mapResters.end(); ++it)
	{
		RestStationControl(*it->second);
	}

	return;
}

void Service::RestStationControl(const RestStation& rester)
{
	if (rester.GetLockSize() == 0)
	{
		return;
	}

	RFIDPtr	locat = GetRFIDPtr(rester.GetLocation());

	if (locat == nullptr)
	{
		return;
	}

	Agv* locker = reinterpret_cast<Agv*>(const_cast<void*>(locat->GetLocker()));

	if (locker)
	{
		return;
	}

	RFIDPtr rfid = GetRFIDPtr(rester.GetFirstLock());

	while (rfid != nullptr)
	{
		locker = reinterpret_cast<Agv*>(const_cast<void*>(rfid->GetLocker()));

		if (locker)
		{
			locker->Move(locat->GetId());

			break;
		}

		rester.GetNextLock(rfid->GetId());
	}

	return;
}

void Service::RestQueueControl()
{
	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		RestQueueControl(it->second);
	}

	return;
}

void Service::RestQueueControl(const RestQueue& queue)
{
	if (queue.IsNull())
	{
		return;
	}

	for (unsigned short id = queue.GetTail(); id != 0; id = queue.GetPrevStation(id))
	{
		RestQueueControl(id, queue.GetPrevStation(id));
	}

	return;
}

void Service::RestQueueControl(const unsigned short& cur, const unsigned short& prev)
{
	return RestQueueControl(GetRestStation(cur), GetRestStation(prev));
}

void Service::RestQueueControl(const RestStation& cur, const RestStation& prev)
{
	if (cur.IsNull())
	{
		// 无效的待机站
		return;
	}

	RFID rfid = GetRFID(cur.GetLocation());

	if (rfid == false)
	{
		// 无效的坐标
		return;
	}

	Agv* agv = static_cast<Agv*>(const_cast<void*>(rfid.GetLocker()));	/*!< AGV指针 */

	if (agv == nullptr)
	{
		// 无AGV在待机站
		return;
	}

	if (prev.IsNull())
	{
		// 无效的待机站
		return;
	}

	rfid = GetRFID(prev.GetLocation());

	if (rfid == false)
	{
		// 无效的坐标
		return;
	}

	if (rfid.GetLocker())
	{
		// 存在AGV在待机站
		return;
	}

	if (IsLocked(prev.GetLockArea()))
	{
		// 待机站上锁
		return;
	}

	// 移动至待机站
	agv->Move(prev.GetLocation());

	return;
}

void Service::ReturnControl(Agv& agv)
{
	/*
	 * 电量不足优先返回充电站
	 * 具有待机队列的情况下优先返回待机队列
	 */
	AgvBattery battery = agv.GetAttribute().GetBattery();

	if (battery <= AgvBattery::Power_Low && battery > AgvBattery::Power_Off)
	{
		if (ReturnToChargeStation(agv))
		{
			return;
		}

		if (m_mapRestQueues.size() > 0)
		{
			ReturnToRestQueue(agv);
		}
		else
		{
			ReturnToRestStation(agv);
		}
	}
	else
	{

		if (m_mapRestQueues.size() > 0)
		{
			if (ReturnToRestQueue(agv))
			{
				return;
			}
		}
		else
		{
			if (ReturnToRestStation(agv))
			{
				return;
			}
		}

		ReturnToChargeStation(agv);
	}

	return;
}

bool Service::ReturnToRestQueue(Agv& agv)
{
	if (IsInRestStation(agv.GetAttribute().GetCurLocation()))
	{
		return true;
	}

	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		RestStation restSta = restStation(it->second.GetTail());

		if (restSta.IsNull())
		{
			continue;
		}

		agv.Move(restSta.GetLocation());

		return true;
	}

	return false;
}

bool Service::ReturnToRestStation(Agv& agv)
{
	if (IsInRestStation(agv.GetAttribute().GetCurLocation()))
	{
		return true;
	}

	for (ResterArray::const_iterator it = m_mapResters.begin(); it != m_mapResters.end(); ++it)
	{
		ResterPtr staPtr = it->second;

		if (staPtr->IsNull())
		{
			continue;
		}

		RFID locat = GetRFID(staPtr->GetLocation());

		if (locat == false)
		{
			continue;
		}

		const void* locker = locat.GetLocker();

		if (locker != nullptr)
		{
			if (locker != &agv)
			{
				continue;
			}

			return true;
		}

		const void* peerPocker = locat.GetPeerLocker();

		if (peerPocker != nullptr && peerPocker != &agv)
		{
			continue;
		}

		agv.Move(locat.GetId());

		return true;
	}

	return false;
}

bool Service::ReturnToChargeStation(Agv& agv)
{
	if (IsInChargeStation(agv.GetAttribute().GetCurLocation()))
	{
		return true;
	}

	for (ChargerArray::const_iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		shared_ptr < ChargeStation > staPtr = it->second;

		if (staPtr->IsNull())
		{
			continue;
		}

		RFID locat = GetRFID(staPtr->GetLocation());

		if (locat == false)
		{
			continue;
		}

		const void* locker = locat.GetLocker();

		if (locker != nullptr)
		{
			if (locker != &agv)
			{
				continue;
			}

			return true;
		}

		const void* peerPocker = locat.GetPeerLocker();

		if (peerPocker != nullptr && peerPocker != &agv)
		{
			continue;
		}

		agv.Move(locat.GetId());

		return true;
	}

	return false;
}

void Service::UpdateCallerSignal(const ZigbeeCaller& caller)
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update caller:" << caller.GetId() << " signal failed:The database is not open" << endl;

		return;
	}

	const CallerAttr attr = caller.GetAttribute();

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_CALLER", sql;

	sql = QString::fromLocal8Bit("update %1 set [caller_call]=%2,[caller_call_info]='%3',\
[caller_cancel]=%4,[caller_cancel_info]='%5',\
[caller_scream]=%6,[caller_scream_info]='%7' where [caller_id]=%8")
.arg(table)
.arg(attr.m_sigCall)
.arg(attr.GetSignalInfo(attr.GetCallSignal()))
.arg(attr.m_sigCancel)
.arg(attr.GetSignalInfo(attr.GetCancelSignal()))
.arg(attr.m_sigScream)
.arg(attr.GetSignalInfo(attr.GetScreamSignal()))
.arg(caller.GetId());

	if (query.exec(sql) == false)
	{
		qDebug() << "Update caller" << caller.GetId() << " signal failed:" << query.lastError().text() << endl;

		return;
	}

	ServiceBase::UpdateCallerSignal(caller.GetId(), attr);

	return;
}

void Service::UpdateCallerUse(const ZigbeeCaller& caller)
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update caller:" << caller.GetId() << " enable use failed:The database is not open" << endl;

		return;
	}

	const CallerAttr attr = caller.GetAttribute();

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_CALLER", sql;

	sql = QString::fromLocal8Bit("update %1 set [caller_enable]=%2 where [caller_id]=%3").arg(table).arg(caller.IsEnabled()).arg(caller.GetId());

	if (query.exec(sql) == false)
	{
		qDebug() << "Update caller" << caller.GetId() << " enable failed:" << query.lastError().text() << endl;

		return;
	}

	emit signalUpdateCallerData();

	return;
}

void Service::PublishOrder(const ZgbCaller& caller)
{
	Signal sig = caller.GetAttribute().GetCallSignal();

	if (sig == Signal::Signal_Off)
	{
		return;
	}

	QString log;

	WorkerPtr pLoad = nullptr, pUnload = nullptr;

	switch (sig)
	{
	case Signal::Signal_On:
		// 不唯一
		// 至XXX取回空托盘
		pLoad = workStationPtr(caller.GetStation());
		pUnload = GetWorkStationPtr(TASK_DEFAULT_TRAYLIB);

		if (pLoad == false)
		{
			return;
		}

		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_GETTRAY);

		break;
	case Signal::Signal_ToStation:
		// 不唯一
		// 至XXX取成品入站
		pLoad = workStationPtr(caller.GetStation());
		pUnload = GetWorkStationPtr(TASK_DEFAULT_PRODUCTSTA);

		if (pLoad == false)
		{
			return;
		}

		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_TOSTATION);
		break;
	case Signal::Signal_ToLib:
		// 不唯一
		// 至XXX取成品入库
		pLoad = workStationPtr(caller.GetStation());
		pUnload = GetWorkStationPtr(TASK_DEFAULT_PRODUCTLIB);

		if (pLoad == false)
		{
			return;
		}

		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_TOLIB);
		break;
	case Signal::Signal_PutBlank1:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_BLANKLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STAHEAD1);

		log = TASK_PUTBLANK1;
		break;
	case Signal::Signal_PutBlank2:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_BLANKLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STAHEAD2);

		log = TASK_PUTBLANK2;
		break;
	case Signal::Signal_PutBlank3:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_BLANKLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STAHEAD3);

		log = TASK_PUTBLANK3;
		break;
	case Signal::Signal_PutTray1:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_TRAYLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STATAIL1);

		log = TASK_PUTTRAY1;
		break;
	case Signal::Signal_PutTray2:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_TRAYLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STATAIL2);

		log = TASK_PUTTRAY2;
		break;
	case Signal::Signal_PutTray3:
		// 唯一
		pLoad = GetWorkStationPtr(TASK_DEFAULT_TRAYLIB);
		pUnload = GetWorkStationPtr(TASK_DEFAULT_STATAIL3);

		log = TASK_PUTTRAY3;
		break;
	default:
		m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Call);
		return;
	}

	return PublishOrder(pLoad, pUnload, log);
}

void Service::PublishOrder(const WorkerPtr& pLoad, const WorkerPtr& pUnload, const QString& log)
{
	if (pLoad == nullptr || pUnload == nullptr)
	{
		qDebug() << "Publish order failed:The station is null" << endl;
		return;
	}

	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("select * from %1 where [task_log] = '%2'").arg(table).arg(log);

	if (query.exec(sql) == false)
	{
		qDebug() << "Publish order failed:" << query.lastError().text() << endl;

		return;
	}

	if (query.next())
	{
		return;
	}

	QString order = CreateOrderNumber();

	if (PublishTask(order, pLoad->GetName(), TASK_MISSION_LOAD, log) && PublishTask(order, pUnload->GetName(), TASK_MISSION_UNLOAD, log) && PublishTask(order, QString::fromLocal8Bit("无"), TASK_MISSION_FINISH, log))
	{
		return;
	}

	return;
}

bool Service::PublishTask(const QString& order, const QString& target, const QString& mission, const QString& log)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("insert into %1([task_order],[task_target],[task_mission],[task_log]) values('%2','%3','%4','%5')")
		.arg(table)
		.arg(order)
		.arg(target)
		.arg(mission)
		.arg(log);

	if (query.exec(sql) == false)
	{
		qDebug() << "Publish task failed:" << query.lastError().text() << endl;
		return false;
	}

	return true;
}

bool Service::UpdateOrder(const QString& order, const QString& tray, const int& agv)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("update %1 set [task_tray]='%2',[task_executer]=%3 where [task_order]='%4'")
		.arg(table)
		.arg(tray)
		.arg(agv)
		.arg(order);

	if (query.exec(sql) == false)
	{
		qDebug() << "Update order failed:" << query.lastError().text() << endl;
		return false;
	}

	return true;
}

bool Service::UpdateTask(const int& no, const QString& status)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");

	QString param;

	if (status == TASK_STA_FINISH)
	{
		param = "task_finish";
	}
	else if (status == TASK_STA_START)
	{
		param = "task_execution";
	}

	QString sql = QString::fromLocal8Bit("update %1 set [task_status]='%2',[%3]=getdate() where [task_id] = %4")
		.arg(table)
		.arg(status)
		.arg(param)
		.arg(no);

	if (query.exec(sql) == false)
	{
		qDebug() << "Update task failed:" << query.lastError().text() << endl;
		return false;
	}

	return true;
}

bool Service::CancelOrder(const QString& order)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("update %1 set [task_status]='%2' where [task_order] = '%3'")
		.arg(table)
		.arg(TASK_STA_CANCEL)
		.arg(order);

	if (query.exec(sql) == false)
	{
		qDebug() << "Cancel order failed:" << query.lastError().text() << endl;
		return false;
	}

	return true;
}

bool Service::CancelOrder(const ZgbCaller& caller)
{
	if (caller.GetAttribute().GetCancelSignal() != Signal::Signal_On)
	{
		return true;
	}

	Signal sig = caller.GetAttribute().GetCallSignal();

	if (sig == Signal::Signal_Off)
	{
		m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Cancel);
		return true;
	}

	WorkerPtr pLoad = workStationPtr(caller.GetStation());

	if (pLoad == nullptr)
	{
		m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Cancel);
		return true;
	}

	QString log;
	switch (sig)
	{
	case Signal::Signal_On:
		// 不唯一
		// 至XXX取回空托盘
		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_GETTRAY);
		break;
	case Signal::Signal_ToStation:
		// 不唯一
		// 至XXX取成品入站
		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_TOSTATION);
		break;
	case Signal::Signal_ToLib:
		// 至XXX取成品入库
		// 不唯一
		log = QString::fromLocal8Bit("至%1%2").arg(pLoad->GetName()).arg(TASK_TOLIB);
		break;
	case Signal::Signal_PutBlank1:
		// 唯一
		log = TASK_PUTBLANK1;
		break;
	case Signal::Signal_PutBlank2:
		// 唯一
		log = TASK_PUTBLANK2;
		break;
	case Signal::Signal_PutBlank3:
		// 唯一
		log = TASK_PUTBLANK3;
		break;
	case Signal::Signal_PutTray1:
		// 唯一
		log = TASK_PUTTRAY1;
		break;
	case Signal::Signal_PutTray2:
		// 唯一
		log = TASK_PUTTRAY2;
		break;
	case Signal::Signal_PutTray3:
		// 唯一
		log = TASK_PUTTRAY3;
		break;
	default:
		m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Cancel);
		return true;
	}

	QString order = GetOrderNumber(log);

	if (order.isNull() || order.isEmpty())
	{
		m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Call);
		return false;
	}

	if (CancelOrder(order) == false)
	{
		return false;
	}

	m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Call);

	return true;
}

QString Service::GetOrderNumber(const QString& log)
{
	if (log.isNull() || log.isEmpty())
	{
		return QString();
	}

	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("select [task_order] from %1 where task_log='%2'").arg(table).arg(log);

	if (query.exec(sql) == false)
	{
		qDebug() << "Get order number:" << query.lastError().text() << endl;
		return QString();
	}

	return query.value("task_order").toString();
}

QString Service::GetOrderNumber(const int& id)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");
	QString sql = QString::fromLocal8Bit("select [task_order] from %1 where task_id='%1'").arg(table).arg(id);

	if (query.exec(sql) == false)
	{
		qDebug() << "Get order number:" << query.lastError().text() << endl;
		return QString();
	}

	return query.value("task_order").toString();
}

bool Service::ExecuteOrder(Order& order)
{
	QString orderNo = order.GetNo();

	if (orderNo.isNull()
		|| orderNo.isEmpty())
	{
		return false;
	}

	if (order.GetExecuter() == 0)
	{
		// 未分配的订单
		return false;
	}

	// 为订单任务排序
	order.Sort();

	while (order.Next())
	{
		if (ExecuteTask(order.GetTask()) == false)
		{
			// 未执行完任务
			return false;
		}

		// 任务完成后执行下一任务
	}

	// 订单里的任务全部完成

	return true;
}

bool Service::ExecuteTask(const TaskBase& task)
{
	AgvPtr agv = task.m_executer;	/*!< AGV指针 */

	QString error;

	if (agv == nullptr)
	{
		qDebug() << "Execute task falied:Can not find AGV" << endl;

		error = QString::fromLocal8Bit("执行任务失败:无法找到AGV");

		// 抛出异常
		ThrowError(task.m_task, error);

		return false;
	}

	if (task.m_mission == TASK_MISSION_FINISH)
	{
		return UpdateTask(task.m_task, TASK_STA_FINISH);
	}

	WorkerPtr worker = task.m_target; /*!< 工作站指针 */

	if (worker == nullptr)
	{
		qDebug() << "Execute task falied:Can not find work station" << endl;

		error = QString::fromLocal8Bit("执行任务失败:无法找到工作站");

		// 抛出异常
		ThrowError(task, error);

		return false;
	}

	if (task.m_status == TASK_STA_DEFAULT)
	{
		UpdateTask(task.m_task, TASK_STA_START);

		return false;
	}

	rfid_t target = worker->GetLocation();	/*!< 任务目标RFID地标卡 */
	rfid_t currnet = agv->GetAttribute().GetCurLocation();	/*!< 当前坐标 */

	if (currnet != target)
	{
		agv->Move(target);

		ClearError(task);

		return false;
	}

	CallerPtr caller = m_zgbCtrler.GetCallerPtr(worker->GetId());

	if (task.m_mission == TASK_MISSION_WAIT)
	{
		ClearError(task);

		if (caller)
		{
			m_zgbCtrler.clear(*caller, SignalType::Signal_Call);
		}

		return false;
	}

	QString curAct = agv->GetActionName();
	AgvExecution cueExe = agv->GetAttribute().GetExecution(); /*!< AGV动作执行状态 */

	QString strAct;

	bool bLoad = false;

	if (task.m_mission == TASK_MISSION_LOAD)
	{
		strAct = TASK_MISSION_LOAD;

		bLoad = true;
	}
	else if (task.m_mission == TASK_MISSION_UNLOAD)
	{
		strAct = TASK_MISSION_UNLOAD;
	}

	if (curAct != strAct)
	{
		agv->Action(strAct);

		return false;
	}

	switch (cueExe)
	{
	case AgvExecution::Act_None:
		agv->Action(strAct);

		ClearError(task);

		return false;
	case AgvExecution::Act_Exe:
		if (agv->GetAttribute().GetActionExecuteTime() > 30 * 1000)
		{
			error = QString::fromLocal8Bit("执行任务异常:AGV动作超时");

			// 抛出异常
			ThrowError(task, error);
		}
		else
		{
			ClearError(task);
		}
		return false;
	case AgvExecution::Act_Fin:

		ClearError(task);

		if (bLoad && caller && caller->GetAttribute().GetCallSignal() != Signal::Signal_Off)
		{
			m_zgbCtrler.ClearSignal(*caller, SignalType::Signal_Call);

			return false;
		}

		return UpdateTask(task.m_task, TASK_STA_FINISH);
	}

	return false;
}

void Service::ThrowError(const int& no, const QString& error) const
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");

	QString sql = QString::fromLocal8Bit("update %1 set [task_error]='%2' where [task_id] = %3")
		.arg(table)
		.arg(error)
		.arg(no);

	if (query.exec(sql) == false)
	{
		qDebug() << "Throw task error failed:" << query.lastError().text() << endl;
		return;
	}

	QString source = QString::fromLocal8Bit("任务%1").arg(no);

	if (error == TASK_ERR_DEFAULT)
	{
		ServiceBase::ClearError(source, 1);
	}
	else
	{
		ServiceBase::ThrowError(source, error, 1);
	}

	return;
}

void Service::ThrowError(const TaskBase& task, const QString& error) const
{
	if (task.m_error == error)
	{
		return;
	}

	return ThrowError(task.m_task, error);
}

void Service::ClearError(const int& id) const
{
	return ThrowError(id, TASK_ERR_DEFAULT);
}

void Service::ClearError(const TaskBase& task) const
{
	if (task.m_error == TASK_ERR_DEFAULT)
	{
		return;
	}

	return ClearError(task.m_task);
}

OrderMap Service::GetOrder()
{
	OrderMap _map;

	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");

	QString sql = QString::fromLocal8Bit("select * from %1 order by [task_publish],[task_executer] DESC,[task_id]")
		.arg(table);

	if (query.exec(sql) == false)
	{
		qDebug() << "Get order failed:" << query.lastError().text() << endl;
		return _map;
	}

	while (query.next())
	{
		TaskBase task;

		task.m_order = query.value("task_order").toString();
		task.m_task = query.value("task_id").toInt();
		task.m_tray = query.value("task_tray").toString();
		task.m_target = GetWorkStationPtr(query.value("task_target").toString());
		task.m_mission = query.value("task_mission").toString();
		task.m_executer = GetAGVPtr(query.value("task_executer").toInt());
		task.m_status = query.value("task_status").toString();
		task.m_error = query.value("task_error").toString();
		task.m_log = query.value("task_log").toString();

		if (_map.find(task.m_order) == _map.end())
		{
			_map[task.m_order] = Order(task.m_order, task.m_tray, task.m_executer);
		}
		else
		{
			if (task.m_executer == nullptr)
			{
				task.m_executer = _map[task.m_order].GetExecuter();
			}
		}

		_map[task.m_order].Insert(task);
	}

	return _map;
}

Order Service::GetOrder(const QString& no)
{
	QSqlQuery query(m_dbAGV);

	QString table("AGVDB_TASK_CURRENT");

	QString sql = QString::fromLocal8Bit("select * from %1 where [task_order] = '%2'order by [task_publish],[task_id]")
		.arg(table)
		.arg(no);

	if (query.exec(sql) == false)
	{
		qDebug() << "Get order" << no << " failed:" << query.lastError().text() << endl;
		return Order();
	}

	Order order(no);

	int count = 0;

	while (query.next())
	{
		TaskBase task;

		task.m_order = no;
		task.m_task = query.value("task_id").toInt();
		task.m_tray = query.value("task_tray").toString();
		task.m_target = GetWorkStationPtr(query.value("task_target").toString());
		task.m_mission = query.value("task_mission").toString();
		task.m_executer = GetAGVPtr(query.value("task_executer").toInt());
		task.m_status = query.value("task_status").toString();
		task.m_error = query.value("task_error").toString();
		task.m_log = query.value("task_log").toString();

		if (count == 0)
		{
			order.SetTray(task.m_tray);
			order.SetExecuter(task.m_executer);
		}

		if (task.m_executer == nullptr)
		{
			task.m_executer = order.GetExecuter();
		}

		order.Insert(task);

		++count;
	}

	return order;
}

bool Service::HaveOrder(const OrderMap& _map, const int& id)
{
	for (OrderMap::const_iterator it = _map.begin(); it != _map.end(); ++it)
	{
		AgvPtr ptr = it->second.GetExecuter();

		if (ptr == nullptr)
		{
			continue;
		}

		if (ptr->GetId() == id)
		{
			return true;
		}
	}

	return false;
}

void Service::OrderManage()
{
	OrderMap list = GetOrder();	/*!< 订单列表 */

	// 可用于分配的AGV列表
	std::list<int> agvList = GetFreeAGVList(list);

	if (list.size() == 0)
	{
		// 没有订单时,所有AGV应返回待机站
		for (std::list<int>::const_iterator it = agvList.begin(); it != agvList.end(); ++it)
		{
			AgvPtr ptr = GetAGVPtr(*it);

			if (ptr == nullptr)
			{
				continue;
			}

			ReturnControl(*ptr);
		}
		return;
	}

	for (OrderMap::iterator it = list.begin(); it != list.end(); ++it)
	{//执行订单
		if (it->second.GetExecuter() == nullptr)
		{// 未分配订单
			if (agvList.size() == 0)
			{
				continue;
			}

			if (it->second.IsFull() == false)
			{
				continue;
			}

			// 分配订单
			if (UpdateOrder(it->second.GetNo(), it->second.GetTray(), agvList.front()))
			{
				agvList.pop_front();
			}

			continue;
		}

		if (ExecuteOrder(it->second) == false)
		{
			continue;
		}

		AgvPtr ptr = it->second.GetExecuter();
		AgvBattery power = ptr->GetAttribute().GetBattery();
		if (power > AgvBattery::Power_Off&& power < AgvBattery::Power_Low)
		{// 电量不足
			ReturnControl(*ptr);

			continue;
		}

		if (m_mapRestQueues.size() == 0)
		{
			// 将AGV返回可分配的AGV列表中
			agvList.push_front(ptr->GetId());
		}
	}

	// 剩余未分配的AGV应返回待机站
	for (std::list<int>::const_iterator it = agvList.begin(); it != agvList.end(); ++it)
	{
		AgvPtr ptr = GetAGVPtr(*it);

		if (ptr == nullptr)
		{
			continue;
		}

		ReturnControl(*ptr);
	}
	return;
}

std::list<int> Service::GetFreeAGVList(const OrderMap& _map)
{
	std::list<int> list;	/*!< 可分配订单的AGV列表 */

	for (AgvArray::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		AgvAttr attr = it->second->GetAttribute();		/*!< AGV属性 */
		AgvMode mode = attr.GetMode();	/*!< AGV模式*/

		if (mode == AgvMode::Mode_Hand)
		{// AGV为手动模式不可以分配订单
			continue;
		}

		if (HaveOrder(_map, it->second->GetId()))
		{// AGV已经有订单,不可以再分配订单
			continue;
		}

		rfid_t cur = attr.GetCurLocation();		/*!< AGV当前RFID地标卡编号*/

		bool head = false;	/*!< 待机站首地址标识 */

		if (IsInRestQueue(cur))
		{// 如果存在待机队列
			if (IsRestQueueHead(cur) == false)
			{// AGV不在队列的首位，不可以再分配订单
				continue;
			}

			head = true;
		}

		AgvBattery power = attr.GetBattery();	/*!< AGV电量 */

		if (power == AgvBattery::Power_Off)
		{// AGV未连接，不可分配订单
			continue;
		}
		else if (power < AgvBattery::Power_Low)
		{// 电量不足，不可分配订单
			ReturnControl(*it->second);
		}

		if (m_mapRestQueues.size() > 0 && head == false)
		{// 非待机首位的AGV，返回待机队列等待订单分配
			ReturnControl(*it->second);

			continue;
		}

		// 存入可分配订单列表
		list.push_back(it->second->GetId());
	}

	return list;
}

bool Service::IsInRestQueue(const rfid_t& rfid) const
{
	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		ResterPtr ptr = restStationPtr(it->second.GetHead());

		while (ptr)
		{
			if (ptr->IsInside(rfid))
			{
				return true;
			}

			ptr = restStationPtr(it->second.GetNextStation(ptr->GetId()));
		}
	}

	return false;
}

bool Service::IsRestQueueHead(const rfid_t& rfid) const
{
	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		ResterPtr ptr = restStationPtr(it->second.GetHead());

		if (ptr == nullptr)
		{
			continue;
		}

		if (ptr->IsInside(rfid))
		{
			return true;
		}
	}

	return false;
}

bool Service::IsInRestStation(const rfid_t& rfid) const
{
	for (ResterArray::const_iterator it = m_mapResters.begin(); it != m_mapResters.end(); ++it)
	{
		if (it->second->IsInside(rfid))
		{
			return true;
		}
	}

	return false;
}

bool Service::IsInChargeStation(const rfid_t& rfid) const
{
	for (ChargerArray::const_iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		if (it->second->GetLocation() == rfid)
		{
			return true;
		}
	}

	return false;
}

QString Service::GetTargetName(const rfid_t& rfid) const
{
	if (rfid == 0)
	{
		return "";
	}

	WorkerPtr ptrW = GetWorkStationPtr(rfid);

	if (ptrW)
	{
		return ptrW->GetName();
	}

	ResterPtr ptrR = GetRestStationPtr(rfid);

	if (ptrR)
	{
		return ptrR->GetName();
	}

	ChargerPtr ptrC = GetChargeStationPtr(rfid);

	if (ptrC)
	{
		return ptrC->GetName();
	}

	return QString::fromLocal8Bit("地标卡%1").arg(rfid);
}

CmdError Service::AgvControl(const id_t& id, const AgvCtrlType& type, const rfid_t& rfid, const QString& action, const AgvAttr::speed_t& speed)
{
	AgvPtr ptr = GetAGVPtr(id);

	if (ptr == nullptr)
	{
		return CmdError::Cmd_ParamError;
	}

	switch (type)
	{
	case AgvCtrlType::Ctrl_Action:
		return ptr->Action(action);
	case AgvCtrlType::Ctrl_Move:
		return ptr->Move(rfid);
	case AgvCtrlType::Ctrl_Speed:
		return ptr->SpeedControl(speed);
	case AgvCtrlType::Ctrl_Continue:
		return ptr->Continue();
	case AgvCtrlType::Ctrl_Pause:
		return ptr->Pause();
	case AgvCtrlType::Ctrl_Reset:
		return ptr->Reset();
	case AgvCtrlType::Ctrl_Restart:
		return ptr->Restart();
	case AgvCtrlType::Ctrl_RmtScream:
		return ptr->RemoteScream();
	case AgvCtrlType::Ctrl_Shutdonw:
		return ptr->Shutdown();
	case AgvCtrlType::Ctrl_Sleep:
		return ptr->Sleep();
	case AgvCtrlType::Ctrl_WakeUp:
		return ptr->WakeUp();
	}

	return CmdError::Cmd_ParamError;
}

void Service::slotNewConnection()
{
	while (m_server.hasPendingConnections())
	{
		QTcpSocket* socket = m_server.nextPendingConnection();

		if (ConnectAgv(socket))
		{
			return;
		}

		//socket->close();
		//socket->deleteLater();
	}

	return;
}

void Service::slotServerError(QAbstractSocket::SocketError error)
{
	emit signalServerStateChange(QString::fromLocal8Bit("服务异常:%1").arg(m_server.errorString()));

	qDebug() << "Server error," << m_server.errorString() << endl;

	switch (error)
	{
	case QAbstractSocket::ConnectionRefusedError:
		// The connection was refused by the peer (or timed out).
		// 连接被对等方拒绝(或超时)。
		break;
	case QAbstractSocket::RemoteHostClosedError:
		// The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.
		// 远程主机关闭了连接.请注意客户端套接字即将在发送远程关闭通知后关闭
		break;
	case QAbstractSocket::HostNotFoundError:
		// The host address was not found.
		// 没有找到主机地址
		break;
	case QAbstractSocket::SocketAccessError:
		// The socket operation failed because the application lacked the required privileges.
		// 由于应用程序缺少所需的特权,套接字操作失败
		break;
	case QAbstractSocket::SocketResourceError:
		// The local system ran out of resources (e.g., too many sockets).
		// 本地系统耗尽了资源(例如,太多的套接字)
		break;
	case QAbstractSocket::SocketTimeoutError:
		// The socket operation timed out.
		// 套接字操作超时
		break;
	case QAbstractSocket::DatagramTooLargeError:
		// The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).
		// 数据报大于操作系统的限制(可能低至8192字节)
		break;
	case QAbstractSocket::NetworkError:
		// An error occurred with the network (e.g., the network cable was accidentally plugged out).
		// 网络出现错误(例如,网络电缆意外地被拔掉)
		break;
	case QAbstractSocket::AddressInUseError:
		// The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.
		// 指定给QAbstractSocket::bind()的地址已经被使用，并且被设置为exclusive。
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
		// The address specified to QAbstractSocket::bind() does not belong to the host.
		// 指定给QAbstractSocket::bind()的地址不属于主机。
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		// The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support)
		// 指定给QAbstractSocket::bind()的地址不属于主机。本地操作系统不支持请求的套接字操作(例如，缺乏对IPv6的支持)
		break;
	case QAbstractSocket::UnfinishedSocketOperationError:
		// Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).
		// 仅用于QAbstractSocketEngine，最后尝试的操作尚未完成(在后台仍在进行中)。
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		// The socket is using a proxy, and the proxy requires authentication.
		// 套接字使用代理，代理需要身份验证。
		break;
	case QAbstractSocket::SslHandshakeFailedError:
		// The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)
		// SSL/TLS握手失败，因此连接被关闭(仅在QSslSocket中使用)
		break;
	case QAbstractSocket::ProxyConnectionRefusedError:
		// 	Could not contact the proxy server because the connection to that server was denied
		// 无法联系代理服务器，因为连接到该服务器被拒绝
		break;
	case QAbstractSocket::ProxyConnectionClosedError:
		// The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)
		// 到代理服务器的连接意外关闭(在与最终对等点的连接建立之前)
		break;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		// The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.
		// 到代理服务器的连接超时或代理服务器在身份验证阶段停止响应。
		break;
	case QAbstractSocket::ProxyNotFoundError:
		// The proxy address set with setProxy() (or the application proxy) was not found.
		// 没有找到使用setProxy()(或应用程序代理)设置的代理地址。
		break;
	case QAbstractSocket::ProxyProtocolError:
		// The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.
		// 与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应。
		break;
	case QAbstractSocket::OperationError:
		// 	An operation was attempted while the socket was in a state that did not permit it.
		// 当套接字处于不允许的状态时尝试操作。
		break;
	case QAbstractSocket::SslInternalError:
		// The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.
		// 在使用的SSL库报告了一个内部错误。这可能是安装不当或库配置错误的结果。
		break;
	case QAbstractSocket::SslInvalidUserDataError:
		// 	Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.
		// 提供了无效的数据(证书、密钥、密码等)，使用这些数据会导致SSL库出现错误。
		break;
	case QAbstractSocket::TemporaryError:
		// A temporary error occurred (e.g., operation would block and socket is non-blocking).
		// 一个临时的错误发生了(例如，操作将阻塞和套接字非阻塞)。
		break;
	case QAbstractSocket::UnknownSocketError:
		// An unidentified error occurred.
		// 发生了一个未知的错误。
		break;
	}
	return;
}

void Service::slotUpdateCallerEnableUse(const unsigned short& id)
{
	UpdateCallerUse(m_zgbCtrler.caller(id));

	return;
}

void Service::slotSerialPortStateChange(QString strOpenDesc)
{
	emit signalSerialPortStateChange(strOpenDesc);

	return;
}

void Service::slotUpdateCallerSignal(const unsigned short& id)
{
	m_bAllScream = m_zgbCtrler.IsScream();

	ZgbCaller caller = m_zgbCtrler.caller(id);

	UpdateCallerSignal(caller);

	const CallerAttr attr = caller.GetAttribute();

	if (attr.GetCancelSignal() == Signal::Signal_On)
	{
		// 取消订单
		CancelOrder(caller);

		return;
	}

	if (attr.GetCallSignal() != Signal::Signal_Off)
	{
		//m_zgbCtrler.ClearSignal(caller, SignalType::Signal_Call);

		// 下达订单
		PublishOrder(caller);
	}

	return;
}

void Service::slotThread()
{
	ScreamControl();

	if (m_bAllScream)
	{
		return;
	}

	TrafficControl();
	RestQueueControl();
	RestStationControl();

	auto dis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_ptCount);

	if (dis.count() >= 1000)
	{
		OrderManage();
		m_ptCount = std::chrono::steady_clock::now();
	}

	return;
}

void Service::slotPublishOrder(QString station, QString mission)
{
	QString log, strLoad = "", strUnload = "";

	if (mission == TASK_GETTRAY)
	{
		// 取回空托盘
		strLoad = station;
		strUnload = TASK_DEFAULT_TRAYLIB;

		// 至XXX取回空托盘
		log = QString::fromLocal8Bit("至%1%2").arg(station).arg(TASK_GETTRAY);
	}
	else if (mission == TASK_PUTBLANK)
	{
		// 送毛坯
		strLoad = TASK_DEFAULT_BLANKLIB;
		strUnload = station;

		// 运送毛坯料至XXX
		log = QString::fromLocal8Bit("%1至%2").arg(TASK_PUTBLANK).arg(station);
	}
	else if (mission == TASK_PUTTRAY)
	{
		// 送空托盘
		strLoad = TASK_DEFAULT_TRAYLIB;
		strUnload = station;

		// 运送空托盘至XXX
		log = QString::fromLocal8Bit("%1至%2").arg(TASK_PUTTRAY).arg(station);
	}
	else if (mission == TASK_TOLIB)
	{
		// 取成品入库
		strLoad = station;
		strUnload = TASK_DEFAULT_PRODUCTLIB;

		// 至XXX取成品入库
		log = QString::fromLocal8Bit("至%1%2").arg(station).arg(TASK_TOLIB);
	}
	else if (mission == TASK_TOSTATION)
	{
		// 取成品入站
		strLoad = station;
		strUnload = TASK_DEFAULT_PRODUCTSTA;

		// 至XXX取成品入站
		log = QString::fromLocal8Bit("至%1%2").arg(station).arg(TASK_TOSTATION);
	}
	else
	{
		return;
	}

	return PublishOrder(GetWorkStationPtr(strLoad), GetWorkStationPtr(strUnload), log);
}

void Service::slotUpdateClock()
{
	if (m_led == nullptr)
	{
		return;
	}

	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(tp);
	tm* t = localtime(&tt);

	QString wday;

	switch (t->tm_wday)
	{
	case 1:
		wday = QString::QString::fromLocal8Bit("一");
		break;
	case 2:
		wday = QString::QString::fromLocal8Bit("二");
		break;
	case 3:
		wday = QString::QString::fromLocal8Bit("三");
		break;
	case 4:
		wday = QString::QString::fromLocal8Bit("四");
		break;
	case 5:
		wday = QString::QString::fromLocal8Bit("五");
		break;
	case 6:
		wday = QString::QString::fromLocal8Bit("六");
		break;
	case 0:
		wday = QString::QString::fromLocal8Bit("日");
		break;
	}

	m_led->SetText(98, QString::fromLocal8Bit(" %1-%2-%3 星期%4 %5:%6:%7")
		.arg(t->tm_year + 1900, 4, 10, QLatin1Char('0'))
		.arg(t->tm_mon + 1, 2, 10, QLatin1Char('0'))
		.arg(t->tm_mday, 2, 10, QLatin1Char('0'))
		.arg(wday)
		.arg(t->tm_hour, 2, 10, QLatin1Char('0'))
		.arg(t->tm_min, 2, 10, QLatin1Char('0'))
		.arg(t->tm_sec, 2, 10, QLatin1Char('0')));

	return;
}

void Service::slotUpdateAGVStatus(const unsigned short& id)
{
	if (m_led == nullptr)
	{
		return;
	}

	AgvPtr agv = GetAGVPtr(id);

	if (agv == nullptr)
	{
		return;
	}

	QString target, text;

	if (agv->IsConnected() == false)
	{
		text = QString::fromLocal8Bit("AGV%1未连接").arg(id);

		m_led->SetText(9 + id, text);

		return;
	}

	AgvAttr attr = agv->GetAttribute();

	target = GetTargetName(attr.m_endLocat);

	if (attr.m_endLocat == attr.m_curLocat)
	{
		if (attr.GetAction() != AgvAction::Act_None)
		{
			QString act = agv->GetActionName();

			switch (attr.GetExecution())
			{
			case AgvExecution::Act_None:
				text = QString::fromLocal8Bit("AGV%1开始执行%2动作").arg(id).arg(act);
				break;
			case AgvExecution::Act_Exe:
				if (attr.GetActionExecuteTime() > 30 * 1000)
				{
					text = QString::fromLocal8Bit("AGV%1执行%2动作超时").arg(id).arg(act);
				}
				else
				{
					text = QString::fromLocal8Bit("AGV%1正在执行%2动作").arg(id).arg(act);
				}
				break;
			case AgvExecution::Act_Fin:
				text = QString::fromLocal8Bit("AGV%1执行%2动作完成").arg(id).arg(act);
				break;
			}
		}
		else if (attr.m_endLocat != 0)
		{
			text = QString::fromLocal8Bit("AGV%1到达%2").arg(id).arg(target);
		}
	}
	else if (attr.m_endLocat != 0)
	{
		text = QString::fromLocal8Bit("AGV%1正在向%2移动").arg(id).arg(target);
	}

	if (text.isNull() || text.isEmpty())
	{
		text = QString::fromLocal8Bit("AGV%1已连接").arg(id);
	}

	m_led->SetText(9 + id, text);

	return;
}

void Service::slotCallerEnable(int id, bool enable)
{
	if (enable)
	{
		EnableUseCaller(static_cast<unsigned short>(id));
	}
	else
	{
		DisableUseCaller(static_cast<unsigned short>(id));
	}

	return;
}

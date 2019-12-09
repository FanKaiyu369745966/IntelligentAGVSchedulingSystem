#include "ServiceBase.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>

ServiceBase::ServiceBase()
	: m_dbAGV(QSqlDatabase::addDatabase("QODBC3"))
	, m_protocolStm32(make_shared<ProtocolStm32>())
	, m_protocolPlc(make_shared<ProtocolPlc>())
	, m_led(nullptr)
{
}

ServiceBase::ServiceBase(const QSqlDatabase& db)
	: m_dbAGV(db)
	, m_protocolStm32(make_shared<ProtocolStm32>())
	, m_protocolPlc(make_shared<ProtocolPlc>())
	, m_led(nullptr)
{
}

ServiceBase::~ServiceBase()
{
}

bool ServiceBase::LoadAGVDBConfig(const QJsonDocument& doc)
{
	QString file = "AGV database";

	if (doc.isNull() || doc.isEmpty())
	{
		// 配置文档为空

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

	QString key = "AGVDatabase";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;

		return false;
	}

	return LoadAGVDBConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadAGVDBConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		qDebug() << "Load AGV database config failed:The object is empty" << endl;

		return false;
	}

	QString host = obj.value("Host").toString();
	QString user = obj.value("User").toString();
	QString password = obj.value("Password").toString();
	QString database = obj.value("Database").toString();

	if (m_dbAGV.isOpen())
	{
		m_dbAGV.close();
	}

	m_dbAGV.setDatabaseName(QString("DRIVER={SQL SERVER};"
		"SERVER=%1;"
		"DATABASE=%2;"
		"UID=%3;"
		"PWD=%4;").arg(host).arg(database).arg(user).arg(password));

	if (m_dbAGV.open() == false)
	{
		qDebug() << "Open AGV database failed:" << m_dbAGV.lastError().text() << endl;

		return false;
	}

	return true;
}

bool ServiceBase::LoadServerConfig(const QJsonDocument& doc)
{
	QString file = "Network server";
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

	QString key = "Server";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadServerConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadServerConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		// JSON对象为空

		qDebug() << "Load Network server config failed:The object is empty" << endl;
		return false;
	}

	QString	ip = obj.value("IP").toString();
	unsigned short port = static_cast<unsigned short>(obj.value("Port").toInt());

	QHostAddress addr;
	if (ip.isNull() || ip.isEmpty())
	{
		addr = QHostAddress::Any;
	}
	else
	{
		addr.setAddress(ip);
	}

	if (m_server.listen(addr, port) == false)
	{
		emit signalServerStateChange(QString::fromLocal8Bit("监听失败:%1").arg(m_server.errorString()));

		return false;
	}

	emit signalServerStateChange(QString::fromLocal8Bit("已监听(IP:%1,Port:%2)").arg(m_server.serverAddress().toString()).arg(m_server.serverPort()));

	return true;
}

bool ServiceBase::LoadLedConfig(const QJsonDocument& doc)
{
	QString file = "Led";
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

	QString key = "Led";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadLedConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadLedConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		// JSON对象为空

		qDebug() << "Load Led config failed:The object is empty" << endl;
		return false;
	}
	int id = obj.value("CardAddress").toInt();
	int width = obj.value("ScreemWidth").toInt();
	int height = obj.value("ScreemHeight").toInt();
	QString type = obj.value("FontType").toString();
	int size = obj.value("FontSize").toInt();
	int maxLine = obj.value("MaxLine").toInt();

	m_led = make_shared<EQLed>(id, width, height, maxLine, type, size);

	return true;
}

bool ServiceBase::LoadAGVTypeConfig()
{
	QString file = "AGV type";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_AGVTYPE");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objAttr;

		QString key = "type_brand";
		objAttr.insert("Brand", record.value(key).toString());

		key = "type_version";
		objAttr.insert("Version", record.value(key).toString());

		key = "type_max_speed";
		objAttr.insert("MaxSpeed", record.value(key).toFloat());

		key = "type_max_weight";
		objAttr.insert("MaxWeight", record.value(key).toFloat());

		key = "type_protocol";
		objAttr.insert("Protocol", record.value(key).toInt());

		key = "type_action";
		objAttr.insert("Action", QJsonDocument().fromJson(record.value(key).toString().toUtf8()).object());

		key = "type_name";
		obj.insert(record.value(key).toString(), objAttr);
	}

	return LoadAGVTypeConfig(obj);
}

bool ServiceBase::LoadAGVTypeConfig(const QJsonDocument& doc)
{
	QString file = "AGV type";
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

	QString key = "AGVType";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadAGVTypeConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadAGVTypeConfig(const QJsonObject& obj)
{
	QString file = "";

	if (obj.isEmpty())
	{
		qDebug() << "Load AGV type config failed:The object is empty" << endl;
		return false;
	}

	m_mapAGVTypes.clear();

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);												/*!< 键值 */

		QString name = key;															/*!< 类型名 */
		QJsonObject attr = obj.value(key).toObject();								/*!< 属性 */

		QString brand = attr.value("Brand").toString();								/*!< 品牌 */
		QString version = attr.value("Version").toString();							/*!< 型号 */
		float maxSpeed = static_cast<float>(attr.value("MaxSpeed").toDouble());		/*!< 最大速度 */
		float maxWeight = static_cast<float>(attr.value("MaxWeight").toDouble());	/*!< 最大速度 */
		int protocol = attr.value("Protocol").toInt();								/*!< 协议类型 */
		QJsonObject actList = attr.value("Action").toObject();						/*!< 动作列表 */

		std::shared_ptr<ProtocolBase> ptr = nullptr;	/*!< 通信协议指针 */

		switch (static_cast<ProtocolType>(protocol))
		{
		case ProtocolType::Protocol_Stm32:
			ptr = m_protocolStm32;
			break;
		case ProtocolType::Protocol_Plc:
			ptr = m_protocolPlc;
			break;
		default:
			break;
		}

		if (ptr == nullptr)
		{
			qDebug() << "Load AGV type" << name << " failed:The protocol type is null" << endl;
			continue;
		}

		m_mapAGVTypes[name] = make_shared<AgvType>(name, ptr, maxSpeed, maxWeight, brand, version);
		m_mapAGVTypes[name]->LoadNameList(actList);
	}

	return true;
}

bool ServiceBase::LoadAGVConfig()
{
	QString file = "AGV";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_AGV");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	QJsonObject obj;

	int ret = table.rowCount();

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objValue;

		QString key = "agv_type";
		objValue.insert("Type", record.value(key).toString());

		key = "agv_client";
		objValue.insert("Client", record.value(key).toBool());

		key = "agv_ip";
		objValue.insert("IP", record.value(key).toString());

		key = "agv_port";
		objValue.insert("Port", record.value(key).toInt());

		key = "agv_locol_ip";
		objValue.insert("LocalIP", record.value(key).toString());

		key = "agv_locol_port";
		objValue.insert("LocalPort", record.value(key).toInt());

		key = "agv_id";
		obj.insert(QString("%1").arg(record.value(key).toInt()), objValue);
	}

	return LoadAGVConfig(obj);
}

bool ServiceBase::LoadAGVConfig(const QJsonDocument& doc)
{
	QString file = "AGV";
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

	QString key = "AGVType";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadAGVTypeConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadAGVConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		qDebug() << "Load AGV config failed:Not a json string" << endl;
		return false;
	}

	m_mapAGVs.clear();

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);

		id_t id = static_cast<id_t>(key.toInt());													/*!< 编号 */
		QJsonObject attr = obj.value(key).toObject();

		QString type = attr.value("Type").toString();												/*!< 类型名称 */
		bool bClient = attr.value("Client").toBool();												/*!< 网络模式 */
		QString addr = attr.value("IP").toString();													/*!< 下位机地址 */
		unsigned short port = static_cast<unsigned short>(attr.value("Port").toInt());				/*!< 下位机端口 */
		QString addrLocal = attr.value("LocalIP").toString();										/*!< 本地地址 */
		unsigned short localPort = static_cast<unsigned short>(attr.value("LocalPort").toInt());	/*!< 本地端口 */

		if (addr.isNull() || addr.isEmpty())
		{
			qDebug() << "Load AGV:" << key << " config failed:The address is null." << endl;
			continue;
		}

		AgvTypePtr ptr = GetAGVTypePtr(type);											/*!< AGV类型指针 */

		if (ptr == nullptr)
		{
			qDebug() << "Load AGV:" << key << " config failed:The type is null." << endl;
			continue;
		}

		try
		{
			m_mapAGVs[id] = std::make_shared<Agv>(ptr, id, bClient, addr, port, addrLocal, localPort);
		}
		catch (char* error)
		{
			qDebug() << error << endl;

			continue;
		}

		// 信号-槽 连接AGV连接成功信号
		QObject::connect(m_mapAGVs[id].get(), &Agv::signalConnected, this, &ServiceBase::slotAGVConnected);
		// 信号-槽 连接AGV连接中断信号
		QObject::connect(m_mapAGVs[id].get(), &Agv::signalConnectionBreak, this, &ServiceBase::slotAGVConnectionBreak);
		// 信号-槽 连接AGV状态更新信号
		QObject::connect(m_mapAGVs[id].get(), &Agv::signalUpdate, this, &ServiceBase::slotUpdateAGV);
	}

	return true;
}

bool ServiceBase::LoadRFIDConfig()
{
	QString file = "RFID";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:The database is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);
	table.setTable("AGVDB_INFO_RFID");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonArray arr;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QString key = "rfid_id";
		arr.append(record.value(key).toInt());
	}

	return LoadRFIDConfig(arr);
}

bool ServiceBase::LoadRFIDConfig(const QJsonDocument& doc)
{
	QString file = "RFID";

	if (doc.isNull() || doc.isEmpty())
	{
		// 配置文档为空

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

	QString key = "RFID";			/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	if (obj.value(key).isArray())
	{
		return LoadRFIDConfig(obj.value(key).toArray());
	}

	return LoadRFIDConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadRFIDConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		qDebug() << "Load RFID config failed:The object is empty" << endl;
		return false;
	}

	m_mapRFIDs.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		rfid_t id = static_cast<rfid_t>(listKeys.at(i).toInt());

		m_mapRFIDs[id] = make_shared<RFID>(id);
	}

	return true;
}

bool ServiceBase::LoadRFIDConfig(const QJsonArray& arr)
{
	if (arr.isEmpty())
	{
		qDebug() << "Load RFID config failed:The array is empty" << endl;
		return false;
	}

	m_mapRFIDs.clear();

	for (QJsonArray::const_iterator it = arr.begin(); it != arr.end(); ++it)
	{
		rfid_t id = static_cast<rfid_t>(it->toInt());

		m_mapRFIDs[id] = make_shared<RFID>(RFID(id));
	}

	return true;
}

bool ServiceBase::LoadTrafficConfig()
{
	QString file = "Traffic";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:The database is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);
	table.setTable("AGVDB_INFO_TRAFFIC");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objValue;

		QString key = "traffic_lock";
		objValue.insert("Lock", QJsonDocument().fromJson(record.value(key).toString().toUtf8()).array());

		key = "traffoc_compare";
		objValue.insert("Compare", QJsonDocument().fromJson(record.value(key).toString().toUtf8()).array());

		key = "ID";
		obj.insert(QString("%1").arg(record.value(key).toInt()), objValue);
	}

	LoadTrafficConfig(obj);

	return true;
}

bool ServiceBase::LoadTrafficConfig(const QJsonDocument& doc)
{
	QString file = "Traffic";

	if (doc.isNull() || doc.isEmpty())
	{
		// 配置文档为空

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

	QString key = "Traffic";			/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadTrafficConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadTrafficConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		qDebug() << "Load Traffic config failed:The object is empty" << endl;
		return false;
	}

	m_mapTraffics.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString	key = listKeys.at(i);
		rfid_t id = static_cast<rfid_t>(key.toInt());

		QJsonObject obj;
		obj = obj.value(key).toObject();

		if (obj.isEmpty())
		{
			continue;
		}

		QJsonArray arrLock, arrCompare;

		arrLock = obj.value("Lock").toArray();
		arrCompare = obj.value("Compare").toArray();

		m_mapTraffics[id] = TrafficPosition(id);
		m_mapTraffics[id].LoadLockList(arrLock);
		m_mapTraffics[id].LoadCompareList(arrCompare);
	}

	return true;
}

bool ServiceBase::LoadRestConfig()
{
	QString file = "Rest station";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_REST");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objvalue;

		QString key = "rester_name";
		objvalue.insert("Name", record.value(key).toString());

		key = "rester_location";
		objvalue.insert("Location", record.value(key).toInt());

		key = "rester_lock";
		objvalue.insert("Lock", QJsonDocument().fromJson(record.value(key).toString().toUtf8()).array());

		key = "rester_id";
		obj.insert(QString("%1").arg(record.value(key).toInt()), objvalue);
	}

	return LoadRestConfig(obj);
}

bool ServiceBase::LoadRestConfig(const QJsonDocument& doc)
{
	QString file = "Rest station";
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

	QString key = "RestStation";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadRestConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadRestConfig(const QJsonObject& obj)
{
	QString file = "Rest station";

	if (obj.isEmpty())
	{
		qDebug() << "Load " << file << " config failed:The object is empty" << endl;
		return false;
	}

	m_mapResters.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		unsigned short id = static_cast<unsigned short>(key.toInt());
		QString name = attr.value("Name").toString();
		rfid_t location = static_cast<rfid_t>(attr.value("Location").toInt());
		QJsonArray lock = attr.value("Lock").toArray();

		m_mapResters[id] = make_shared<RestStation>(id, location, name);
		m_mapResters[id]->LoadLockArea(lock);
	}

	return true;
}

bool ServiceBase::LoadRestQueueConfig()
{
	QString file = "Rest queue";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_RESTQUEUE");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		obj.insert(QString("%1").arg(record.value("queue_id").toInt()), QJsonDocument().fromJson(record.value("queue_list").toString().toUtf8()).array());
	}

	return 	LoadRestQueueConfig(obj);
}

bool ServiceBase::LoadRestQueueConfig(const QJsonDocument& doc)
{
	QString file = "Rest queue";
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

	QString key = "RestQueue";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadRestQueueConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadRestQueueConfig(const QJsonObject& obj)
{
	QString file = "Rest queue";

	if (obj.isEmpty())
	{
		qDebug() << "Load " << file << " config failed:Not a json string" << endl;
		return false;
	}

	m_mapResters.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonArray arr = obj.value(key).toArray();

		unsigned short id = static_cast<unsigned short>(key.toInt());

		m_mapRestQueues[id] = RestQueue(id);
		m_mapRestQueues[id].LoadQueue(arr);
	}

	return true;
}

bool ServiceBase::LoadWorkConfig()
{
	QString file = "Work station";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_WORK");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objvalue;

		QString key = "worker_name";
		objvalue.insert("Name", record.value(key).toString());

		key = "worker_location";
		objvalue.insert("Location", record.value(key).toInt());

		key = "worker_id";
		obj.insert(QString("%1").arg(record.value(key).toInt()), objvalue);
	}

	return LoadWorkConfig(obj);
}

bool ServiceBase::LoadWorkConfig(const QJsonDocument& doc)
{
	QString file = "Word station";
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

	QString key = "WorkStation";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadWorkConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadWorkConfig(const QJsonObject& obj)
{
	if (obj.isEmpty())
	{
		qDebug() << "Load Work station config failed:The object is empty" << endl;
		return false;
	}

	m_mapWorkers.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		unsigned short id = static_cast<unsigned short>(key.toInt());
		QString name = attr.value("Name").toString();
		rfid_t location = static_cast<rfid_t>(attr.value("Location").toInt());

		m_mapWorkers[id] = std::make_shared<WorkStation>(id, location, name);
	}

	return true;
}

bool ServiceBase::LoadChargeConfig()
{
	QString file = "Charge station";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_CHARGER");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objvalue;

		QString key = "charger_name";
		objvalue.insert("Name", record.value(key).toString());

		key = "charger_location";
		objvalue.insert("Location", record.value(key).toInt());

		key = "charger_id";
		obj.insert(QString("%1").arg(record.value(key).toInt()), objvalue);
	}

	return LoadChargeConfig(obj);
}

bool ServiceBase::LoadChargeConfig(const QJsonDocument& doc)
{
	QString file = "Charge station";
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

	QString key = "ChargeStation";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadChargeConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadChargeConfig(const QJsonObject& obj)
{
	QString file = "Charge station";

	if (obj.isEmpty())
	{
		qDebug() << "Load " << file << " config failed:The object is empty" << endl;
		return false;
	}

	m_mapChargers.clear();

	QStringList listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		unsigned short id = static_cast<unsigned short>(key.toInt());
		QString name = attr.value("Name").toString();
		rfid_t location = static_cast<rfid_t>(attr.value("Location").toInt());

		m_mapChargers[id] = std::make_shared<ChargeStation>(id, location, name);
	}

	return true;
}

bool ServiceBase::LoadZgbeCtrlConfig(const QJsonDocument& doc)
{
	QString file = "Zigbee controler";
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

	QString key = "ZigbeeControler";		/*!< JSON 键值 */

	if (obj.find(key) == obj.end())
	{
		// 未找到Key

		qDebug() << "Load " << file << " config failed: Can not find key:" << key << endl;
		return false;
	}

	return LoadZgbeCtrlConfig(obj.value(key).toObject());
}

bool ServiceBase::LoadZgbeCtrlConfig(const QJsonObject& obj)
{
	QString file = "Zigbee controler";

	if (obj.isEmpty())
	{
		// JSON对象为空

		qDebug() << "Load " << file << " config failed:The object is empty" << endl;
		return false;
	}

	QString key = "Port";
	QString port = obj.value(key).toString(); /*!< 串口号 */

	key = "Baud";
	QSerialPort::BaudRate baud = static_cast<QSerialPort::BaudRate>(obj.value(key).toInt());	/*!< 波特率 */

	if (m_zgbCtrler.IsOpened())
	{
		m_zgbCtrler.Close();
	}

	if (m_zgbCtrler.Open(port, baud) == false)
	{
		return false;
	}

	return true;
}

bool ServiceBase::LoadZigbeeCaller()
{
	QString file = "Zigbee caller";

	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load " << file << " config failed:" << "The databse is not open" << endl;

		return false;
	}

	QSqlTableModel table(nullptr, m_dbAGV);

	table.setTable("AGVDB_INFO_CALLER");

	if (table.select() == false)
	{
		qDebug() << "Load " << file << " config failed:" << table.lastError().text() << endl;

		return false;
	}

	int ret = table.rowCount();

	QJsonObject obj;

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		QJsonObject objValue;

		objValue.insert("MAC", record.value("caller_address").toString());
		objValue.insert("Bind", record.value("caller_bind_station").toString());

		obj.insert(QString("%1").arg(record.value("caller_id").toInt()), objValue);
	}

	return m_zgbCtrler.LoadCaller(obj);
}

bool ServiceBase::LoadZigbeeCaller(const QJsonDocument& doc)
{
	return m_zgbCtrler.LoadCaller(doc);
}

bool ServiceBase::LoadZigbeeCaller(const QJsonObject& obj)
{
	return m_zgbCtrler.LoadCaller(obj);
}

AgvTypePtr ServiceBase::GetAGVTypePtr(const QString& name) const
{
	if (m_mapAGVTypes.find(name) == m_mapAGVTypes.end())
	{
		return nullptr;
	}

	return m_mapAGVTypes.at(name);
}

AgvPtr ServiceBase::GetAGVPtr(const id_t& id) const
{
	if (m_mapAGVs.find(id) == m_mapAGVs.end())
	{
		return nullptr;
	}

	return m_mapAGVs.at(id);
}

const RFID ServiceBase::GetRFID(const rfid_t& id) const
{
	RFIDPtr ptr = GetRFIDPtr(id);

	if (ptr == nullptr)
	{
		return RFID();
	}

	return *ptr;
}

RFIDPtr ServiceBase::GetRFIDPtr(const rfid_t& id) const
{
	if (m_mapRFIDs.find(id) == m_mapRFIDs.end())
	{
		return nullptr;
	}

	return m_mapRFIDs.at(id);
}

TrafficPos ServiceBase::GetTrafficPosition(const rfid_t& id) const
{
	if (m_mapTraffics.find(id) == m_mapTraffics.end())
	{
		return TrafficPos();
	}

	return m_mapTraffics.at(id);
}

ResterPtr ServiceBase::restStationPtr(const unsigned short& id) const
{
	if (id == 0)
	{
		return nullptr;
	}

	if (m_mapResters.find(id) == m_mapResters.end())
	{
		return nullptr;
	}

	return m_mapResters.at(id);
}

ResterPtr ServiceBase::GetRestStationPtr(const QString& name) const
{
	if (name.isNull() || name.isEmpty())
	{
		return nullptr;
	}

	for (ResterArray::const_iterator it = m_mapResters.begin(); it != m_mapResters.end(); ++it)
	{
		if (it->second->GetName() == name)
		{
			return it->second;
		}
	}

	return nullptr;
}

ResterPtr ServiceBase::GetRestStationPtr(const rfid_t& rfid) const
{
	if (rfid == 0)
	{
		return nullptr;
	}

	for (ResterArray::const_iterator it = m_mapResters.begin(); it != m_mapResters.end(); ++it)
	{
		if (it->second->GetLocation() == rfid)
		{
			return it->second;
		}
	}

	return nullptr;
}

const RestStation ServiceBase::restStation(const unsigned short& id) const
{
	ResterPtr ptr = restStationPtr(id);

	if (ptr == nullptr)
	{
		return RestStation();
	}

	return *ptr;
}

const RestStation ServiceBase::GetRestStation(const QString& name) const
{
	ResterPtr ptr = GetRestStationPtr(name);

	if (ptr == nullptr)
	{
		return RestStation();
	}

	return *ptr;
}

const RestStation ServiceBase::GetRestStation(const rfid_t& rfid) const
{
	ResterPtr ptr = GetRestStationPtr(rfid);

	if (ptr == nullptr)
	{
		return RestStation();
	}

	return *ptr;
}

const RestQueue ServiceBase::restQueue(const unsigned short& id) const
{
	if (id == 0)
	{
		return RestQueue();
	}

	if (m_mapRestQueues.find(id) == m_mapRestQueues.end())
	{
		return RestQueue();
	}

	return m_mapRestQueues.at(id);
}

const RestQueue ServiceBase::GetRestQueue(const unsigned short& id) const
{
	if (id == 0)
	{
		return RestQueue();
	}

	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		if (it->second.IsInside(id))
		{
			return it->second;
		}
	}

	return RestQueue();
}

WorkerPtr ServiceBase::workStationPtr(const unsigned short& id) const
{
	if (id == 0)
	{
		return nullptr;
	}

	if (m_mapWorkers.find(id) == m_mapWorkers.end())
	{
		return nullptr;
	}

	return m_mapWorkers.at(id);
}

WorkerPtr ServiceBase::GetWorkStationPtr(const QString& name) const
{
	if (name.isNull() || name.isEmpty())
	{
		return nullptr;
	}

	for (WorkerArray::const_iterator it = m_mapWorkers.begin(); it != m_mapWorkers.end(); ++it)
	{
		if (it->second->GetName() == name)
		{
			return it->second;
		}
	}

	return nullptr;
}

WorkerPtr ServiceBase::GetWorkStationPtr(const rfid_t& rfid) const
{
	if (rfid == 0)
	{
		return nullptr;
	}

	for (WorkerArray::const_iterator it = m_mapWorkers.begin(); it != m_mapWorkers.end(); ++it)
	{
		if (it->second->GetLocation() == rfid)
		{
			return it->second;
		}
	}

	return nullptr;
}

const WorkStation ServiceBase::workStation(const unsigned short& id) const
{
	WorkerPtr ptr = workStationPtr(id);

	if (ptr == nullptr)
	{
		return WorkStation();
	}

	return *ptr;
}

const WorkStation ServiceBase::GetWorkStation(const QString& name) const
{
	WorkerPtr ptr = GetWorkStationPtr(name);

	if (ptr == nullptr)
	{
		return WorkStation();
	}

	return *ptr;
}

const WorkStation ServiceBase::GetWorkStation(const rfid_t& rfid) const
{
	WorkerPtr ptr = GetWorkStationPtr(rfid);

	if (ptr == nullptr)
	{
		return WorkStation();
	}

	return *ptr;
}

ChargerPtr ServiceBase::chargeStationPtr(const unsigned short& id) const
{
	if (id == 0)
	{
		return nullptr;
	}

	if (m_mapChargers.find(id) == m_mapChargers.end())
	{
		return nullptr;
	}

	return m_mapChargers.at(id);
}

ChargerPtr ServiceBase::GetChargeStationPtr(const QString& name) const
{
	if (name.isNull() || name.isEmpty())
	{
		return nullptr;
	}

	for (ChargerArray::const_iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		if (it->second->GetName() == name)
		{
			return it->second;
		}
	}

	return nullptr;
}

ChargerPtr ServiceBase::GetChargeStationPtr(const rfid_t& rfid) const
{
	if (rfid == 0)
	{
		return nullptr;
	}

	for (ChargerArray::const_iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		if (it->second->GetLocation() == rfid)
		{
			return it->second;
		}
	}

	return nullptr;
}

const ChargeStation ServiceBase::chargeStation(const unsigned short& id) const
{
	ChargerPtr ptr = chargeStationPtr(id);

	if (ptr == nullptr)
	{
		return ChargeStation();
	}

	return *ptr;
}

const ChargeStation ServiceBase::GetChargeStation(const QString& name) const
{
	ChargerPtr ptr = GetChargeStationPtr(name);

	if (ptr == nullptr)
	{
		return ChargeStation();
	}

	return *ptr;
}

const ChargeStation ServiceBase::GetChargeStation(const rfid_t& rfid) const
{
	ChargerPtr ptr = GetChargeStationPtr(rfid);

	if (ptr == nullptr)
	{
		return ChargeStation();
	}

	return *ptr;
}

void ServiceBase::LoadAGVStatus()
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load AGV status failed:The database is not open" << endl;

		return;
	}

	for (AgvArray::const_iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		LoadAGVStatus(*(it->second));
	}

	return;
}

void ServiceBase::LoadAGVStatus(Agv& agv)
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load AGV:" << agv.GetId() << " status failed:The database is not open" << endl;

		return;
	}

	QSqlTableModel table(nullptr, m_dbAGV);
	table.setTable("AGVDB_INFO_AGV");

	table.setFilter(QString("agv_id = %1").arg(agv.GetId()));

	if (table.select() == false)
	{
		qDebug() << "Load AGV:" << agv.GetId() << " status failed:" << table.lastError().text() << endl;

		return;
	}

	QSqlRecord record = table.record(0);

	AgvAttr attr;
	//attr.m_mode = static_cast<AgvAttr::mode_t>(record.value("agv_mode").toInt());
	attr.m_curLocat = static_cast<rfid_t>(record.value("agv_cur").toInt());
	attr.m_endLocat = static_cast<rfid_t>(record.value("agv_target").toInt());
	attr.m_status = static_cast<AgvAttr::status_t>(record.value("agv_status").toInt());
	//attr.m_battery = static_cast<AgvAttr::battery_t>(record.value("agv_battery").toInt());
	//attr.m_speed = static_cast<AgvAttr::speed_t>(record.value("agv_speed").toInt());
	attr.m_cargo = static_cast<AgvAttr::cargo_t>(record.value("agv_cargo").toInt());
	attr.m_action = static_cast<AgvAttr::action_t>(record.value("agv_action").toInt());
	attr.m_execute = static_cast<AgvAttr::execute_t>(record.value("agv_action_status").toInt());
	attr.m_error = static_cast<AgvAttr::error_t>(record.value("agv_error").toInt());
	attr.SetActionExecuteTime(static_cast<size_t>(record.value("agv_action_time").toLongLong()));

	agv.UpdateAttribute(attr);

	ShowAGV(agv);

	emit signalUpdateAGVStatus(agv.GetId());

	RFIDPtr end = GetRFIDPtr(attr.GetEndLocation());

	if (end)
	{
		end->PeerLock(&agv);
	}

	RFIDPtr oldEnd = GetRFIDPtr(attr.GetOldEndLocation());

	if (oldEnd)
	{
		oldEnd->Cancel(&agv);
	}

	RFIDPtr cur = GetRFIDPtr(attr.GetCurLocation());

	if (cur)
	{
		cur->Lock(&agv);
	}

	RFIDPtr old = GetRFIDPtr(attr.GetOldLocation());

	if (old)
	{
		old->Free(&agv);
	}

	return;
}

void ServiceBase::UpdateAGVStatus() const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update AGV status failed:The database is not open" << endl;

		return;
	}

	for (AgvArray::const_iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		UpdateAGVStatus(*(it->second));
	}

	return;
}

void ServiceBase::UpdateAGVStatus(const Agv& agv) const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update AGV:" << agv.GetId() << " status failed:The database is not open" << endl;

		return;
	}

	AgvAttr attr = agv.GetAttribute();

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_AGV", sql;

	int speed = static_cast<int>(attr.m_speed);

	sql = QString::fromLocal8Bit("update %1 set [agv_mode]=%2,[agv_mode_info]='%3',\
[agv_cur]=%4,[agv_target]=%5,\
[agv_status]=%6,[agv_status_info]='%7',\
[agv_battery]=%8,\
[agv_speed]=%9,[agv_speed_info]=%10,\
[agv_cargo]=%11,[agv_cargo_info]='%12',\
[agv_action]=%13,[agv_action_status]=%14,[agv_action_time]=%15,[agv_action_info]='%16',\
[agv_error]=%17,[agv_error_info]='%18' where [agv_id]=%19")
.arg(table)
.arg(attr.m_mode).arg(attr.GetModeInfo())
.arg(attr.m_curLocat).arg(attr.m_endLocat)
.arg(attr.m_status).arg(attr.GetStatusInfo())
.arg(attr.m_battery)
.arg(speed).arg(agv.GetActualSpeed())
.arg(attr.m_cargo).arg(attr.GetCargoInfo())
.arg(attr.m_action).arg(attr.m_execute).arg(attr.GetActionExecuteTime()).arg(agv.GetActionInfo())
.arg(attr.m_error).arg(attr.GetErrorInfo())
.arg(agv.GetId());

	if (query.exec(sql) == false)
	{
		qDebug() << "Update AGV" << agv.GetId() << " status failed:" << query.lastError().text() << endl;
	}

	QString source = QString::fromLocal8Bit("AGV%1").arg(agv.GetId());

	AgvBattery battery = agv.GetAttribute().GetBattery();

	if (battery > AgvBattery::Power_Low)
	{// 电量恢复，清除异常
		ClearError(source, 1);
	}
	else if (battery > AgvBattery::Power_Off)
	{// 电量过低，抛出异常
		QString detail = QString::fromLocal8Bit("AGV电量不足,请及时充电或更换电池");
		ThrowError(source, detail, 1);
	}

	if (agv.GetAttribute().GetError() != AgvError::Error_None)
	{// AGV异常，抛出异常
		QString detail = QString::fromLocal8Bit("%1").arg(attr.GetErrorInfo());
		ThrowError(source, detail, 2);
	}
	else
	{// AGV异常解除，清除异常
		ClearError(source, 2);
	}

	return;
}

void ServiceBase::UpdateAGVConnection() const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update AGV connection failed:The database is not open" << endl;

		return;
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_AGV", sql;

	sql = QString::fromLocal8Bit("update %1 set [agv_connection]='未连接' ").arg(table);

	if (query.exec(sql) == false)
	{
		qDebug() << "Update AGV connection failed:" << query.lastError().text() << endl;
	}

	return;

}

void ServiceBase::UpdateAGVConnection(const Agv& agv) const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Update AGV:" << agv.GetId() << " connection failed:The database is not open" << endl;

		return;
	}

	QString str;
	if (agv.IsConnected())
	{
		str = QString::fromLocal8Bit("已连接");
	}
	else
	{
		str = QString::fromLocal8Bit("未连接");
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_AGV", sql;

	sql = QString::fromLocal8Bit("update %1 set [agv_connection]='%2' where [agv_id]=%3").arg(table).arg(str).arg(agv.GetId());

	if (query.exec(sql) == false)
	{
		qDebug() << "Update AGV" << agv.GetId() << " connection failed:" << query.lastError().text() << endl;
	}

	return;
}

void ServiceBase::ShowAGV(const Agv& agv)
{
	if (agv.IsConnected() == false)
	{
		return;
	}

	AgvAttr attr = agv.GetAttribute();

	bool bError = false, bObs = false, bUnvol = false, bRun = false, bAuto = false;

	if (attr.GetMode() == AgvMode::Mode_Auto)
	{
		bAuto = true;
	}

	switch (attr.GetError())
	{
	case AgvError::Error_MObs:
		bObs = true;
		break;
	case AgvError::Error_Obs:
		bObs = true;
		break;
	case AgvError::Error_Miss:
		bError = true;
		break;
	}

	if (attr.GetBattery() <= AgvBattery::Power_Low && attr.GetBattery() > AgvBattery::Power_Off)
	{
		bUnvol = true;
	}

	switch (attr.GetStatus())
	{
	case AgvStatus::Sta_ObsDown:
		bObs = true;
		break;
	case AgvStatus::Sta_SpeedDown:
		bRun = true;
		break;
	case AgvStatus::Sta_SpeedUp:
		bRun = true;
		break;
	case AgvStatus::Sta_Run:
		bRun = true;
		break;
	}

	emit signalShowAgv(agv.GetId(), attr.GetCurLocation(), bAuto, bRun, bUnvol, bObs, bError);

	return;
}

void ServiceBase::BindRestQueue()
{
	for (RestQueueArray::const_iterator it = m_mapRestQueues.begin(); it != m_mapRestQueues.end(); ++it)
	{
		BindRestQueue(it->second);
	}

	return;
}

void ServiceBase::BindRestQueue(const RestQueue& queue)
{
	unsigned short id = queue.GetTail();

	while (id != 0)
	{
		ResterPtr ptr = restStationPtr(id);

		if (ptr)
		{
			ptr->SetQueueID(queue.GetId());
		}

		id = queue.GetPrevStation(id);
	}

	return;
}

void ServiceBase::LoadCallerSignal()
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load caller signal failed:" << "The database is not open" << endl;

		return;
	}

	QSqlTableModel table(nullptr, m_dbAGV);
	table.setTable("AGVDB_INFO_CALLER");

	if (table.select() == false)
	{
		qDebug() << "Load caller signal failed:" << table.lastError().text() << endl;

		return;
	}

	int ret = table.rowCount();

	for (int i = 0; i < ret; ++i)
	{
		QSqlRecord record = table.record(i);

		bool enable = record.value("Enable").toBool();
		unsigned short id = static_cast<unsigned short>(record.value("ID").toInt());
		CallerAttr attr;

		attr.m_sigCall = static_cast<unsigned char>(record.value("Call").toInt());
		attr.m_sigCancel = static_cast<unsigned char>(record.value("Cancel").toInt());
		attr.m_sigScream = static_cast<unsigned char>(record.value("Scream").toInt());

		if (m_zgbCtrler.LoadSignal(id, enable, attr))
		{
			emit signalUpdateCallerUse(id, enable);

			UpdateCallerSignal(id, attr);
		}
	}

	return;
}

void ServiceBase::LoadCallerSignal(const ZigbeeCaller& caller)
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Load caller signal failed:" << "The database is not open" << endl;

		return;
	}

	QSqlTableModel table(nullptr, m_dbAGV);
	table.setTable("AGVDB_INFO_CALLER");

	table.setFilter(QString("caller_id = %1").arg(caller.GetId()));

	if (table.select() == false)
	{
		qDebug() << "Load caller signal failed:" << table.lastError().text() << endl;

		return;
	}

	int ret = table.rowCount();

	QSqlRecord record = table.record(0);

	bool enable = record.value("Enable").toBool();
	unsigned short id = static_cast<unsigned short>(record.value("ID").toInt());
	CallerAttr attr;

	attr.m_sigCall = static_cast<unsigned char>(record.value("Call").toInt());
	attr.m_sigCancel = static_cast<unsigned char>(record.value("Cancel").toInt());
	attr.m_sigScream = static_cast<unsigned char>(record.value("Scream").toInt());

	if (m_zgbCtrler.LoadSignal(id, enable, attr))
	{
		emit signalUpdateCallerUse(id, enable);
		UpdateCallerSignal(id, attr);
	}

	return;
}

void ServiceBase::UpdateCallerSignal(const unsigned short& id, const CallerAttr& attr)
{
	bool bCall = false, bCancel = false, bScream = false, bPass = false;

	if (attr.GetScreamSignal() != Signal::Signal_Off)
	{
		bScream = true;
	}

	if (attr.GetCallSignal() != Signal::Signal_Off)
	{
		bCall = true;
	}

	if (attr.GetCancelSignal() != Signal::Signal_Off)
	{
		bCancel = true;
	}

	// 信号-槽 发出呼叫器更新信号
	emit signalUpdateCallerSignal(id, bScream, bCall, bPass, bCancel);

	return;
}

void ServiceBase::ThrowError(const QString& source, const QString& detail, const int& level) const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Throw error failed:The database is not open" << endl;

		return;
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_ABNORMAL", sql;

	sql = QString::fromLocal8Bit("insert into %1([abnormal_source],[abnormal_desc],[abnormal_level]) values('%2','%3',%4)").arg(table).arg(source).arg(detail).arg(level);

	if (query.exec(sql) == false)
	{
		qDebug() << "Throw error failed:" << query.lastError().text() << endl;

		return;
	}

	return;
}

void ServiceBase::ClearError(const QString& source, const QString& detail) const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Throw error failed:The database is not open" << endl;

		return;
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_ABNORMAL", sql;

	sql = QString::fromLocal8Bit("update %1 set [abnormal_state]=3 where [abnormal_source]='%2' and [abnormal_desc]='%3' and [abnormal_state]<>3").arg(table).arg(source).arg(detail);

	if (query.exec(sql) == false)
	{
		qDebug() << "Throw error failed:" << query.lastError().text() << endl;

		return;
	}

	return;
}

void ServiceBase::ClearError(const QString& source, const int& level) const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Throw error failed:The database is not open" << endl;

		return;
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_ABNORMAL", sql;

	sql = QString::fromLocal8Bit("update %1 set [abnormal_state]=3 where [abnormal_source]='%2' and [abnormal_level]='%3' and [abnormal_state]<>3").arg(table).arg(source).arg(level);

	if (query.exec(sql) == false)
	{
		qDebug() << "Throw error failed:" << query.lastError().text() << endl;

		return;
	}

	return;
}

void ServiceBase::ClearError() const
{
	if (m_dbAGV.isOpen() == false)
	{
		qDebug() << "Throw error failed:The database is not open" << endl;

		return;
	}

	QSqlQuery query(m_dbAGV);
	QString table = "AGVDB_INFO_ABNORMAL", sql;

	sql = QString::fromLocal8Bit("update %1 set [abnormal_state]=3 where [abnormal_state]<>3").arg(table);

	if (query.exec(sql) == false)
	{
		qDebug() << "Throw error failed:" << query.lastError().text() << endl;

		return;
	}

	return;
}

void ServiceBase::slotAGVConnected(const unsigned short& id)
{
	AgvPtr agv = GetAGVPtr(id);

	if (agv == nullptr)
	{
		return;
	}

	// 通知界面AGV连接
	UpdateAGVConnection(*agv);

	ShowAGV(*agv);

	return;
}

void ServiceBase::slotAGVConnectionBreak(const unsigned short& id)
{
	AgvPtr agv = GetAGVPtr(id);

	if (agv == nullptr)
	{
		return;
	}

	// 通知界面AGV中断
	UpdateAGVConnection(*agv);

	emit signalHideAgv(agv->GetId());

	return;
}

void ServiceBase::slotUpdateAGV(const unsigned short& id)
{
	AgvPtr agv = GetAGVPtr(id);

	if (agv == nullptr)
	{
		return;
	}

	// 通知界面AGV状态更新

	UpdateAGVStatus(*agv);

	ShowAGV(*agv);

	emit signalUpdateAGVStatus(id);

	AgvAttr attr = agv->GetAttribute();

	RFIDPtr end = GetRFIDPtr(attr.GetEndLocation());

	if (end)
	{
		end->PeerLock(agv.get());
	}

	RFIDPtr oldEnd = GetRFIDPtr(attr.GetOldEndLocation());

	if (oldEnd)
	{
		oldEnd->Cancel(agv.get());
	}

	RFIDPtr cur = GetRFIDPtr(attr.GetCurLocation());

	if (cur)
	{
		cur->Lock(agv.get());
	}

	RFIDPtr old = GetRFIDPtr(attr.GetOldLocation());

	if (old)
	{
		old->Free(agv.get());
	}

	return;
}

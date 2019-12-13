#include "MapForm.h"
#include "ui_MapForm.h"
#include <QFileDialog>

MapForm::MapForm(const QSqlDatabase& db, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::MapForm),
	m_db(db)
{
	ui->setupUi(this);

	setStyleSheet("QWidget{border: 1px solid #34373E;}");

	m_pMap = std::make_shared<GraphicMap>(m_db, this);
	m_pMap->drag(QPoint(0, 0));
}

MapForm::~MapForm()
{
	delete ui;
}

void MapForm::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event)

		QPainter painter(this);

	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	m_pMap->drawMap(painter);
}

void MapForm::onCursorChange()
{
	this->setCursor(Qt::ArrowCursor);
}

void MapForm::wheelEvent(QWheelEvent* event)
{
	// 鼠标中轮“缩放”操作
	if (m_pMap)
	{
		m_pMap->wheel(event->delta() >= 0 ? false : true);
		this->setCursor(Qt::CrossCursor);
		QTimer::singleShot(500, this, SLOT(onCursorChange()));
		event->accept();
		this->update();
	}
}

void MapForm::mousePressEvent(QMouseEvent* event)
{
	// 鼠标左键按下“选择”操作
	if (event->button() == Qt::LeftButton)
	{
		if (m_pMap)
		{
			m_pMap->select(BUTTONCTRL_LDOWN, event->pos());
		}
		event->accept();
		this->update();
	}

	// Alt+鼠标右键弹出更新地图菜单
	else if (event->button() == Qt::RightButton && event->modifiers() == Qt::AltModifier)
	{
		QMenu* pMenu = new QMenu(this);
		pMenu->setStyleSheet("QMenu {background-color: #323334; border:none; font-size:12pt; font-family:Microsoft YaHei; color:#D0D0D0}\
                        QMenu::item {background-color: transparent; padding:8px 32px; margin:0px 0px;}\
                        QMenu::item:selected {background-color:#1E1F1F; color:#FFFFFF}\
                        QMenu::item:!selected {background-color:#323334; color:#D0D0D0}");
		QAction* pActionUpdateMap = new QAction(QString::fromLocal8Bit("更新地图"), this);
		pActionUpdateMap->setData(1);
		pMenu->addAction(pActionUpdateMap);
		connect(pActionUpdateMap, SIGNAL(triggered()), this, SLOT(onUpdateMapMenuEvent()));
		QPoint pos = cursor().pos();
		pos.setX(pos.x() + 5);
		pMenu->exec(pos);

		delete pActionUpdateMap;
		delete pMenu;
	}
}

void MapForm::mouseReleaseEvent(QMouseEvent* event)
{
	// 鼠标左键抬起“提交”操作
	if (event->button() == Qt::LeftButton)
	{
		if (m_pMap)
		{
			m_pMap->confirm(BUTTONCTRL_LUP, event->pos());
			QCursor cursor = m_pMap->drag(event->pos()); // 完成拖拽需再调用一次
			this->setCursor(cursor);
		}
		event->accept();
		this->update();
	}
}

void MapForm::mouseMoveEvent(QMouseEvent* event)
{
	// 鼠标左键按下且移动为“拖拽”操作
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_pMap)
		{
			QCursor cursor = m_pMap->drag(event->pos());
			this->setCursor(cursor);
		}
		event->accept();
		this->update();
	}
}

void MapForm::onUpdateMapMenuEvent()
{
	MsgBoxEx* msgBox = new MsgBoxEx();
	connect(msgBox, SIGNAL(btnOkClicked()), this, SLOT(onBtnOkClicked()));
	connect(msgBox, SIGNAL(btnCancelClicked()), this, SLOT(onBtnCancelClicked()));
	msgBox->setMsgBoxMode(QString::fromLocal8Bit("与地图相关数据将会被新地图数据覆盖，读取成功后将自动重启,\n此操作不可逆，确定更新吗？"), "", MsgBoxBtnType::MsgBoxBtnType_OkCancle);
	delete msgBox;
}
void MapForm::onBtnOkClicked()
{
	QString path = QFileDialog::getOpenFileName(NULL, QString::fromLocal8Bit("请选择地图文件"), ".", "map(*.map)");

	if (path.isNull() || path.isEmpty())
	{
		return;
	}

	QFile file(path);
	if (!file.exists())
	{
		MsgBoxEx* msgBox = new MsgBoxEx();
		msgBox->setMsgBoxMode(QString::fromLocal8Bit("未找到 %1 文件,请检查此文件是否在！").arg(path));
		delete msgBox;
		return;
	}

	emit onUpdateMapRelateTable();

	if (UpdateMapRelateTable(path))
	{
		m_pMap->loadMapElement();

		qApp->exit(RETCODE_RESTART);
	}
	else
	{
		MsgBoxEx* msgBox = new MsgBoxEx();
		msgBox->setMsgBoxMode(QString::fromLocal8Bit("地图更新失败！"));
		delete msgBox;
		return;
	}
}

void MapForm::onBtnCancelClicked() {}

void MapForm::onShowAgv(quint32 unNo, quint32 unCurMark, bool bAuto, bool bRun, bool bUnVol, bool bObs, bool bError)
{
	if (m_pMap)
	{
		m_pMap->showAgv(unNo, unCurMark, bAuto, bRun, bUnVol, bObs, bError);
	}

	return;
}

void MapForm::onHideAgv(quint32 unNo)
{
	if (m_pMap)
	{
		m_pMap->hideAgv(unNo);
	}

	return;
}

void MapForm::onUpdateCallerSignal(quint32 unNo, bool scream, bool call, bool pass, bool cancel)
{
	if (m_pMap)
	{
		m_pMap->updateCallerSignal(unNo, scream, call, pass, cancel);
	}

	return;
}

void MapForm::onUpdateCallerEnable(int unNo, bool enable)
{
	if (m_pMap)
	{
		m_pMap->updateCallerEnable(unNo, enable);
	}

	return;
}

bool MapForm::UpdateMapRelateTable(const QString& path)
{
	QFile file(path);
	if (!file.open(QFile::ReadOnly))
	{
		return false;
	}

	QDomDocument xmlDoc;			/*!< XML格式文档 */
	QString strError = "";			/*!< 错误信息 */
	int nLine = 0, nColumn = 0;	/*!< 错误行和列 */

	// 读取XML文档
	if (!xmlDoc.setContent(&file, &strError, &nLine, &nColumn))
	{
		qDebug() << "Loading xml file failed,Line:" << nLine << ",Column:" << nColumn << ",Error:" << strError << endl;

		QJsonDocument jsonDoc;
		QJsonParseError jsonError;
		jsonDoc = QJsonDocument::fromJson(file.readAll(), &jsonError);

		file.close();

		if (jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "Loading json file failed,Error:" << jsonError.errorString() << endl;
			return false;
		}

		return UpdateMapRelateTable(jsonDoc);
	}

	file.close();

	return UpdateMapRelateTable(xmlDoc);
}

bool MapForm::UpdateMapRelateTable(const QDomDocument& doc)
{
	if (doc.isNull())
	{
		qDebug() << "Loading xml file failed,Error:Empty file" << endl;
		return false;
	}

	QDomElement root = doc.documentElement(); //返回根节点

	if (root.isNull())
	{
		qDebug() << "Loading xml file failed,Error:Can not find root" << endl;

		return false;
	}

	QDomNode node = root.firstChild(); //获得第一个子节点
	if (node.isNull())
	{
		qDebug() << "Loading xml file failed,Error:Can not find child" << endl;

		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;
	// 清空要更新的表
	QStringList clearTableList;
	clearTableList.append("truncate table AGVDB_INFO_AGV");
	clearTableList.append("truncate table AGVDB_INFO_AGVTYPE");
	clearTableList.append("truncate table AGVDB_INFO_CALLER");
	clearTableList.append("truncate table AGVDB_INFO_CHARGER");
	clearTableList.append("truncate table AGVDB_INFO_REST");
	clearTableList.append("truncate table AGVDB_INFO_RESTQUEUE");
	clearTableList.append("truncate table AGVDB_INFO_RFID");
	clearTableList.append("truncate table AGVDB_INFO_TRACK");
	clearTableList.append("truncate table AGVDB_INFO_TRAFFIC");
	clearTableList.append("truncate table AGVDB_INFO_WORK");

	for (int i = 0; i < clearTableList.size(); i++)
	{
		query.exec(clearTableList[i]);
	}

	while (!node.isNull())  //如果节点不空
	{
		if (node.isElement() == false)
		{
			continue;
		}

		QDomElement e = node.toElement();

		QString elemtName(e.tagName()); // 元素名称

		// RFID地标卡
		if (elemtName == "RFID")
		{
			UpdateRFID(e);
		}
		// 工作站
		else if (elemtName == "Station")
		{
			UpdateStation(e);
		}
		// 待机站
		else if (elemtName == "Rest")
		{
			UpdateRest(e);
		}
		// 充电站
		else if (elemtName == "Charger")
		{
			UpdateCharger(e);
		}
		// 磁条
		else if (elemtName == "Track")
		{
			UpdateTrack(e);
		}
		// AGV
		else if (elemtName == "AGV")
		{
			UpdateAGV(e);
		}
		// 呼叫器
		else if (elemtName == "Caller")
		{
			UpdateCaller(e);
		}
		// AGV类型
		else if (elemtName == "AGVType")
		{
			UpdateAGVType(e);
		}
		// 待机队列
		else if (elemtName == "RestQueue")
		{
			UpdateRestQueue(e);
		}
		// 交通管制
		else if (elemtName == "Traffic")
		{
			UpdateTraffic(e);
		}

		node = node.nextSibling();
	}

	qDebug() << "Update map related table succeeded";

	return true;
}

bool MapForm::UpdateMapRelateTable(const QJsonDocument& doc)
{
	if (doc.isNull() || doc.isEmpty())
	{
		qDebug() << "Loading json file failed,Error:Empty file" << endl;
		return false;
	}

	QJsonObject obj = doc.object();

	if (obj.isEmpty())
	{
		qDebug() << "Loading json file failed,Error:Empty file" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;
	// 清空要更新的表
	QStringList clearTableList;
	clearTableList.append("truncate table AGVDB_INFO_AGV");
	clearTableList.append("truncate table AGVDB_INFO_AGVTYPE");
	clearTableList.append("truncate table AGVDB_INFO_CALLER");
	clearTableList.append("truncate table AGVDB_INFO_CHARGER");
	clearTableList.append("truncate table AGVDB_INFO_REST");
	clearTableList.append("truncate table AGVDB_INFO_RESTQUEUE");
	clearTableList.append("truncate table AGVDB_INFO_RFID");
	clearTableList.append("truncate table AGVDB_INFO_TRACK");
	clearTableList.append("truncate table AGVDB_INFO_TRAFFIC");
	clearTableList.append("truncate table AGVDB_INFO_WORK");

	for (int i = 0; i < clearTableList.size(); i++)
	{
		query.exec(clearTableList[i]);
	}

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.count(); ++i)
	{
		QString key = listKeys.at(i);

		// RFID地标卡
		if (key == "RFID")
		{
			UpdateRFID(obj.value(key).toObject());
		}
		// 工作站
		else if (key == "Station")
		{
			UpdateStation(obj.value(key).toObject());
		}
		// 待机站
		else if (key == "Rest")
		{
			UpdateRest(obj.value(key).toObject());
		}
		// 充电站
		else if (key == "Charger")
		{
			UpdateCharger(obj.value(key).toObject());
		}
		// 磁条
		else if (key == "Track")
		{
			UpdateTrack(obj.value(key).toArray());
		}
		// AGV
		else if (key == "AGV")
		{
			UpdateAGV(obj.value(key).toObject());
		}
		// 呼叫器
		else if (key == "Caller")
		{
			UpdateCaller(obj.value(key).toObject());
		}
		// AGV类型
		else if (key == "AGVType")
		{
			UpdateAGVType(obj.value(key).toObject());
		}
		// 待机队列
		else if (key == "RestQueue")
		{
			UpdateRestQueue(obj.value(key).toObject());
		}
		// 交通管制
		else if (key == "Traffic")
		{
			UpdateTraffic(obj.value(key).toObject());
		}
	}

	return true;
}

bool MapForm::UpdateAGV(const QDomElement& e)
{
	QString target = "AGV";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "AGV")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nNO = 0;				/*!< 编号 */
	QString strType = "";      /*!< 类型 */
	int nClient = 0;			/*!< 网络类型 */
	QString strIP = "";        /*!< IP地址 */
	int nPort = 0;				/*!< 端口 */
	QString strLocalIP = "";	/*!< 本地IP地址 */
	int nLocalPort = 0;		/*!< 本地端口 */

	nNO = e.attribute("No").toInt();
	strType = e.attribute("Type");
	nClient = e.attribute("Client").toInt();
	strIP = e.attribute("IP");
	nPort = e.attribute("Port").toInt();
	strLocalIP = e.attribute("LocalIP");
	nLocalPort = e.attribute("LocalPort").toInt();

	strSql = QString("INSERT INTO AGVDB_INFO_AGV (agv_id, agv_type, agv_client, agv_ip, agv_port,agv_locol_ip,agv_locol_port) VALUES (%1, '%2', %3, '%4', %5, '%6', %7)")
		.arg(nNO).arg(strType).arg(nClient).arg(strIP).arg(nPort).arg(strLocalIP).arg(nLocalPort);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateAGV(const QJsonObject& obj)
{
	QString target = "AGV";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);

		int nNO = 0;				/*!< 编号 */
		QString strType = "";      /*!< 类型 */
		int nClient = 0;			/*!< 网络类型 */
		QString strIP = "";        /*!< IP地址 */
		int nPort = 0;				/*!< 端口 */
		QString strLocalIP = "";	/*!< 本地IP地址 */
		int nLocalPort = 0;		/*!< 本地端口 */

		QJsonObject attr = obj.value(key).toObject();

		nNO = key.toInt();
		strType = attr.value("Type").toString();
		nClient = attr.value("Client").toBool();
		strIP = attr.value("IP").toString();
		nPort = attr.value("Port").toInt();
		strLocalIP = attr.value("LocalIP").toString();
		nLocalPort = attr.value("LocalPort").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_AGV (agv_id, agv_type, agv_client, agv_ip, agv_port,agv_locol_ip,agv_locol_port) VALUES (%1, '%2', %3, '%4', %5, '%6', %7)")
			.arg(nNO).arg(strType).arg(nClient).arg(strIP).arg(nPort).arg(strLocalIP).arg(nLocalPort);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateRFID(const QDomElement& e)
{
	QString target = "RFID";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "RFID")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nNO = 0;	// ID
	int nX = 0;		// X
	int nY = 0;		// Y

	nNO = e.attribute("No").toInt();
	nX = e.attribute("X").toInt();
	nY = e.attribute("Y").toInt();

	strSql = QString("INSERT INTO AGVDB_INFO_RFID (rfid_id, rfid_x, rfid_y) VALUES (%1, %2, %3)").arg(nNO).arg(nX).arg(nY);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateRFID(const QJsonObject& obj)
{
	QString target = "RFID";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		int nNO = 0;	// ID
		int nX = 0;		// X
		int nY = 0;		// Y

		nNO = key.toInt();
		nX = attr.value("X").toInt();
		nY = attr.value("Y").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_RFID (rfid_id, rfid_x, rfid_y) VALUES (%1, %2, %3)").arg(nNO).arg(nX).arg(nY);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateStation(const QDomElement& e)
{
	QString target = "Work station";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Station")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nNO = 0;				// ID
	QString strName("");		// 名称
	int nLocation = 0;			// 坐标	
	int nX = 0;					// X
	int nY = 0;					// Y

	nNO = e.attribute("No").toInt();
	nLocation = e.attribute("Location").toInt();
	strName = e.attribute("Name");
	nX = e.attribute("X").toInt();
	nY = e.attribute("Y").toInt();

	strSql = QString("INSERT INTO AGVDB_INFO_WORK (worker_id, worker_name, worker_location, worker_x, worker_y) VALUES (%1, '%2', %3, %4, %5)").arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateStation(const QJsonObject& obj)
{
	QString target = "Work station";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);

		int nNO = 0;				// ID
		QString strName("");		// 名称
		int nLocation = 0;			// 坐标	
		int nX = 0;					// X
		int nY = 0;					// Y

		QJsonObject attr = obj.value(key).toObject();

		nNO = key.toInt();
		nLocation = attr.value("Location").toInt();
		strName = attr.value("Name").toString();
		nX = attr.value("X").toInt();
		nY = attr.value("Y").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_WORK (worker_id, worker_name, worker_location, worker_x, worker_y) VALUES (%1, '%2', %3, %4, %5)").arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateRest(const QDomElement& e)
{
	QString target = "Rest station";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Rest")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nNO = 0;				// ID
	QString strName("");		// 名称
	int nLocation = 0;			// 坐标
	int nX = 0;					// X
	int nY = 0;					// Y

	nNO = e.attribute("No").toInt();
	nLocation = e.attribute("Location").toInt();
	strName = e.attribute("Name");
	nX = e.attribute("X").toInt();
	nY = e.attribute("Y").toInt();

	QDomNodeList list = e.childNodes();
	QJsonArray arr;

	for (int i = 0; i < list.count(); ++i)
	{
		QDomNode n = list.at(i);
		if (n.isElement() == false)
		{
			continue;
		}

		QDomElement element = n.toElement();

		arr.append(element.attribute("No").toInt());
	}

	strSql = QString("INSERT INTO AGVDB_INFO_REST (rester_id, rester_name, rester_location, rester_x, rester_y, rester_lock) VALUES (%1, '%2', %3, %4, %5, '%6')")
		.arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY).arg(QString(QJsonDocument(arr).toJson()));
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateRest(const QJsonObject& obj)
{
	QString target = "Rest station";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		int nNO = 0;				// ID
		QString strName("");		// 名称
		int nLocation = 0;			// 坐标
		int nX = 0;					// X
		int nY = 0;					// Y

		nNO = key.toInt();
		nLocation = attr.value("Location").toInt();
		strName = attr.value("Name").toString();
		nX = attr.value("X").toInt();
		nY = attr.value("Y").toInt();
		QJsonArray arr = attr.value("Lock").toArray();

		strSql = QString("INSERT INTO AGVDB_INFO_REST (rester_id, rester_name, rester_location, rester_x, rester_y, rester_lock) VALUES (%1, '%2', %3, %4, %5, '%6')")
			.arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY).arg(QString(QJsonDocument(arr).toJson()));
		query.exec(strSql);
	}

	return false;
}

bool MapForm::UpdateCharger(const QDomElement& e)
{
	QString target = "Charge station";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Charger")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nNO = 0;				// ID
	QString strName("");		// 名称
	int nLocation = 0;			// 坐标
	int nX = 0;					// X
	int nY = 0;					// Y

	nNO = e.attribute("No").toInt();
	nLocation = e.attribute("Location").toInt();
	strName = e.attribute("Name");
	nX = e.attribute("X").toInt();
	nY = e.attribute("Y").toInt();

	strSql = QString("INSERT INTO AGVDB_INFO_CHARGER (charger_id, charger_name, charger_location, charger_x, charger_y) VALUES (%1, '%2', %3, %4, %5)").arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateCharger(const QJsonObject& obj)
{
	QString target = "Charge station";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		int nNO = 0;				// ID
		QString strName("");		// 名称
		int nLocation = 0;			// 坐标
		int nX = 0;					// X
		int nY = 0;					// Y

		nNO = key.toInt();
		nLocation = attr.value("Location").toInt();
		strName = attr.value("Name").toString();
		nX = attr.value("X").toInt();
		nY = attr.value("Y").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_CHARGER (charger_id, charger_name, charger_location, charger_x, charger_y) VALUES (%1, '%2', %3, %4, %5)").arg(nNO).arg(strName).arg(nLocation).arg(nX).arg(nY);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateTrack(const QDomElement& e)
{
	QString target = "Track";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Track")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int nStartX = 0;			// 起始坐标X
	int nStartY = 0;			// 起始坐标Y
	int nEndX = 0;				// 终止坐标X
	int nEndY = 0;				// 终止坐标Y
	int nArc = 0;				// 磁轨类型

	nStartX = e.attribute("StartX").toInt();
	nStartY = e.attribute("StartY").toInt();
	nEndX = e.attribute("EndX").toInt();
	nEndY = e.attribute("EndY").toInt();
	nArc = e.attribute("Arc").toInt();

	strSql = QString("INSERT INTO AGVDB_INFO_TRACK (track_start_x, track_start_y, track_end_x, track_end_y, track_arc) VALUES (%1, %2, %3, %4, %5)").arg(nStartX).arg(nStartY).arg(nEndX).arg(nEndY).arg(nArc);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateTrack(const QJsonArray& obj)
{
	QString target = "Track";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	for (QJsonArray::const_iterator it = obj.begin(); it != obj.end(); ++it)
	{
		QJsonObject attr = it->toObject();

		int nStartX = 0;			// 起始坐标X
		int nStartY = 0;			// 起始坐标Y
		int nEndX = 0;				// 终止坐标X
		int nEndY = 0;				// 终止坐标Y
		int nArc = 0;				// 磁轨类型

		nStartX = attr.value("StartX").toInt();
		nStartY = attr.value("StartY").toInt();
		nEndX = attr.value("EndX").toInt();
		nEndY = attr.value("EndY").toInt();
		nArc = attr.value("Arc").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_TRACK (track_start_x, track_start_y, track_end_x, track_end_y, track_arc) VALUES (%1, %2, %3, %4, %5)").arg(nStartX).arg(nStartY).arg(nEndX).arg(nEndY).arg(nArc);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateCaller(const QDomElement& e)
{
	QString target = "Caller";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Caller")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	int nNO = 0;			// ID
	QString strAddr("");	// 地址
	int nStation = 0;		// 绑定工作站
	int enable = 0;			// 启用标识
	int nX = 0;				// X
	int nY = 0;				// Y

	nNO = e.attribute("No").toInt();
	nStation = e.attribute("Station").toInt();
	strAddr = e.attribute("Address");
	enable = e.attribute("Enable").toInt();
	nX = e.attribute("X").toInt();
	nY = e.attribute("Y").toInt();

	QSqlQuery query(m_db);
	QString strSql;

	strSql = QString("INSERT INTO AGVDB_INFO_CALLER (caller_id, caller_address, caller_bind_station, caller_enable, caller_x, caller_y) VALUES (%1, '%2', %3, %4, %5, %6)")
		.arg(nNO).arg(strAddr).arg(nStation).arg(enable).arg(nX).arg(nY);
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateCaller(const QJsonObject& obj)
{
	QString target = "Caller";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		int nNO = 0;			// ID
		QString strAddr("");	// 地址
		int nStation = 0;		// 绑定工作站
		int enable = 0;			// 启用标识
		int nX = 0;				// X
		int nY = 0;				// Y

		nNO = key.toInt();
		nStation = attr.value("Station").toInt();
		strAddr = attr.value("Address").toString();
		enable = attr.value("Enable").toInt();
		nX = attr.value("X").toInt();
		nY = attr.value("Y").toInt();

		strSql = QString("INSERT INTO AGVDB_INFO_CALLER (caller_id, caller_address, caller_bind_station, caller_enable, caller_x, caller_y) VALUES (%1, '%2', %3, %4, %5, %6)")
			.arg(nNO).arg(strAddr).arg(nStation).arg(enable).arg(nX).arg(nY);
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateAGVType(const QDomElement& e)
{
	QString target = "AGV type";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "AGVType")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QString name;
	QString brand;
	QString version;
	float maxSpeed;
	float maxWeight;
	int protocol;
	QJsonObject	action;

	name = e.attribute("Name");
	brand = e.attribute("Brand");
	version = e.attribute("Version");
	maxSpeed = e.attribute("MaxSpeed").toFloat();
	maxWeight = e.attribute("MaxWeight").toFloat();
	protocol = e.attribute("Protocol").toInt();

	QDomNodeList list = e.childNodes();

	for (int i = 0; i < list.count(); ++i)
	{
		QDomNode n = list.at(i);
		if (n.isElement() == false)
		{
			continue;
		}

		QDomElement element = n.toElement();

		action.insert(QString("%1").arg(element.attribute("No").toInt()), element.attribute("Name"));
	}

	strSql = QString("INSERT INTO AGVDB_INFO_AGVTYPE (type_name, type_brand, type_version, type_max_speed, type_max_weight, type_protocol,type_action) VALUES ('%1', '%2', '%3', %4, %5, %6, '%7')")
		.arg(name).arg(brand).arg(version).arg(maxSpeed).arg(maxWeight).arg(protocol).arg(QString(QJsonDocument(action).toJson()));
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateAGVType(const QJsonObject& obj)
{
	QString target = "AGV Type";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		QString name;
		QString brand;
		QString version;
		float maxSpeed;
		float maxWeight;
		int protocol;
		QJsonObject	action;

		name = key;
		brand = attr.value("Brand").toString();
		version = attr.value("Version").toString();
		maxSpeed = attr.value("MaxSpeed").toVariant().toFloat();
		maxWeight = attr.value("MaxWeight").toVariant().toFloat();
		protocol = attr.value("Protocol").toInt();
		action = attr.value("Action").toObject();

		strSql = QString("INSERT INTO AGVDB_INFO_AGVTYPE (type_name, type_brand, type_version, type_max_speed, type_max_weight, type_protocol,type_action) VALUES ('%1', '%2', '%3', %4, %5, %6, '%7')")
			.arg(name).arg(brand).arg(version).arg(maxSpeed).arg(maxWeight).arg(protocol).arg(QString(QJsonDocument(action).toJson()));
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateRestQueue(const QDomElement& e)
{
	QString target = "Rest queue";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "RestQueue")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int no;
	QJsonArray arr;

	no = e.attribute("No").toInt();

	QDomNodeList list = e.childNodes();

	for (int i = 0; i < list.count(); ++i)
	{
		QDomNode n = list.at(i);
		if (n.isElement() == false)
		{
			continue;
		}

		QDomElement element = n.toElement();

		arr.push_back(element.attribute("No").toInt());
	}

	strSql = QString("INSERT INTO AGVDB_INFO_RESTQUEUE (queue_id, queue_list) VALUES (%1, '%2')")
		.arg(no).arg(QString(QJsonDocument(arr).toJson()));
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateRestQueue(const QJsonObject& obj)
{
	QString target = "Rest queue";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);

		int no;
		QJsonArray arr;

		no = key.toInt();
		arr = obj.value(key).toArray();

		strSql = QString("INSERT INTO AGVDB_INFO_RESTQUEUE (queue_id, queue_list) VALUES (%1, '%2')")
			.arg(no).arg(QString(QJsonDocument(arr).toJson()));
		query.exec(strSql);
	}

	return true;
}

bool MapForm::UpdateTraffic(const QDomElement& e)
{
	QString target = "Traffic";

	if (e.isNull())
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is null" << endl;
		return false;
	}

	if (e.tagName() != "Traffic")
	{
		qDebug() << "Update" << target << "failed:Error:" << "Element is wrong name" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	int no;
	QJsonArray lockArray, compareArray;

	no = e.attribute("No").toInt();

	QDomNodeList list = e.childNodes();

	for (int i = 0; i < list.count(); ++i)
	{
		QDomNode n = list.at(i);
		if (n.isElement() == false)
		{
			continue;
		}

		QDomElement element = n.toElement();

		if (element.tagName() == "Lock")
		{
			lockArray.push_back(element.attribute("No").toInt());
		}
		else if (element.tagName() == "Compare")
		{
			compareArray.push_back(element.attribute("No").toInt());
		}
	}

	strSql = QString("INSERT INTO AGVDB_INFO_TRAFFIC (traffic_id, traffic_lock,traffic_compare) VALUES (%1, '%2','%3')")
		.arg(no).arg(QString(QJsonDocument(lockArray).toJson())).arg(QString(QJsonDocument(compareArray).toJson()));
	query.exec(strSql);

	return true;
}

bool MapForm::UpdateTraffic(const QJsonObject& obj)
{
	QString target = "Traffic";

	if (obj.isEmpty())
	{
		qDebug() << "Update" << target << "failed:Error:" << "JSON object is empty" << endl;
		return false;
	}

	QSqlQuery query(m_db);
	QString strSql;

	QStringList	listKeys = obj.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);
		QJsonObject attr = obj.value(key).toObject();

		int no;
		QJsonArray lockArray, compareArray;

		no = key.toInt();
		lockArray = attr.value("Lock").toArray();
		compareArray = attr.value("Compare").toArray();

		strSql = QString("INSERT INTO AGVDB_INFO_TRAFFIC (traffic_id, traffic_lock,traffic_compare) VALUES (%1, '%2','%3')")
			.arg(no).arg(QString(QJsonDocument(lockArray).toJson())).arg(QString(QJsonDocument(compareArray).toJson()));
		query.exec(strSql);
	}

	return true;
}

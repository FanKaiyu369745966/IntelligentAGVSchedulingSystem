#include "GraphicMap.h"

GraphicMap::GraphicMap(const QSqlDatabase& db, QWidget* parent)
	: m_db(db)
{
	m_pBg = std::make_shared<GraphicBg>();

	loadMapElement();
}

GraphicMap::~GraphicMap()
{
	// 保存地图缩放比例和偏移量
	QSqlQuery query(m_db);
	query.exec("truncate table AGVDB_INFO_BACKGROUND");
	query.exec(QString("INSERT INTO AGVDB_INFO_BACKGROUND (bg_scal, bg_offset_x, bg_offset_y) VALUES (%1, %2, %3)")
		.arg(GraphicBg::getScale()).arg(GraphicBg::getOffset().x()).arg(GraphicBg::getOffset().y()));

	// 清空地图元素
	clearMapElement();
}

void GraphicMap::clearMapElement()
{
	m_mutex.lock();

	m_mapAGVs.clear();

	m_mapRFIDs.clear();

	m_mapStations.clear();

	m_mapRests.clear();

	m_mapChargers.clear();

	m_mapCaller.clear();

	m_listTracks.clear();

	m_mutex.unlock();
}

QCursor GraphicMap::drag(QPoint ptWinPoint)
{
	QCursor hCursor;

	if (m_pBg)
	{
		hCursor = m_pBg->Drag(ptWinPoint);
	}

	return hCursor;
}

void GraphicMap::cancel()
{
	if (m_pBg)
	{
		m_pBg->Cancel();
	}
}

void GraphicMap::drawMap(QPainter& painter)
{
	if (m_mutex.try_lock() == false)
	{
		return;
	}

	if (m_pBg)
	{
		m_pBg->Draw(painter);
	}

	for (QList< std::shared_ptr<IconTrack>>::reverse_iterator it = m_listTracks.rbegin(); it != m_listTracks.rend(); ++it)
	{
		(*it)->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconRFID>>::iterator it = m_mapRFIDs.begin(); it != m_mapRFIDs.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconCS>>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconWS>>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconRS>>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconCaller>>::iterator it = m_mapCaller.begin(); it != m_mapCaller.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	for (QMap < quint32, std::shared_ptr<IconAGV>>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		it.value()->Draw(painter);
	}

	m_mutex.unlock();

	return;
}

bool GraphicMap::select(const BUTTONCTRL& nButton, const QPoint& point)
{
	if (nButton == BUTTONCTRL_LDOWN)
	{
		if (m_pBg)
		{
			m_pBg->Select(point);
		}
	}
	return true;
}

bool GraphicMap::confirm(const BUTTONCTRL& nButton, const QPoint& point)
{
	if (nButton == BUTTONCTRL_LUP)
	{
		if (m_pBg)
		{
			m_pBg->Confirm();
		}
	}
	return true;
}


void GraphicMap::wheel(const bool& bDire)
{
	m_mutex.lock();

	if (m_pBg)
	{
		m_pBg->Scale(bDire);
	}

	m_mutex.unlock();

	return;
}

void GraphicMap::size(const QRect& rect)
{
	m_mutex.lock();

	Graphic::setClientRect(rect);

	m_mutex.unlock();

	return;
}

void GraphicMap::showAgv(const quint32& unNo, const quint32& unCurMark, const bool& bAuto, const bool& bRun, const bool& bUnVol, const bool& bObs, const bool& bError)
{
	if (m_mapRFIDs.find(unCurMark) == m_mapRFIDs.end())
	{
		return;
	}

	if (m_mapAGVs.find(unNo) == m_mapAGVs.end())
	{
		m_mapAGVs[unNo] = std::make_shared<GraphicAGV>(unNo);
	}

	m_mapAGVs[unNo]->SetStatus(bAuto, bRun, bUnVol, bObs, bError);
	m_mapAGVs[unNo]->SetCenterPoint(m_mapRFIDs[unCurMark]->GetCenterPoint());

	m_mapAGVs[unNo]->Show(true);

	return;
}


void GraphicMap::hideAgv(const quint32& unNo)
{
	m_mutex.lock();

	if (m_mapAGVs.find(unNo) != m_mapAGVs.end())
	{
		m_mapAGVs[unNo]->Show(false);
	}

	m_mutex.unlock();

	return;
}

void GraphicMap::updateCallerSignal(const quint32& unNo, const bool& scream, const bool& call, const bool& pass, const bool& bCancel)
{
	if (m_mapCaller.find(unNo) == m_mapCaller.end())
	{
		return;
	}

	m_mapCaller[unNo]->SetSignal(scream, call, pass, bCancel);

	return;
}

void GraphicMap::updateCallerEnable(const quint32& unNo, const bool& enable)
{
	if (m_mapCaller.find(unNo) == m_mapCaller.end())
	{
		return;
	}

	m_mapCaller[unNo]->SetEnable(enable);

	return;
}


void GraphicMap::loadMapElement()
{
	clearMapElement();

	m_mutex.lock();

	if (m_pBg == nullptr)
	{
		m_pBg = std::make_shared<GraphicBg>();
	}

	QSqlQuery query(m_db);
	// 读取地图元素信息
	// 背景
	query.exec("SELECT * FROM AGVDB_INFO_BACKGROUND");
	while (query.next())
	{
		float fScale = query.value(0).toFloat();    // 缩放比例
		int nOffsetX = query.value(1).toInt();      // 偏移量X
		int nOffsetY = query.value(2).toInt();      // 偏移量Y

		m_pBg->setScale(fScale);
		m_pBg->setOffset(QPoint(nOffsetX, nOffsetY));
	}

	// 磁轨
	query.exec("SELECT * FROM AGVDB_INFO_TRACK");
	while (query.next())
	{
		int nStartx = query.value(0).toInt();   // 起点X坐标
		int nStarty = query.value(1).toInt();   // 起点Y坐标
		int nEndx = query.value(2).toInt();		// 终点X坐标
		int nEndy = query.value(3).toInt();     // 终点Y坐标
		bool bArc = query.value(4).toBool();    // 弧线标识

		std::shared_ptr<GraphicTrack> ptr = std::make_shared<GraphicTrack>(bArc);

		ptr->SetStartPoint(QPoint(nStartx, nStarty));
		ptr->SetEndPoint(QPoint(nEndx, nEndy));
		m_listTracks.push_back(ptr);
	}

	// RFID地标卡
	query.exec("SELECT * FROM AGVDB_INFO_RFID");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();
		int nX = query.value(1).toInt();
		int nY = query.value(2).toInt();

		m_mapRFIDs[unID] = std::make_shared<GraphicRFID>(unID);
		m_mapRFIDs[unID]->SetCenterPoint(QPoint(nX, nY));
	}

	// 工作站
	query.exec("SELECT worker_id,worker_name,worker_x,worker_y FROM AGVDB_INFO_WORK");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();				// 编号
		QString strName = query.value(1).toString();		// 名称
		int x = query.value(2).toInt();						// X坐标
		int y = query.value(3).toInt();						// Y坐标

		m_mapStations[unID] = std::make_shared<GraphicWorkStation>(unID, strName);
		m_mapStations[unID]->SetCenterPoint(QPoint(x, y));
	}

	// 待机站
	query.exec("SELECT rester_id,rester_name,rester_x,rester_y FROM AGVDB_INFO_REST");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();				// 编号
		QString strName = query.value(1).toString();		// 名称
		int x = query.value(2).toInt();						// X坐标
		int y = query.value(3).toInt();						// Y坐标

		m_mapRests[unID] = std::make_shared<GraphicRestStation>(unID, strName);
		m_mapRests[unID]->SetCenterPoint(QPoint(x, y));
	}

	// 充电站
	query.exec("SELECT charger_id,charger_name,charger_x,charger_y FROM AGVDB_INFO_CHARGER");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();				// 编号
		QString strName = query.value(1).toString();		// 名称
		int x = query.value(2).toInt();						// X坐标
		int y = query.value(3).toInt();						// Y坐标

		m_mapChargers[unID] = std::make_shared<GraphicChargeStation>(unID, strName);
		m_mapChargers[unID]->SetCenterPoint(QPoint(x, y));
	}

	// 呼叫器
	query.exec("SELECT caller_id,caller_enable,caller_x,caller_y FROM AGVDB_INFO_CALLER");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();				// 编号
		bool bEnable = static_cast<bool>(query.value(1).toInt());	// 启用标识
		int x = query.value(2).toInt();						// X坐标
		int y = query.value(3).toInt();						// Y坐标

		m_mapCaller[unID] = std::make_shared<IconCaller>(unID);
		m_mapCaller[unID]->SetCenterPoint(QPoint(x, y));
	}

	// AGV
	query.exec("SELECT agv_id FROM AGVDB_INFO_AGV");
	while (query.next())
	{
		quint32 unID = query.value(0).toUInt();				// 编号

		m_mapAGVs[unID] = std::make_shared<GraphicAGV>(unID);
	}

	m_mutex.unlock();

	return;
}

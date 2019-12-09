#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <QObject>
#include <QJsonDocument>
#include <QSqlDataBase>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include "AgvBase.h"
#include "Station.h"
#include "ZigbeeCaller.h"
#include "ZigbeeControler.h"
#include "TrafficPosition.h"
#include "EQLed.h"

class ServiceBase
	: public QObject
{
	Q_OBJECT
public:
	ServiceBase();
	explicit ServiceBase(const QSqlDatabase&);
	~ServiceBase();
protected:
	QSqlDatabase m_dbAGV;									/*!< AGV数据库 */
	QTcpServer m_server;									/*!< 网络服务端 */
	ZgbCtrler m_zgbCtrler;									/*!< Zigbee控制器 */
	std::shared_ptr<EQLed> m_led;							/*!< LED显示器 */
	std::shared_ptr<ProtocolStm32> m_protocolStm32;			/*!< STM32通信协议 */
	std::shared_ptr<ProtocolPlc> m_protocolPlc;				/*!< PLC通信协议 */
	AgvTypeArray m_mapAGVTypes;								/*!< AGV类型组(Key:类型名称) */
	AgvArray m_mapAGVs;										/*!< AGV组(Key:AGV编号) */
	RFIDArray m_mapRFIDs;									/*!< RFID地标卡组(Key:地标卡编号) */
	TrafficArray m_mapTraffics;								/*!< 交通管制组(Key:管制地标卡编号) */
	WorkerArray m_mapWorkers;								/*!< 工作站组(Key:工作站编号) */
	ResterArray m_mapResters;								/*!< 待机站组(Key:待机站编号) */
	RestQueueArray m_mapRestQueues;							/*!< 待机队列组(Key:待机队列编号) */
	ChargerArray m_mapChargers;								/*!< 充电站组(Key:充电站编号)*/
public:
	/*!
	 * @brief 读取AGV数据库配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVDBConfig(const QJsonDocument&);

	/*!
	 * @brief 读取AGV数据库配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVDBConfig(const QJsonObject&);

	/*!
	 * @brief 读取网络服务端配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadServerConfig(const QJsonDocument&);

	/*!
	 * @brief 读取网络服务端配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadServerConfig(const QJsonObject&);

	/*!
	 * @brief 读取LED配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadLedConfig(const QJsonDocument&);

	/*!
	 * @brief 读取LED配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadLedConfig(const QJsonObject&);

	/*!
	 * @brief 读取AGV类型配置信息
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVTypeConfig();

	/*!
	 * @brief 读取AGV类型配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVTypeConfig(const QJsonDocument&);

	/*!
	 * @brief 读取AGV类型配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVTypeConfig(const QJsonObject&);

	/*!
	 * @brief 读取AGV配置信息
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVConfig();

	/*!
	 * @brief 读取AGV配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVConfig(const QJsonDocument&);

	/*!
	 * @brief 读取AGV配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadAGVConfig(const QJsonObject&);

	/*!
	 * @brief 读取RFI地标卡配置信息
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRFIDConfig();

	/*!
	 * @brief 读取RFI地标卡配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRFIDConfig(const QJsonDocument&);

	/*!
	 * @brief 读取RFI地标卡配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRFIDConfig(const QJsonObject&);

	/*!
	 * @brief 读取RFI地标卡配置信息
	 *
	 * 从JSON数组中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonArray & 包含配置信息的JSON数组
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRFIDConfig(const QJsonArray&);

	/*!
	 * @brief 读取交通管制的配置
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadTrafficConfig();

	/*!
	 * @brief 读取交通管制的配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument &  包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadTrafficConfig(const QJsonDocument&);

	/*!
	 * @brief 读取交通管制的配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadTrafficConfig(const QJsonObject&);

	/*!
	 * @brief 读取待机站配置
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestConfig();

	/*!
	 * @brief 读取待机站配置
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestConfig(const QJsonDocument&);

	/*!
	 * @brief 读取待机站配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestConfig(const QJsonObject&);

	/*!
	 * @brief 读取待机队列配置
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestQueueConfig();

	/*!
	 * @brief 读取待机队列配置
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestQueueConfig(const QJsonDocument&);

	/*!
	 * @brief 读取待机队列配置
	 *
	 * ServiceBase::LoadRestQueueConfig
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadRestQueueConfig(const QJsonObject&);

	/*!
	 * @brief 读取工作站配置
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadWorkConfig();

	/*!
	 * @brief 读取工作站配置
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadWorkConfig(const QJsonDocument&);

	/*!
	 * @brief 读取工作站配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadWorkConfig(const QJsonObject&);

	/*!
	 * @brief 读取充电站配置
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadChargeConfig();

	/*!
	 * @brief 读取充电站配置
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadChargeConfig(const QJsonDocument&);

	/*!
	 * @brief 读取充电站配置
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadChargeConfig(const QJsonObject&);

	/*!
	 * @brief 读取Zigbee控制器配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadZgbeCtrlConfig(const QJsonDocument&);

	/*!
	 * @brief 读取Zigbee控制器配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadZgbeCtrlConfig(const QJsonObject&);

	/*!
	 * @brief 读取Zigbee呼叫器配置信息
	 *
	 * 从AGV数据库表中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadZigbeeCaller();

	/*!
	 * @brief 读取Zigbee呼叫器配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadZigbeeCaller(const QJsonDocument&);

	/*!
	 * @brief 读取Zigbee呼叫器配置信息
	 *
	 * 从JSON对象中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadZigbeeCaller(const QJsonObject&);
protected:
	/*!
	 * @brief 获取指向AGV类型的指针
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 类型名称
	 * @return AgvTypePtr 指向AGV类型的智能指针
	 */
	AgvTypePtr GetAGVTypePtr(const QString&) const;

	/*!
	 * @brief 获取指向AGV的指针
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const Agv::id_t & AGV编号
	 * @returnAgvPtr 指向AGV的智能指针
	 */
	AgvPtr GetAGVPtr(const id_t&) const;

	/*!
	 * @brief 获取RFID地标卡对象
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return const RFID 返回RFID地标卡对象
	 */
	const RFID GetRFID(const rfid_t&) const;

	/*!
	 * @brief 获取RFID地标卡对象指针
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return RFIDPtr 指向RFID地标卡的智能指针
	 */
	RFIDPtr GetRFIDPtr(const rfid_t&) const;

	/*!
	 * @brief 获取交通管制点对象
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & 交通管制点坐标
	 * @return TrafficPos 交通管制点对象
	 */
	TrafficPos GetTrafficPosition(const rfid_t&) const;

	/*!
	 * @brief 获取待机站指针
	 *
	 * 通过待机站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 待机站编号
	 * @return ResterPtr
	 */
	ResterPtr restStationPtr(const unsigned short&) const;

	/*!
	 * @brief 获取待机站指针
	 *
	 * 通过待机站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 待机站名称
	 * @return ResterPtr
	 */
	ResterPtr GetRestStationPtr(const QString&) const;

	/*!
	 * @brief 获取待机站指针
	 *
	 * 通过待机站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return ResterPtr
	 */
	ResterPtr GetRestStationPtr(const rfid_t&) const;

	/*!
	 * @brief 获取待机站对象
	 *
	 * 通过待机站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 待机站编号
	 * @return const RestStation
	 */
	const RestStation restStation(const unsigned short&) const;

	/*!
	 * @brief 获取待机站对象
	 *
	 * 通过待机站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 待机站名称
	 * @return const RestStation
	 */
	const RestStation GetRestStation(const QString&) const;

	/*!
	 * @brief 获取待机站对象
	 *
	 * 通过待机站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return const RestStation
	 */
	const RestStation GetRestStation(const rfid_t&) const;

	/*!
	 * @brief 获取待机队列
	 *
	 * 通过待机队列编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 待机站编号
	 * @return const RestQueue
	 */
	const RestQueue restQueue(const unsigned short&) const;

	/*!
	 * @brief 获取待机队列
	 *
	 * 通过待机站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 待机站编号
	 * @return const RestQueue
	 */
	const RestQueue GetRestQueue(const unsigned short&) const;

	/*!
	 * @brief 获取工作站指针
	 *
	 * 通过工作站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @return WorkerPtr
	 */
	WorkerPtr workStationPtr(const unsigned short&) const;

	/*!
	 * @brief 获取工作站指针
	 *
	 * 通过工作站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 工作站名称
	 * @return WorkerPtr
	 */
	WorkerPtr GetWorkStationPtr(const QString&) const;

	/*!
	 * @brief 获取工作站指针
	 *
	 * 通过工作站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return WorkerPtr
	 */
	WorkerPtr GetWorkStationPtr(const rfid_t&) const;

	/*!
	 * @brief 获取工作站对象
	 *
	 * 通过工作站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @return const WorkStation
	 */
	const WorkStation workStation(const unsigned short&) const;

	/*!
	 * @brief 获取工作站对象
	 *
	 * 通过工作站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 工作站名称
	 * @return const WorkStation
	 */
	const WorkStation GetWorkStation(const QString&) const;

	/*!
	 * @brief 获取工作站对象
	 *
	 * 通过工作站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return const WorkStation
	 */
	const WorkStation GetWorkStation(const rfid_t&) const;

	/*!
	 * @brief 获取充电站指针
	 *
	 * 通过充电站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 充电站编号
	 * @return std::shared_ptr	<ChargeStation>
	 */
	ChargerPtr chargeStationPtr(const unsigned short&) const;

	/*!
	 * @brief 获取充电站指针
	 *
	 * 通过充电站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 充电站名称
	 * @return std::shared_ptr	<ChargeStation>
	 */
	ChargerPtr GetChargeStationPtr(const QString&) const;

	/*!
	 * @brief 获取充电站指针
	 *
	 * 通过充电站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return std::shared_ptr	<ChargeStation>
	 */
	ChargerPtr GetChargeStationPtr(const rfid_t&) const;

	/*!
	 * @brief 获取充电站对象
	 *
	 * 通过充电站编号获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const unsigned short & 充电站编号
	 * @return const ChargeStation
	 */
	const ChargeStation chargeStation(const unsigned short&) const;

	/*!
	 * @brief 获取充电站对象
	 *
	 * 通过充电站名称获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 充电站名称
	 * @return const ChargeStation
	 */
	const ChargeStation GetChargeStation(const QString&) const;

	/*!
	 * @brief 获取充电站对象
	 *
	 * 通过充电站坐标获取
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const rfid_t & RFID地标卡编号
	 * @return const ChargeStation
	 */
	const ChargeStation GetChargeStation(const rfid_t&) const;
protected:
	/*!
	 * @brief 读取AGV状态
	 *
	 * 读取全部AGV状态
	 * @date 2019/11/22
	 * @author FanKaiyu
	 */
	void LoadAGVStatus();

	/*!
	 * @brief 读取AGV状态
	 *
	 * 读取单一AGV状态
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param Agv & 要读取的AGV对象
	 */
	void LoadAGVStatus(Agv& agv);

	/*!
	 * @brief 更新AGV状态
	 *
	 * 更新全部AGV状态
	 * @date 2019/11/22
	 * @author FanKaiyu
	 */
	void UpdateAGVStatus() const;

	/*!
	 * @brief 更新AGV状态
	 *
	 * 更新单一AGV状态
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const Agv & 要更新的AGV对象
	 */
	void UpdateAGVStatus(const Agv&) const;

	/*!
	 * @brief 更新连接状态
	 *
	 * 更新全部AGV连接状态
	 * @date 2019/12/08
	 * @author FanKaiyu
	 */
	void UpdateAGVConnection() const;

	/*!
	 * @brief 更新连接状态
	 *
	 * 更新单一AGV连接状态
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const Agv & 要更新的AGV对象
	 */
	void UpdateAGVConnection(const Agv&) const;

	/*!
	 * @brief 更新AGV图标显示
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const Agv & 要显示的AGV对象
	 */
	void ShowAGV(const Agv&);

	/*!
	 * @brief 为待机站绑定队列
	 *
	 * 为所有队列内的待机站绑定队列
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param RestQueue & 待机队列对象
	 */
	void BindRestQueue();

	/*!
	 * @brief 为待机站绑定队列
	 *
	 * 为单一队列内的待机站绑定队列
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param RestQueue & 待机队列对象
	 */
	void BindRestQueue(const RestQueue&);

	/*!
	 * @brief 读取呼叫器信号状态
	 *
	 * 读取全部呼叫器信号状态
	 * @date 2019/11/25
	 * @author FanKaiyu
	 */
	void LoadCallerSignal();

	/*!
	 * @brief 读取呼叫器信号状态
	 *
	 * 读取单一呼叫器状态
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param ZigbeeCaller & 要读取的呼叫器对象
	 */
	void LoadCallerSignal(const ZigbeeCaller& caller);

	/*!
	 * @brief 更新呼叫器信号
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @param const CallerAttr & 呼叫器属性
	 */
	void UpdateCallerSignal(const unsigned short&, const CallerAttr&);

	/*!
	 * @brief 抛出异常
	 * @date 2019/12/01
	 * @author FanKaiyu
	 * @param const QString & 异常来源
	 * @param const QString & 异常描述
	 * @param const int & 异常等级
	 */
	void ThrowError(const QString&, const QString&, const int&) const;

	/*!
	 * @brief 清除异常
	 * @date 2019/12/01
	 * @author FanKaiyu
	 * @param const QString & 异常来源
	 * @param const QString & 异常描述
	 */
	void ClearError(const QString&, const QString&) const;

	/*!
	 * @brief 清除异常
	 * @date 2019/12/01
	 * @author FanKaiyu
	 * @param const QString & 异常来源
	 * @param const int & 异常等级
	 */
	void ClearError(const QString&, const int&) const;

	/*!
	 * @brief 清除全部异常
	 * @date 2019/12/01
	 * @author FanKaiyu
	 */
	void ClearError() const;
signals:
	/*!
	 * @brief 服务器状态改变时发出此信号
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param QString 描述服务器当前状态的字符串
	 */
	void signalServerStateChange(QString);

	/*!
	 * @brief 通知界面显示AGV图标时发出此信号
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param quint32 AGV编号
	 * @param quint32 AGV当前RFID地标卡坐标
	 * @param bool 模式标识
	 * @param bool 移动标识
	 * @param bool 电量不足标识
	 * @param bool 避障标识
	 * @param bool 异常标识
	 */
	void signalShowAgv(quint32, quint32, bool, bool, bool, bool, bool);

	/*!
	 * @brief 通知界面隐藏AGV图标时发出此信号
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param quint32 AGV编号
	 */
	void signalHideAgv(quint32);

	/*!
	 * @brief 通知界面更新呼叫器启用状态时发出此信号
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param int 呼叫器编号
	 * @param bool 启用状态
	 */
	void signalUpdateCallerUse(int, bool);

	/*!
	 * @brief 通知界面更新呼叫器信号状态时发出此信号
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param quint32
	 * @param bool 急停信号
	 * @param bool 呼叫信号
	 * @param bool 放行信号
	 * @param bool 取消信号
	 */
	void signalUpdateCallerSignal(quint32, bool, bool, bool, bool);

	void signalUpdateAGVStatus(const unsigned short&);
public slots:
	/*!
	 * @brief 接收AGV已连接信号时触发此槽函数
	 * @date 2019/11/22
	 * @author FanKaiyu
	 */
	void slotAGVConnected(const unsigned short&);

	/*!
	 * @brief 接收AGV连接中断信号时触发此槽函数
	 * @date 2019/11/22
	 * @author FanKaiyu
	 */
	void slotAGVConnectionBreak(const unsigned short&);

	/*!
	 * @brief 接收AGV更新信号时触发此槽函数
	 * @date 2019/11/22
	 * @author FanKaiyu
	 */
	void slotUpdateAGV(const unsigned short&);
};

#endif // SERVICEBASE_H

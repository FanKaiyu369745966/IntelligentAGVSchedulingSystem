/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file Service.h
 *
 * @brief 描述程序业务属性以及功能的文件
 *
 * @date 2019/11/17
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */

#ifndef SERVICE_H
#define SERVICE_H

#include <QThread>
#include "ServiceBase.h"
#include "TaskBase.h"

 /*!
  * @class Service
  * @brief 描述程序属性以及功能的类
  * @date 2019/11/17
  * @author FanKaiyu
  */
class Service
	: public ServiceBase
{
	Q_OBJECT
public:
	explicit Service(const QSqlDatabase& db);
	~Service();
protected:
	bool m_bAllScream;				/*!< 急停标识 */
	QThread m_thread;
	QTimer m_timer;
	QTimer m_clock;
	std::chrono::steady_clock::time_point m_ptCount;
public:
	/*!
	 * @brief 启动业务
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @return bool 启动成功返回true,否则返回false
	 */
	bool Start(const QJsonDocument&);
	bool Start(const QJsonDocument&, const QJsonDocument&);

	/*!
	 * @brief 退出业务
	 * @date 2019/11/25
	 * @author FanKaiyu
	 */
	void Exit();

protected:
	/*!
	 * @brief 初始化
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonDocument &
	 * @return void
	 */
	void Initialize(const QJsonDocument&);

	/*!
	 * @brief 读取配置信息
	 *
	 * 从数据库中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadConfig();

	/*!
	 * @brief 读取配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonDocumet &
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadConfig(const QJsonDocument&);

	/*!
	 * @brief 启动业务功能
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @return bool 启动成功返回true,否则返回false
	 */
	bool Open();

protected:
	/*!
	 * @brief AGV连接
	 * @return bool 连接成功返回true,否则返回false
	*/
	bool ConnectAgv(QTcpSocket*);

	/*!
	 * @brief 判断是否上锁
	 * @return bool 被锁定返回true,否则返回false
	*/
	bool IsLocked(const LocatArray&);

	/*!
	 * @brief 急停控制
	*/
	void ScreamControl();

	/*!
	 * @brief 交通管制控制
	*/
	void TrafficControl();

	/*!
	 * @brief 交通管制控制
	 * @param const RFID::rfid& 交通管制控制点坐标
	*/
	void TrafficControl(const rfid_t&);
	void TrafficControl(const RFID&);

	/*!
	 * @brief 交通管制控制
	 * @param const TrafficPos& 交通管制控制点坐标
	*/
	void TrafficControl(const TrafficPos&);

	/*!
	 * @brief 交通管制点对比
	 * @return bool 管制点可通行返回true,否则返回false
	*/
	bool TrafficCompare(const rfid_t&, LocatArray&);
	bool TrafficCompare(const RFID&, LocatArray&);

	/*!
	 * @brief 待机站控制
	 * @date 2019/11/27
	 * @author FanKaiyu
	 */
	void RestStationControl();
	void RestStationControl(const RestStation&);

	/*!
	 * @brief 待机队列控制
	*/
	void RestQueueControl();

	/*!
	 * @brief 待机队列控制
	 * @param const RestQueue& 待机队列
	*/
	void RestQueueControl(const RestQueue&);

	/*!
	 * @brief 待机队列控制
	 * @param const unsigned short& 当前待机站编号
	 * @param const unsigned short& 上一待机站编号
	*/
	void RestQueueControl(const unsigned short& cur, const unsigned short& prev);

	/*!
	 * @brief 待机队列控制
	 * @param const unsigned short& 当前待机站对象
	 * @param const unsigned short& 上一待机站对象
	*/
	void RestQueueControl(const RestStation& cur, const RestStation& prev);

	/*!
	 * @brief 返回控制
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param Agv & 需要返回的AGV
	 */
	void ReturnControl(Agv&);

	/*!
	 * @brief 返回待机队列
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param Agv & 需要返回的AGV
	 * @return bool	存在返回点返回true,否则返回false
	 */
	bool ReturnToRestQueue(Agv&);

	/*!
	 * @brief 返回待机站
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param Agv & 需要返回的AGV
	 * @return bool 存在返回点返回true,否则返回false
	 */
	bool ReturnToRestStation(Agv&);

	/*!
	 * @brief 返回充电站
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param Agv & 需要返回的AGV
	 * @return bool 存在返回点返回true,否则返回false
	 */
	bool ReturnToChargeStation(Agv&);

protected:
	/*!
	 * @brief 更新呼叫器信号状态
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const ZigbeeCaller & 需要更新的呼叫器
	 */
	void UpdateCallerSignal(const ZigbeeCaller& caller);

	/*!
	 * @brief 更新呼叫器的启用状态
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const ZigbeeCaller & 需要更新的呼叫器
	 */
	void UpdateCallerUse(const ZigbeeCaller& caller);
protected:
	/*!
	 * @brief 发布订单
	 *
	 * 通过呼叫器发布订单
	 * @date 2019/11/26
	 * @author FanKaiyu
	 * @param const ZgbCaller & 发出呼叫信号的呼叫器
	 */
	void PublishOrder(const ZgbCaller&);

	/*!
	 * @brief 发布订单
	 *
	 * Service::PublishOrder
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const WorkerPtr & 上料工位指针
	 * @param const WorkerPtr & 下料工位指针
	 * @param const QString & 备注信息
	 */
	void PublishOrder(const WorkerPtr& pLoad, const WorkerPtr& pUnload, const QString& log);

	/*!
	 * @brief 发布任务
	 * @date 2019/11/26
	 * @author FanKaiyu
	 * @param const QString & 订单号
	 * @param const QString & 工作站名称
	 * @param const QString & 任务内容
	 * @param const QString & 任务备注
	 * @return bool 发布成功返回true,否则返回false
	 */
	bool PublishTask(const QString& order, const QString& target, const QString& mission, const QString& log = "");

	/*!
	 * @brief 更新订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const QString & order
	 * @return bool 更新成功返回true,否则返回false
	 */
	bool UpdateOrder(const QString& order, const QString& tray, const int& agv);

	/*!
	 * @brief UpdateTask
	 *
	 * Service::UpdateTask
	 * @date 2019/11/26
	 * @author FanKaiyu
	 * @param const int & 任务号
	 * @param const QString & 任务状态
	 * @return bool 发布成功返回true,否则返回false
	 */
	bool UpdateTask(const int& no, const QString& status);

	/*!
	 * @brief 取消订单
	 * @date 2019/11/26
	 * @author FanKaiyu
	 * @param const QString & 订单号
	 * @return bool 取消成功返回true,否则返回false
	 */
	bool CancelOrder(const QString& order);

	/*!
	 * @brief 取消订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const ZgbCaller & 发出取消信号的呼叫器
	 * @return bool 取消成功返回true,否则返回false
	 */
	bool CancelOrder(const ZgbCaller&);

	/*!
	 * @brief 获取订单号
	 *
	 * 通过备注信息获取
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const QString & 备注信息
	 * @return QString
	 */
	QString GetOrderNumber(const QString&);

	/*!
	 * @brief 获取订单号
	 *
	 * 通过任务号获取
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 任务号
	 * @return QString
	 */
	QString GetOrderNumber(const int&);

	/*!
	 * @brief 执行订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param Order &
	 * @return bool 执行完成返回true,否则返回false
	 */
	bool ExecuteOrder(Order&);

	/*!
	 * @brief 执行任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const TaskBase & 待执行的任
	 * @return bool 执行完成返回true,否则返回false
	 */
	bool ExecuteTask(const TaskBase&);

	/*!
	 * @brief 抛出异常
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 任务号
	 * @param const QString & 异常信息
	 */
	void ThrowError(const int&, const QString&) const;
	void ThrowError(const TaskBase&, const QString&) const;

	/*!
	 * @brief 清除异常
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 任务号
	 */
	void ClearError(const int&) const;
	void ClearError(const TaskBase&) const;

	/*!
	 * @brief 获取订单
	 *
	 * 获取全部订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return OrderMap
	 */
	OrderMap GetOrder();

	/*!
	 * @brief 获取订单
	 *
	 * 获取单一订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const QString & 订单号
	 * @return Order
	 */
	Order GetOrder(const QString&);

	/*!
	 * @brief 判断AGV是否已经被分配订单
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const OrderMap & 订单列表
	 * @param const int & AGV编号
	 * @return bool 被分配返回true,否则返回false
	 */
	bool HaveOrder(const OrderMap&, const int&);

	/*!
	 * @brief 订单管理器
	 * @date 2019/11/27
	 * @author FanKaiyu
	 */
	void OrderManage();

	/*!
	 * @brief 获取可用于分配订单的AGV列表
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const OrderMap & 订单列表
	 * @return std::list<int>
	 */
	std::list<int> GetFreeAGVList(const OrderMap&);

	/*!
	 * @brief 判断地标卡是否在待机队列
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const rfid_t &
	 * @return bool
	 */
	bool IsInRestQueue(const rfid_t&) const;

	/*!
	 * @brief 判断地标卡是否为队列首地标
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const rfid_t &
	 * @return bool
	 */
	bool IsRestQueueHead(const rfid_t&) const;

	/*!
	 * @brief 判断地标卡是否在待机站
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const rfid_t &
	 * @return bool
	 */
	bool IsInRestStation(const rfid_t&) const;

	/*!
	 * @brief 判断地标卡是否在充电站
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const rfid_t &
	 * @return bool
	 */
	bool IsInChargeStation(const rfid_t&) const;

	/*!
	 * @brief 获取目标点名称
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param const rfid_t & 目标点坐标
	 * @return QString 目标点名称
	 */
	QString GetTargetName(const rfid_t&) const;
public:
	/*!
	 * @brief AGV控制
	 * @date 2019/11/17
	 * @author FanKaiyu
	 * @param const AgvBase::id_t & AGV编号
	 * @param const AgvControl & AGV控制类型
	 * @param const rfid_t & 地标卡编号
	 * @param const QString & 动作名称
	 * @param const AgvAttr::speed_t & 速度(%)
	 * @return CmdError
	 */
	CmdError AgvControl(const id_t&, const AgvCtrlType&, const rfid_t & = 0, const QString & = "", const AgvAttr::speed_t & = 0);

	/*!
	 * @brief 启用全部呼叫器
	*/
	void EnableUseAllCaller() { return m_zgbCtrler.EnableUseAllCaller(); }
	void EnableUseCaller(const unsigned short& id) { return m_zgbCtrler.EnableUseCaller(id); }
	void EnableUseCaller(ZigbeeCaller& caller) { return m_zgbCtrler.EnableUseCaller(caller); }

	/*!
	 * @brief 禁用全部呼叫器
	*/
	void DisableUseAllCaller() { return m_zgbCtrler.DisableUseAllCaller(); }
	void DisableUseCaller(const unsigned short& id) { return m_zgbCtrler.DisableUseCaller(id); }
	void DisableUseCaller(ZigbeeCaller& caller) { return m_zgbCtrler.DisableUseCaller(caller); }

signals:
	/*!
	 * @brief 呼叫器数据更新时发出的信号
	 * @date 2019/12/08
	 * @author FanKaiyu
	 */
	void signalUpdateCallerData();

	/*!
	 * @brief 串口状态发生改变时发出的信号
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param QString 串口状态描述
	 */
	void signalSerialPortStateChange(QString strOpenDesc);
public slots:
	/*!
	 * @brief 有新的客户端接入时触发的槽函数
	*/
	void slotNewConnection();

	/*!
	 * @brief 服务端中断时触发的槽函数
	*/
	void slotServerError(QAbstractSocket::SocketError);

	/*!
	 * @brief 呼叫器信号更新时触发的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 */
	void slotUpdateCallerSignal(const unsigned short&);

	/*!
	 * @brief 呼叫器启用状态更新时触发的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 */
	void slotUpdateCallerEnableUse(const unsigned short&);

	/*!
	 * @brief 串口状态发送改变时触发的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param QString 串口状态描述
	 */
	void slotSerialPortStateChange(QString strOpenDesc);

	/*!
	 * @brief 线程执行的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 */
	void slotThread();

	/*!
	 * @brief 更改呼叫器启用状态的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param int 呼叫器编号
	 * @param bool 更改后的启用状态
	 */
	void slotCallerEnable(int id, bool enable);

	/*!
	 * @brief 发布订单的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 * @param QString 目标工作站
	 * @param QString 任务内容
	 */
	void slotPublishOrder(QString station, QString mission);

	/*!
	 * @brief 更新LED时钟的槽函数
	 * @date 2019/12/08
	 * @author FanKaiyu
	 */
	void slotUpdateClock();

	void slotUpdateAGVStatus(const unsigned short&);
};

#endif // SERVICE_H

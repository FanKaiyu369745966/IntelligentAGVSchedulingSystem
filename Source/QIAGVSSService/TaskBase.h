/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file TaskBase.h
 *
 * @brief
 *
 * @date 2019/11/17
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef TASKBASE_H
#define TASKBASE_H

#include <QString>	
#include <string>
#include <chrono>
#include <ctime>
#include <map>
#include <list>
#include "AgvBase.h"
#include "WorkStation.h"

class TaskBase;
class Order;

typedef	std::map<int, TaskBase> TaskMap;
typedef	std::map<QString, Order> OrderMap;

static QString CreateOrderNumber();

static const QString TASK_STA_DEFAULT = QString::fromLocal8Bit("未执行");	/*!< 默认的任务执行状态 */
static const QString TASK_STA_START = QString::fromLocal8Bit("开始执行");
static const QString TASK_STA_FINISH = QString::fromLocal8Bit("执行完成");
static const QString TASK_STA_CANCEL = QString::fromLocal8Bit("取消订单");
static const QString TASK_ERR_DEFAULT = QString::fromLocal8Bit("正常");		/*!< 默认的任务异常信息 */

static const QString TASK_GETTRAY = QString::fromLocal8Bit("取回空托盘");
static const QString TASK_TOLIB = QString::fromLocal8Bit("取成品入库");
static const QString TASK_TOSTATION = QString::fromLocal8Bit("取成品至热处理车间");
static const QString TASK_PUTBLANK = QString::fromLocal8Bit("运送毛坯料");
static const QString TASK_PUTTRAY = QString::fromLocal8Bit("运送空托盘");
static const QString TASK_PUTBLANK1 = QString::fromLocal8Bit("运送毛坯料至CNC");
static const QString TASK_PUTBLANK2 = QString::fromLocal8Bit("运送毛坯料至磨齿机1");
static const QString TASK_PUTBLANK3 = QString::fromLocal8Bit("运送毛坯料至磨齿机2");
static const QString TASK_PUTTRAY1 = QString::fromLocal8Bit("运送空托盘至滚齿机");
static const QString TASK_PUTTRAY2 = QString::fromLocal8Bit("运送空托盘至磨齿机1");
static const QString TASK_PUTTRAY3 = QString::fromLocal8Bit("运送空托盘至磨齿机2");


static const QString TASK_MISSION_LOAD = QString::fromLocal8Bit("上料");
static const QString TASK_MISSION_UNLOAD = QString::fromLocal8Bit("下料");
static const QString TASK_MISSION_WAIT = QString::fromLocal8Bit("停靠");
static const QString TASK_MISSION_FINISH = QString::fromLocal8Bit("结束");

static const QString TASK_DEFAULT_TRAYLIB = QString::fromLocal8Bit("托盘库");
static const QString TASK_DEFAULT_BLANKLIB = QString::fromLocal8Bit("毛坯库");
static const QString TASK_DEFAULT_PRODUCTLIB = QString::fromLocal8Bit("成品库");
static const QString TASK_DEFAULT_PRODUCTSTA = QString::fromLocal8Bit("热处理车间");
static const QString TASK_DEFAULT_STAHEAD1 = QString::fromLocal8Bit("CNC上料口");
static const QString TASK_DEFAULT_STAHEAD2 = QString::fromLocal8Bit("磨齿机1上料口");
static const QString TASK_DEFAULT_STAHEAD3 = QString::fromLocal8Bit("磨齿机2上料口");
static const QString TASK_DEFAULT_STATAIL1 = QString::fromLocal8Bit("滚齿机下料口");
static const QString TASK_DEFAULT_STATAIL2 = QString::fromLocal8Bit("磨齿机1下料口");
static const QString TASK_DEFAULT_STATAIL3 = QString::fromLocal8Bit("磨齿机2下料口");

/*!
 * @class
 * @brief
 * @date 2019/11/17
 * @author FanKaiyu
 */
class TaskBase
{
public:
	TaskBase();
	explicit TaskBase(const QString&, const int&, const QString&, const WorkerPtr&, const QString&, const AgvPtr & = nullptr, const QString & = TASK_STA_DEFAULT, const QString & = TASK_ERR_DEFAULT, const QString & = "");
	TaskBase(const TaskBase&);
public:
	void operator = (const TaskBase&);
	bool IsNull() const;
public:
	QString m_order;	/*!< 订单号 一组由时间日期组成的字符串时间精确至毫秒如“020191126084700123” 共18位 */
	int m_task;			/*!< 任务号 */
	QString m_tray;		/*!< 托盘号 */
	WorkerPtr m_target;	/*!< 目标工站 */
	QString m_mission;	/*!< 任务内容 */
	AgvPtr m_executer;	/*!< 任务执行者 */
	QString m_status;	/*!< 任务状态 */
	//QString m_publish;	/*!< 下单时间 */
	//QString m_execute;	/*!< 执行时间 */
	//QString m_finish;	/*!< 完成时间 */
	QString m_error;	/*!< 异常信息 */
	QString m_log;		/*!< 备注信息 */
};

class Order
{
public:
	Order();
	explicit Order(const QString&, const QString & = "", const AgvPtr & = nullptr);
	Order(const Order&);
public:
	void operator=(const Order&);
	bool IsNull() const;
protected:
	QString m_no;		/*!< 订单号 */
	QString m_tray;		/*!< 托盘码 */
	AgvPtr m_executer;	/*!< 任务执行者 */
	TaskMap m_mapTasks;	/*!< 任务组 */

	std::list<int> m_list;	/*!< 订单任务的顺序 */
	int m_count;
public:
	/*!
	 * @brief 获取订单号
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return QString
	 */
	QString GetNo() const { return m_no; }

	/*!
	 * @brief 获取托盘码
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return QString
	 */
	QString GetTray() const { return m_tray; }

	/*!
	 * @brief 获取执行者编号
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return int 执行任务的AGV编号
	 */
	int GetExecuterNo() const;

	/*!
	 * @brief 获取执行者指针
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return AgvPtr 执行任务的AGV指针
	 */
	AgvPtr GetExecuter() const { return m_executer; }

	/*!
	 * @brief 设置托盘码
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const QString &
	 */
	void SetTray(const QString& tray) { m_tray = tray; return; }

	/*!
	 * @brief 设置执行者
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const AgvPtr &
	 */
	void SetExecuter(const AgvPtr& executer) { m_executer = executer; return; }
public:
	/*!
	 * @brief 插入任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const TaskBase &
	 * @return bool 插入成功返回true,否则返回false
	 */
	bool Insert(const TaskBase&);

	/*!
	 * @brief 删除任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 任务编号
	 * @return bool 删除成功返回true,否则返回false
	 */
	bool Delete(const int&);

	/*!
	 * @brief 排序
	 *
	 * 为相邻的任务内容相同的任务进行排序
	 * @date 2019/11/27
	 * @author FanKaiyu
	 */
	void Sort();

	/*!
	 * @brief 指向下一任务
	 *
	 * 将游标执行下一任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return bool 存在下一任务返回true,否则返回false
	 */
	bool Next();

	/*!
	 * @brief 设置游标位置
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 游标坐标
	 */
	void Seed(const int&);

	/*!
	 * @brief 队列中第一个任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return TaskBase
	 */
	TaskBase Head() const;

	/*!
	 * @brief 队列中最后一个任务
	 *
	 * Order::Tail
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return TaskBase
	 */
	TaskBase Tail() const;

	/*!
	 * @brief 获取任务
	 *
	 * 获取当前游标指向的任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return TaskBase
	 */
	TaskBase GetTask() const;

	/*!
	 * @brief 获取任务
	 *
	 * 通过任务号获取任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @param const int & 任务号
	 * @return TaskBase
	 */
	TaskBase GetTask(const int&) const;

	/*!
	 * @brief 判断订单是否完整
	 *
	 * 即判断订单中是否包含结束任务
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return bool 完整返回true,否则返回false
	 */
	bool IsFull() const;
};

/*!
 * @brief 生成订单号
 *
 * 一组由时间日期组成的字符串时间精确至毫秒,
 * 如“020191126084700123”.
 * 共18位
 * @date 2019/11/26
 * @author FanKaiyu
 * @return QT_NAMESPACE::QString 生成的订单号
 */
QString CreateOrderNumber()
{
	std::chrono::system_clock::time_point tpCur = std::chrono::system_clock::now();

	time_t tt = std::chrono::system_clock::to_time_t(tpCur);
	tm* t = std::localtime(&tt);

	auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(tpCur.time_since_epoch());

	QString str = QString("%1%2%3%4%5%6%7")
		.arg(t->tm_year + 1900, 4, 10, QLatin1Char('0'))
		.arg(t->tm_mon + 1, 2, 10, QLatin1Char('0'))
		.arg(t->tm_mday, 2, 10, QLatin1Char('0'))
		.arg(t->tm_hour, 2, 10, QLatin1Char('0'))
		.arg(t->tm_min, 2, 10, QLatin1Char('0'))
		.arg(t->tm_sec, 2, 10, QLatin1Char('0'))
		.arg(timeNow.count() % 1000, 3, 10, QLatin1Char('0'));

	return QString("%1").arg(str, 18, QLatin1Char('0'));
}

#endif //TASKBASE_H

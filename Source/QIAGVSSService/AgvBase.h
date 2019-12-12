/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file AgvBase.h
 *
 * @brief 描述AGV类型基本属性以及功能的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.1
 */
#ifndef AGVBASE_H
#define AGVBASE_H

#include <QObject>
#include <memory>
#include <map>
#include "AgvType.h"
#include "SlaveConnect.h"
#include "Protocol.h"

typedef class AgvBase Agv;
typedef unsigned short id_t;
typedef std::shared_ptr<Agv> AgvPtr;
typedef std::map < id_t, AgvPtr> AgvArray;

#pragma region 枚举
/*! @brief  描述AGV指令动作结果的枚举 */
enum class CmdError : unsigned char
{
	Cmd_Success,		/*!< 指令创建成功 */
	Cmd_StatusError,	/*!< 状态错误 */
	Cmd_NetError,		/*!< 网络错误 */
	Cmd_ModeError,		/*!< 模式错误 */
	Cmd_ParamError,		/*!< 参数错误 */
	Cmd_ActionError,	/*!< 动作错误 */
	Cmd_LocationError,	/*!< 坐标错误 */
	Cmd_PowerError,		/*!< 电量错误 */
	Cmd_Unknow = 255,	/*!< 未知错误 */
};

/*!  @brief 描述AGV指令功能的枚举 */
enum class AgvCmdFunc : unsigned char
{
	Func_Heartbeat = 0x1F,	/*!< 心跳 */
	Func_Move = 0x2F,		/*!< 移动控制 */
	Func_Action = 0x3F,		/*!< 动作控制 */
	Func_Traffic = 0x4F,	/*!< 交通管制控制 */
	Func_Status = 0x5F,		/*!< 状态控制 */
	Func_Speed = 0x6F,		/*!< 速度控制 */
};

/*! @brief 描述AGV状态控制码的枚举 */
enum class AgvStatusCtrl : unsigned char
{
	Ctrl_WakeUp,				/*!< 唤醒 */
	Ctrl_Reset,					/*!< 复位 */
	Ctrl_Restart,				/*!< 重置*/
	Ctrl_RmtScream,				/*!< 远程急停 */
	Ctrl_Sleep,					/*!< 休眠 */
	Ctrl_Pause,					/*!< 暂停 */
	Ctrl_Continue,				/*!< 继续 */
	Ctrl_AllScream,				/*!< 全线急停 */
	Ctrl_Shutdonw = UCHAR_MAX,	/*!< 关机 */
};

/*!
 * @brief 描述AGV控制类型的枚举
 * @date 2019/11/17
 * @author FanKaiyu
 */
enum class AgvCtrlType : unsigned char
{
	Ctrl_WakeUp,				/*!< 唤醒 */
	Ctrl_Reset,					/*!< 复位 */
	Ctrl_Restart,				/*!< 重置*/
	Ctrl_RmtScream,				/*!< 远程急停 */
	Ctrl_Sleep,					/*!< 休眠 */
	Ctrl_Pause,					/*!< 暂停 */
	Ctrl_Continue,				/*!< 继续 */
	Ctrl_Move,					/*!< 移动 */
	Ctrl_Action,				/*!< 动作控制 */
	Ctrl_Speed,					/*!< 速度控制 */
	Ctrl_Shutdonw = UCHAR_MAX,	/*!< 关机 */
};
#pragma endregion

/*!
 * @class AgvBase
 * @brief 描述AGV基本属性与功能的类
 */
class AgvBase : public QObject
{
	Q_OBJECT

public:
	explicit AgvBase(const shared_ptr<AgvType>& type, const id_t& id, const bool& bClient, const QString& qstrAddr, const unsigned short& usPort,
		const QString& localAddr = "", const unsigned short& localPort = 0, QObject* parent = nullptr);
	~AgvBase();

protected:
	const shared_ptr<AgvType> m_pType;			/*!< 类型 */
	AgvAttribute m_attribute;					/*!< 属性 */
	SlaveConnect m_conn;						/*!< 客户端 */

protected:
	id_t m_id;									/*!< 编号 */

protected:
	QByteArray m_byarrBuffer;	/*!< 接收下位机上传的数据的缓存区 */
	ByteArrayList m_listSend;					/*!< 待发送的数据列表 */
	QThread m_thread;							/*!< 报文发送线程 */
	QTimer m_timer;

public:
	/*!
	 * @brief 获取动作详细信息
	 * @return QString 动作详细信息
	*/
	QString GetActionInfo() const;

	/*!
	 * @brief 获取属性
	 * @return AgvAttr Agv属性状态;
	*/
	const AgvAttr GetAttribute() const { return m_attribute; }

	/*!
	 * @brief 更新属性
	 * @param const AgvAttr& 新的属性
	 * @return bool 当属性发生改变时返回true,否则返回false
	*/
	bool UpdateAttribute(const AgvAttr& attr) { return m_attribute.Update(attr); }

	/*!
	 * @brief 获取正在执行的动作名称
	 * @date 2019/11/27
	 * @author FanKaiyu
	 * @return QString 动作名称
	 */
	QString GetActionName() const { return m_pType->GetActionName(m_attribute.GetAction()); }

	/*!
	 * @brief 获取AGV实际运行速度
	 * @date 2019/12/02
	 * @author FanKaiyu
	 * @return float 实际运行速度
	 */
	float GetActualSpeed() const { return (m_pType->GetMaxSpeed() * static_cast<float>(m_attribute.m_speed)) / 100.0f; }

	/*!
	 * @brief 获取类型
	 * @return const AgvType* 类型指针
	*/
	const shared_ptr<AgvType> GetType() const { return m_pType; }

	/*!
	 * @brief 获取编号
	 * @return const id_t 编号
	*/
	id_t GetId() const { return m_id; }

	/*!
	 * @brief 判断是否已连接下位机
	 * @return bool 已连接返回true,否则返回false
	*/
	bool IsConnected() const { return m_conn.IsConnected(); }

	/*!
	 * @brief 连接下位机
	 * @return bool 连接成功返回true,否则返回false
	*/
	bool Connect(QTcpSocket* socket) { return m_conn.Connect(socket); }
protected:
	/*!
	 * @brief 处理报文列表
	 * @param const QByteArrayList& 待处理的报文列表
	*/
	void ProcessPacket(const ByteArrayList& list);

	void ProcessPacketStm32(const ByteArrayList& list);
	void ProcessPacketPlc(const ByteArrayList& list);

signals:
	/*!
	 * @brief 当下位机连接成功时发出的信号
	*/
	void signalConnected(const unsigned short&);

	/*!
	 * @brief 当与下位连接中断时发出的信息
	*/
	void signalConnectionBreak(const unsigned short&);

	/*!
	 * @brief 当属性状态发生改变时发出的信号
	*/
	void signalUpdate(const unsigned short&);

	bool signalWriteData(QByteArray) const;

public slots:
	/*!
	 * @brief 当有数据需要处理时触发的槽函数
	*/
	void slotProcess(QByteArray);

	/*!
	 * @brief 连接下位机成功时触发的槽函数
	*/
	void slotConnected();

	/*!
	 * @brief 与下位机连接中断时触发的槽函数
	*/
	void slotConnectionBreak();

	/*!
	 * @brief 报文发送
	*/
	void slotSendPacket();

#pragma region 动作与通信指令
public:
	/*!
	 * @brief 唤醒
	 *
	 * 唤醒处于休眠状态下的AGV
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError WakeUp();

	/*!
	 * @brief 复位
	 *
	 * 恢复因远程急停指令或全线急停指令停止的AGV运行
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	 */
	CmdError Reset();

	/*!
	 * @brief 重置
	 *
	 * 使AGV恢复至初始状态
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Restart();

	/*!
	 * @brief 休眠
	 *
	 * 使AGV进入休眠模式
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Sleep();

	/*!
	 * @brief 暂停
	 *
	 * 使AGV暂停正在执行的动作
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Pause();

	/*!
	 * @brief 继续
	 *
	 * 使AGV继续被暂停的任务
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Continue();

	/*!
	 * @brief 远程急停
	 *
	 * 通过指令使AGV紧急停止
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError RemoteScream();

	/*!
	 * @brief 全线急停
	 *
	 * 通过系统外接设备上的急停按钮,触发指令,通知AGV紧急停止
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError AllScream();

	/*!
	 * @brief 关机
	 *
	 * 使AGV关闭电源
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Shutdown();

public:
	/*!
	 * @brief 移动控制
	 *
	 * 通知AGV移动至指定的坐标
	 * @param rfid_t 终点RFID地标卡编号
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Move(const rfid_t& location);

	/*!
	 * @brief 执行动作
	 *
	 * 通知AGV执行相应的动作
	 * @param const AgvAttribute::action_t& 动作控制码
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError Action(const AgvAction& action);
	CmdError Action(const QString& actName) { return Action(m_pType->GetActionCode(actName)); }

	/*!
	 * @brief 停止动作
	 *
	 * 通知AGV停止正在执行的动作
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError StopAction();

	/*!
	 * @brief 交通管制放行
	 *
	 * 通知AGV允许通过交通管制点
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError TrafficPass();

	/*!
	 * @brief 交通管制停止
	 *
	 * 通知AGV在当前坐标交通管制停止
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError TrafficStop();

	/*!
	 * @brief 速度控制
	 *
	 * 通知AGV将速度调整至指定值
	 * @param const AgvAttribute::speed_t& 速度值
	*/
	CmdError SpeedControl(const AgvAttr::speed_t& speed);

protected:
	/*!
	 * @brief 心跳
	 *
	 * 获取AGV实时状态信息
	*/
	void Heartbeat();

	void HeartbeatStm32();

	void HeartbeatPlc();

	CmdError MoveStm32(const rfid_t& location);

	CmdError MovePlc(const rfid_t& location);

	/*!
	 * @brief 动作控制
	 *
	 * 通知AGV执行相应的动作
	 * @param const AgvAttribute::action_t& 动作控制码
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError ActionControl(const AgvAction& action);

	CmdError ActionControlStm32(const AgvAction& action);

	CmdError ActionControlPlc(const AgvAction& action);

	/*!
	 * @brief 状态控制
	 * @param unsigned char 状态控制码
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError StatusControl(const AgvStatusCtrl& cmd);

	/*!
	 * @brief 交通管制控制
	 *
	 * @param const unsigend char& 管制命令
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError TrafficControl(const unsigned char& cmd);

	CmdError TrafficControlStm32(const unsigned char& cmd);

	CmdError TrafficControlPlc(const unsigned char& cmd);

	/*!
	 * @brief 发送指令
	 * @param const char* 数据体首地址指针
	 * @param const int& 数据体大小
	 * @return int 指令生成成功返回0,否则返回指令错误码(CmdError)
	*/
	CmdError SentCommand(const unsigned char* data, const int& maxSize);

	CmdError SendData(const unsigned char* data, const int& maxSize);
#pragma endregion
};

#endif // AGVBASE_H

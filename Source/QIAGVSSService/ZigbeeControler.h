/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file ZigbeeControler.h
 *
 * @brief Zigbee控制器文件
 *
 * 描述Zigbee控制器的基本属性与功能的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef ZIGBEECONTROL_H
#define ZIGBEECONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QTimer>
#include <memory>
#include <map>
#include "ProtocolZigbee.h"
#include "ZigbeeCaller.h"		

typedef class ZigbeeControler ZgbCtrler;

enum class ZigbeeCmdFunc : unsigned	char
{
	Func_Heartbeat = 0x1F,	/*!< 心跳 */
	Func_Set = 0x2F,		/*!< 设置 */
	Func_Clear = 0x3F,		/*!< 清除 */
};

/*!
 * @class ZigbeeControler
 * @brief Zigbee控制器类
 *
 * 描述Zigbee控制器的属性以及功能
 * @date 2019/11/16
 * @author FanKaiyu
 */
class ZigbeeControler
	: public QObject
{
	Q_OBJECT
public:
	/*!
	 * @brief ZigbeeControler
	 *
	 * ZigbeeControler::ZigbeeControler
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	ZigbeeControler();

	/*!
	 * @brief ZigbeeControler
	 *
	 * overload ZigbeeControler::ZigbeeControler
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const QString & 串口名
	 * @param const QSerialPort::BaudRate & 波特率
	 * @param const QSerialPort::FlowControl & 流控制
	 * @param const QSerialPort::Parity & 校验位
	 * @param const QSerialPort::DataBits & 数据位
	 * @param const QSerialPort::StopBits & 停止位
	 */
	explicit ZigbeeControler(const QString&,
		const QSerialPort::BaudRate & = QSerialPort::BaudRate::Baud9600,
		const QSerialPort::FlowControl & = QSerialPort::FlowControl::NoFlowControl,
		const QSerialPort::Parity & = QSerialPort::Parity::NoParity,
		const QSerialPort::DataBits & = QSerialPort::DataBits::Data8,
		const QSerialPort::StopBits & = QSerialPort::StopBits::OneStop);
	~ZigbeeControler();
protected:
	QString m_serialName;					/*!< 串口名 */
	QSerialPort::BaudRate m_baudBite;		/*!< 波特率 */
	QSerialPort::StopBits m_stopBite;		/*!< 停止位 */
	QSerialPort::DataBits m_dataBite;		/*!< 数据位*/
	QSerialPort::Parity m_parityBite;		/*!< 校验位 */
	QSerialPort::FlowControl m_flowCtrl;	/*!< 流控制 */
protected:
	QSerialPort m_serialPort;				/*!< 串口对象 */
	static ProtocolZigbee m_protocol;		/*!< 通信协议 */
protected:
	ByteArrayList m_listSend;				/*!< 待发送的数据列表 */
	QThread m_thread;						/*!< 报文发送线程 */
	QTimer m_timer;
	QByteArray m_byarrBuffer;				/*!< 接收下位机上传的数据的缓存区 */
protected:
	CallerArray m_mapCallers;				/*!< Zigbee呼叫器组 */
public:
	/*!
	 * @brief 读取呼叫器配置信息
	 *
	 * 从JSON文档中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonDocument & 包含配置信息的JSON文档
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadCaller(const QJsonDocument&);

	/*!
	 * @brief 读取呼叫器配置信息
	 *
	 *从JSON对象中读取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const QJsonObject & 包含配置信息的JSON对象
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool LoadCaller(const QJsonObject&);
public:
	/*!
	 * @brief 获取呼叫器指针
	 *
	 * 通过呼叫器编号获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @return std::shared_ptr<ZigbeeCaller>
	 */
	CallerPtr callerPtr(const unsigned short&) const;

	/*!
	 * @brief 获取呼叫器指针
	 *
	 * 通过呼叫器地址获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const std::string & 呼叫器地址
	 * @return std::shared_ptr<ZigbeeCaller>
	 */
	CallerPtr GetCallerPtr(const std::string&) const;

	/*!
	 * @brief 获取呼叫器指针
	 *
	 * 通过呼叫器绑定工作站获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @return std::shared_ptr<ZigbeeCaller>
	 */
	CallerPtr GetCallerPtr(const unsigned short&) const;

	/*!
	 * @brief 获取呼叫器对象
	 *
	 * 通过呼叫器编号获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @return const ZgbCaller
	 */
	const ZgbCaller caller(const unsigned short&) const;

	/*!
	 * @brief 获取呼叫器对象
	 *
	 * 通过呼叫器地址获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器地址
	 * @return const ZgbCaller
	 */
	const ZgbCaller GetCaller(const std::string&) const;

	/*!
	 * @brief 获取呼叫器对象
	 *
	 * 通过呼叫器绑定工作站获取
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @return const ZgbCaller
	 */
	const ZgbCaller GetCaller(const unsigned short&);
public:
	/*!
	 * @brief 开打串口
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return bool 打开成功返回true,否则返回false
	 */
	bool Open();

	/*!
	 * @brief 打开串口
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const QString & 串口号
	 * @param const QSerialPort::BaudRate & 波特率
	 * @param const QSerialPort::FlowControl & 流控制
	 * @param const QSerialPort::Parity & 校验位
	 * @param const QSerialPort::DataBits & 数据位
	 * @param const QSerialPort::StopBits & 停止位
	 * @return bool 打开成功返回true,否则返回false
	 */
	bool Open(const QString&,
		const QSerialPort::BaudRate & = QSerialPort::BaudRate::Baud9600,
		const QSerialPort::FlowControl & = QSerialPort::FlowControl::NoFlowControl,
		const QSerialPort::Parity & = QSerialPort::Parity::NoParity,
		const QSerialPort::DataBits & = QSerialPort::DataBits::Data8,
		const QSerialPort::StopBits & = QSerialPort::StopBits::OneStop);

	/*!
	 * @brief 判断串口是否打开
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return bool 已打开返回true,否则返回false
	 */
	bool IsOpened() const { return m_serialPort.isOpen(); }

	/*!
	 * @brief 关闭串口
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	void Close();

	/*!
	 * @brief 判断是否触发了急停信号
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return bool 触发急停信号返回true,否则返回false
	 */
	bool IsScream() const;
public:
	/*!
	 * @brief 读取呼叫器状态
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @param const bool & 呼叫器启用状态
	 * @param const CallerAttr & 呼叫器属性
	 */
	bool LoadSignal(const unsigned short&, const bool&, const CallerAttr&);

	/*!
	 * @brief 启用全部呼叫器
	*/
	void EnableUseAllCaller();
	void EnableUseCaller(const unsigned short&);
	void EnableUseCaller(ZigbeeCaller& caller);

	/*!
	 * @brief 禁用全部呼叫器
	*/
	void DisableUseAllCaller();
	void DisableUseCaller(const unsigned short&);
	void DisableUseCaller(ZigbeeCaller& caller);
protected:
	/*!
	 * @brief 处理报文列表
	 * @param const QByteArrayList& 待处理的报文列表
	*/
	void ProcessPacket(const ByteArrayList& list);

	/*!
	 * @brief 更新呼叫器信号状态
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @param const CallerAttr & 新的呼叫器信号状态
	 */
	void UpdateCallerSignal(const unsigned short&, const CallerAttr&);
signals:
	/*!
	 * @brief 当控制器读取到呼叫器的信号状态时触发此函数
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param[out] const unsigned short & 呼叫器编号
	 */
	void signalUpdateCallerSignal(const unsigned short&);

	/*!
	 * @brief 当呼叫器启用状态发送改变时触发此函数
	 *
	 * ZigbeeControler::signalUpdateCallerEnableUse
	 * @date 2019/11/25
	 * @author FanKaiyu
	* @param[out] const unsigned short & 呼叫器编号
	 */
	void signalUpdateCallerEnableUse(const unsigned short&);

	/*!
	 * @brief 串口状态改变时发出此信号
	 * @date 2019/11/25
	 * @author FanKaiyu
	 * @param QString 描述串口当前状态的字符串
	 */
	void signalSerialPortStateChange(QString);

public slots:
	/*!
	 * @brief 读取数据
	 *
	 * 当存在可读取数据时触发的槽函数
	 * @date 2019/11/16
	 * @author FanKaiyu
	 */
	void slotReadyRead();
	void slotSentPacket();
protected:
	void Heartbeat();
	void Heartbeat(const ZgbCaller&);
public:
	/*!
	 * @brief 设置呼叫器信号
	 *
	 * 通过呼叫器编号设置
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @param const SignalType & 信号类型
	 * @param const Signal & 信号
	 */
	void signal(const unsigned short&, const SignalType&, const Signal&);

	/*!
	 * @brief 设置呼叫器信号
	 *
	 * 通过呼叫器地址设置
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const std::string & 呼叫器地址
	 * @param const SignalType & 信号类型
	 * @param const Signal & 信号
	 */
	void SetSignal(const std::string&, const SignalType&, const Signal&);

	/*!
	 * @brief 设置呼叫器信号
	 *
	 * 通过绑定工作站设置
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @param const SignalType & 信号类型
	 * @param const Signal & 信号
	 */
	void SetSignal(const unsigned short&, const SignalType&, const Signal&);

	void SetSignal(const ZgbCaller&, const SignalType&, const Signal&);

	/*!
	 * @brief 清除呼叫器信号
	 *
	 * 通过呼叫器编号清除
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 呼叫器编号
	 * @param const SignalType & 信号类型
	 */
	void clear(const unsigned short&, const SignalType&);

	/*!
	 * @brief 清除呼叫器信号
	 *
	 * 通过呼叫器地址清除
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const std::string & 呼叫器地址
	 * @param const SignalType & 信号类型
	 */
	void ClearSignal(const std::string&, const SignalType&);

	/*!
	 * @brief 清除呼叫器信号
	 *
	 * 通过绑定工作站清除
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @param const unsigned short & 工作站编号
	 * @param const SignalType & 信号类型
	 */
	void ClearSignal(const unsigned short&, const SignalType&);

	void ClearSignal(const ZgbCaller&, const SignalType&);

protected:
	void SendCommand(const std::string, const ByteArray&);
};

#endif // ZIGBEECONTROL_H

/*!
 * @file SlaveConnect
 * @brief 用以连接下位机设备的文件
 * @date 2019-10-28
 * @author FanKaiyu
 * @version 2.0
*/

#ifndef TCPCLIENT_T
#define TCPCLIENT_T

#include <QObject>
#include <QtNetwork>

/*!
 * @class SlaveConnect
 * @brief 描述下位机连接的类
*/
typedef class SlaveConnect : public QObject
{
	Q_OBJECT
public:
	explicit SlaveConnect(const bool& bClient, const QString& qstrAddr, const quint16& usPort, const QString& addrLocal = "", const quint16& usLocalPort = 0
		, QObject* parent = nullptr);
	~SlaveConnect();

protected:
	bool m_bClient;				/*!< 模式标识:为true时客户端以客户端模式运行,为false时客户端以服务端模式运行 */
	QTcpSocket* m_pSocket;		/*!< Socket对象指针:描述与下位机通信通道的Socket对象指针 */

protected:
	QString m_qstrAddr;			/*!< 下位机地址:值不能为空,否则下位机对象无效 */
	QString m_addrLocal;	/*!< 连接下位机时绑定的本地IP地址 */
	quint16 m_usPort;			/*!< 下位机端口:值为0时,采用随机端口 */
	quint16 m_usLocalPort;		/*!< 连接下位机时绑定的本地端口 */

protected:
	/*!
	 * @brief 初始化下位机连接
	 * @param const bool& 模式标识:为true时客户端以客户端模式运行,为false时客户端以服务端模式运行
	 * @param const QString& 下位机地址:值不能为空,否则下位机对象无效
	 * @param const quint16& 下位机端口:值为0时,采用随机端口
	 * @param const QString& 连接下位机时绑定的本地IP地址
	 * @param const quint16 连接下位机时绑定的本地端口
	 * @throw std::string 当传入的参数出现错误时,抛出字符异常
	*/
	void Initialize(const bool& bClient, const QString& qstrAddr, const quint16& usPort, const QString& addrLocal = "", const quint16& usLocalPort = 0);

public:
	/*!
	 * @brief 判断是否与下位机连接
	 * @return bool 已连接返回true,否则返回false
	*/
	bool IsConnected() const;

	/*!
	 * @breif 连接下位机
	 *
	 * 当下位机以服务端模式运行时调用此函数主动连接下位机
	*/
	void Connect();

	/*!
	 * @brief 连接下位机
	 *
	 * 当服务端接收下位机连接请求时调用此函数
	 * @param QTcpSocket* 描述与下位机通信通道的Socket对象指针
	 * @return bool 连接成功返回true,否则返回false
	*/
	bool Connect(QTcpSocket* socket);

protected:
	/*!
	 * @breif 重新连接下位机
	*/
	void Reconnect();

signals:
	/*!
	 * @brief 连接下位机成功时发出的信号
	*/
	void signalConnected();

	/*!
	 * @brief 与下位机连接中断时发出的信号
	*/
	void signalConnectionBreak();

	/*!
	 * @brief 当存在需要处理的数据时发出的信号
	*/
	void signalProcess(QByteArray);

public slots:
	/*!
	 * @breif 发送数据至下位机
	 * @param const char* 数据首地址指针
	 * @param const qint64& 数据大小
	 * @return bool 发送成功返回true,否则返回false
	*/
	bool WriteData(const char* data, const qint64& maxSize) const;
	bool WriteData(QByteArray data) const;
	bool WriteData(const char* data) const;

	/*!
	 * @brief 连接下位机成功时触发的槽函数
	*/
	void slotConnected();

	/*!
	 * @brief 连接下位机失败时触发的槽函数
	*/
	void slotConnectFailed(QAbstractSocket::SocketError error);

	/*!
	 * @brief 与下位机连接中断时触发的槽函数
	*/
	void slotConnectionBreak();

	/*!
	 * @brief 当存在可接收的数据时触发的槽函数
	*/
	void slotReadyRead();
} SlaveConn;

#endif // TCPCLIENT_T

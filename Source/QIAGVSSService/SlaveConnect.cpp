#include "SlaveConnect.h"
#include <string>

SlaveConnect::SlaveConnect(const bool& bClient, const QString& qstrAddr, const quint16& usPort, const QString& addrLocal, const quint16& usLocalPort, QObject* parent)
	: QObject(parent)
	, m_pSocket(nullptr)
{
	try
	{
		Initialize(bClient, qstrAddr, usPort, addrLocal, usLocalPort);
	}
	catch (const std::string & e)
	{
		throw(e.c_str());
	}
}

SlaveConnect::~SlaveConnect()
{
	if (m_pSocket)
	{
		m_pSocket->close();

		while (m_pSocket)
		{
			m_pSocket->waitForDisconnected(-1);
		}

		QObject::disconnect(m_pSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &SlaveConnect::slotConnectFailed);
		QObject::disconnect(m_pSocket, &QTcpSocket::connected, this, &SlaveConnect::slotConnected);
		QObject::disconnect(m_pSocket, &QTcpSocket::disconnected, this, &SlaveConnect::slotConnectionBreak);
		QObject::disconnect(m_pSocket, &QTcpSocket::readyRead, this, &SlaveConnect::slotReadyRead);
	}
}

void SlaveConnect::Initialize(const bool& bClient, const QString& qstrAddr, const quint16& usPort, const QString& addrLocal, const quint16& usLocalPort)
{
	m_bClient = bClient;
	m_qstrAddr = qstrAddr;
	m_usPort = usPort;
	m_addrLocal = addrLocal;
	m_usLocalPort = usLocalPort;

	if (m_qstrAddr.isNull() || m_qstrAddr.isEmpty())
	{
		throw("Error:The Slave Address cannot be null");
	}

	if (m_bClient == false && m_usPort == 0)
	{
		throw("Error:The Slave Port cannot be 0");
	}

	Connect();

	return;
}

bool SlaveConnect::IsConnected() const
{
	if (m_pSocket == nullptr)
	{
		return false;
	}

	switch (m_pSocket->state())
	{
	case QAbstractSocket::SocketState::BoundState:
		// The socket is bound to an address and port.
		// 套接字绑定到一个地址和端口。
		break;
	case QAbstractSocket::SocketState::ClosingState:
		// The socket is about to close (data may still be waiting to be written).
		// 套接字即将关闭(数据可能仍在等待写入)。
		return false;
	case QAbstractSocket::SocketState::ConnectedState:
		// A connection is established.
		// 建立连接。
		break;
	case QAbstractSocket::SocketState::ConnectingState:
		// The socket has started establishing a connection.
		// 套接字已开始建立连接。
		return true;
	case QAbstractSocket::SocketState::HostLookupState:
		// The socket is performing a host name lookup.
		// 套接字正在执行主机名查找。
		return false;
	case QAbstractSocket::SocketState::ListeningState:
		// For internal use only.
		// 仅供内部使用。
		break;
	case QAbstractSocket::SocketState::UnconnectedState:
		// The socket is not connected.
		// 未连接
		return false;
	default:
		break;
	}

	return m_pSocket->isOpen();
}

bool SlaveConnect::Connect(QTcpSocket* socket)
{
	if (m_bClient == false)
	{
		return false;
	}

	if (socket == nullptr)
	{
		return false;
	}

	if (m_pSocket == socket)
	{
		return true;
	}

	if (QHostAddress(m_qstrAddr) != socket->peerAddress())
	{
		return false;
	}

	if (m_usPort != 0 && m_usPort != socket->peerPort())
	{
		return false;
	}

	if (m_pSocket)
	{
		m_pSocket->close();

		while (m_pSocket)
		{
			m_pSocket->waitForDisconnected(-1);
		}
	}

	m_pSocket = socket;

	QObject::connect(m_pSocket, &QTcpSocket::disconnected, this, &SlaveConnect::slotConnectionBreak);
	QObject::connect(m_pSocket, &QTcpSocket::readyRead, this, &SlaveConnect::slotReadyRead);

	slotConnected();

	return true;
}

bool SlaveConnect::WriteData(const char* data, const qint64& maxSize) const
{
	if (IsConnected() == false)
	{
		return false;
	}

	qint64 len = m_pSocket->write(data, maxSize);

	if (len == -1)
	{
		return false;
	}

	if (len != maxSize)
	{
		return false;
	}

	//QString _debug = "Sent data:";
	//for (int i = 0; i < maxSize; ++i)
	//{
	//	_debug += QString::fromLocal8Bit("%1 ").arg(*(data + i) & 0xFF, 2, 16, QLatin1Char('0'));
	//}

	//qDebug() << _debug << endl;

	return m_pSocket->waitForBytesWritten(1000);
}

bool SlaveConnect::WriteData(QByteArray data) const
{
	if (IsConnected() == false)
	{
		return false;
	}

	qint64 len = m_pSocket->write(data);

	if (len == -1)
	{
		return false;
	}

	if (len != data.size())
	{
		return false;
	}

	//QString _debug = "Sent data:";
	//for (int i = 0; i < data.size(); ++i)
	//{
	//	_debug += QString::fromLocal8Bit("%1 ").arg(*(data.data() + i) & 0xFF, 2, 16, QLatin1Char('0'));
	//}

	//qDebug() << _debug << endl;

	return m_pSocket->waitForBytesWritten();
	//return true;
}

bool SlaveConnect::WriteData(const char* data) const
{
	if (IsConnected() == false)
	{
		return false;
	}

	qint64 len = m_pSocket->write(data);

	if (len == -1)
	{
		return false;
	}

	return m_pSocket->waitForBytesWritten();
}

void SlaveConnect::Connect()
{
	if (m_bClient)
	{
		return;
	}

	if (IsConnected())
	{
		return;
	}

	if (m_pSocket)
	{
		m_pSocket->close();

		while (m_pSocket)
		{
			m_pSocket->waitForDisconnected(-1);
		}
	}

	m_pSocket = new QTcpSocket();

	QHostAddress local;

	if (m_addrLocal.isNull() || m_addrLocal.isEmpty())
	{
		local = QHostAddress::Any;
	}
	else
	{
		local.setAddress(m_addrLocal);
	}

	if (m_pSocket->bind(local, m_usLocalPort) == false)
	{
		// 绑定本地地址失败
		delete m_pSocket;
		m_pSocket = nullptr;

		return;
	}

	//QObject::connect(m_pSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &SlaveConnect::slotConnectFailed);
	QObject::connect(m_pSocket, &QTcpSocket::connected, this, &SlaveConnect::slotConnected);
	QObject::connect(m_pSocket, &QTcpSocket::disconnected, this, &SlaveConnect::slotConnectionBreak);
	QObject::connect(m_pSocket, &QTcpSocket::readyRead, this, &SlaveConnect::slotReadyRead);

	m_pSocket->connectToHost(m_qstrAddr, m_usPort);

	if (m_pSocket->waitForConnected() == false)
	{
		delete m_pSocket;
		m_pSocket = nullptr;
	}

	return;
}

void SlaveConnect::Reconnect()
{
	if (m_bClient)
	{
		return;
	}

	if (m_pSocket == nullptr)
	{
		return;
	}

	m_pSocket->connectToHost(m_qstrAddr, m_usPort);

	return;
}

void SlaveConnect::slotConnected()
{
	emit signalConnected();

	return;
}

void SlaveConnect::slotConnectFailed(QAbstractSocket::SocketError error)
{
	emit signalConnectionBreak();

	switch (error)
	{
	case QAbstractSocket::ConnectionRefusedError:
		// The connection was refused by the peer (or timed out).
		// 连接被对等方拒绝(或超时)。
		Reconnect();
		return;
	case QAbstractSocket::RemoteHostClosedError:
		// The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.
		// 远程主机关闭了连接.请注意客户端套接字即将在发送远程关闭通知后关闭
		Reconnect();
		return;
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
		Reconnect();
		return;
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

	qDebug() << "Socket error" << m_pSocket->errorString() << endl;

	return;
}

void SlaveConnect::slotConnectionBreak()
{
	QObject::disconnect(m_pSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &SlaveConnect::slotConnectFailed);
	QObject::disconnect(m_pSocket, &QTcpSocket::connected, this, &SlaveConnect::slotConnected);
	QObject::disconnect(m_pSocket, &QTcpSocket::disconnected, this, &SlaveConnect::slotConnectionBreak);
	QObject::disconnect(m_pSocket, &QTcpSocket::readyRead, this, &SlaveConnect::slotReadyRead);

	m_pSocket->deleteLater();
	m_pSocket = nullptr;

	emit signalConnectionBreak();

	return;
}

void SlaveConnect::slotReadyRead()
{
	if (m_pSocket->isReadable())
	{
		emit signalProcess(m_pSocket->readAll());
	}

	return;
}

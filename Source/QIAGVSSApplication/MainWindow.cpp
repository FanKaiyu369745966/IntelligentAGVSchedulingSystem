#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QSqlDatabase& db, QString user, QString passwd, int level, QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_db(db),
	m_userName(user),
	m_userPasswd(passwd),
	m_userLevel(level),
	m_srv(nullptr)
{
	ui->setupUi(this);

	initWindow();
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initWindow()
{
	m_srv = new Service(m_db);

	// 读取配置参数
	Config config("./res/set/config.ini");

	QString serverName = config.get("DataBase", "ServerName").toString();
	QString databaseName = config.get("DataBase", "DataBaseName").toString();
	QString loginName = config.get("DataBase", "LoginName").toString();
	QString passwd = config.get("DataBase", "Passwd").toString();

	QString netip = config.get("Net", "IP").toString();
	QString netport = config.get("Net", "Port").toString();

	QString com = config.get("SerialPort", "Com").toString();
	QString num = config.get("SerialPort", "Num").toString();
	QString baud = config.get("SerialPort", "Baud").toString();

	Config screem("./res/set/EQ2008_Screem_Set.ini");
	QString cardAddress = screem.get("Address:0", "CardAddress").toString();
	QString screemHeight = screem.get("Address:0", "ScreemHeight").toString();
	QString screemWidth = screem.get("Address:0", "ScreemWidth").toString();
	QString fontType = screem.get("Address:0", "FontType").toString();
	QString fontSize = screem.get("Address:0", "FontSize").toString();
	QString maxLine = screem.get("Address:0", "MaxLine").toString();
	QString x = screem.get("Address:0", "X").toString();
	QString y = screem.get("Address:0", "Y").toString();

	QJsonObject	jsonConfig, jsonDBAttr, jsonSrvAttr, jsonComAttr, jsonLedAttr;
	jsonDBAttr.insert("Host", serverName);
	jsonDBAttr.insert("User", loginName);
	jsonDBAttr.insert("Password", passwd);
	jsonDBAttr.insert("Database", databaseName);

	jsonSrvAttr.insert("IP", netip);
	jsonSrvAttr.insert("Port", netport.toInt());

	if (num.isNull() || num.isEmpty())
	{
		jsonComAttr.insert("Port", com);
	}
	else
	{
		jsonComAttr.insert("Port", num);
	}
	jsonComAttr.insert("Baud", baud.toInt());

	jsonLedAttr.insert("CardAddress", cardAddress.toInt());
	jsonLedAttr.insert("ScreemHeight", screemHeight.toInt());
	jsonLedAttr.insert("ScreemWidth", screemWidth.toInt());
	jsonLedAttr.insert("FontType", fontType);
	jsonLedAttr.insert("FontSize", fontSize.toInt());
	jsonLedAttr.insert("MaxLine", maxLine.toInt());
	jsonLedAttr.insert("X", x.toInt());
	jsonLedAttr.insert("Y", y.toInt());

	jsonConfig.insert("AGVDatabase", jsonDBAttr);
	jsonConfig.insert("Server", jsonSrvAttr);
	jsonConfig.insert("ZigbeeControler", jsonComAttr);
	jsonConfig.insert("Led", jsonLedAttr);

	// 窗口背景
	QPalette pal(this->palette());
	pal.setColor(QPalette::Background, QColor("#484C55"));
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	// 窗口初始尺寸
	this->resize(QSize(1600, 900));
	this->setFixedSize(this->width(), this->height());

	// 窗口标题栏
	setWindowIcon(QIcon("./res/icon/sys.png"));
	setWindowTitle(QString::fromLocal8Bit("分布式智能AGV调度系统V3.0"));
	//setWindowTitle(QString::fromLocal8Bit("辉腾自动化集中控制系统V1.0"));
	setWindowFlags(Qt::CustomizeWindowHint | Qt::Window | Qt::FramelessWindowHint);

	TitleBarEx* pTitleBar = new TitleBarEx(this);
	pTitleBar->setIcon("./res/icon/main.png");
	pTitleBar->setButtonType(MINI_MAX_BUTTON);
	pTitleBar->setTitle(QString::fromLocal8Bit("分布式智能AGV调度系统V3.0"));
	//pTitleBar->setTitle(QString::fromLocal8Bit("辉腾自动化集中控制系统V1.0"));
	QObject::connect(pTitleBar, &TitleBarEx::windowClose, this, &MainWindow::onWindowClose); // 绑定信号槽


	// 主页
	ui->tabWidget->setStyleSheet("QTabWidget::pane{border-left:1px solid #31343B;}\
            QTabBar::tab:last:!selected {border-image: url(./res/icon/error_normal.png);}\
            QTabBar::tab:last:selected {border-image: url(./res/icon/error_pressed.png);}");
	ui->tabWidget->setTabPosition(QTabWidget::West);
	ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);

	MapForm* pMapForm = new MapForm(m_db);
	ui->tabWidget->addTab(pMapForm, QString::fromLocal8Bit("地图信息"));
	QObject::connect(m_srv, &Service::signalShowAgv, pMapForm, &MapForm::onShowAgv);
	QObject::connect(m_srv, &Service::signalHideAgv, pMapForm, &MapForm::onHideAgv);
	QObject::connect(m_srv, &Service::signalUpdateCallerSignal, pMapForm, &MapForm::onUpdateCallerSignal);
	QObject::connect(m_srv, &Service::signalUpdateCallerUse, pMapForm, &MapForm::onUpdateCallerEnable);
	QObject::connect(pMapForm, &MapForm::onUpdateMapRelateTable, this, &MainWindow::slotUpdateMapRelateTable);

	TaskForm* pTaskForm = new TaskForm(m_db);
	ui->tabWidget->addTab(pTaskForm, QString::fromLocal8Bit("任务信息"));
	QObject::connect(pTaskForm, &TaskForm::createTask, m_srv, &Service::slotPublishOrder);

	HistoryForm* pHistoryForm = new HistoryForm(m_db);
	ui->tabWidget->addTab(pHistoryForm, QString::fromLocal8Bit("历史信息"));

	DeviceStateForm* pDeviceStateForm = new DeviceStateForm(m_db);
	ui->tabWidget->addTab(pDeviceStateForm, QString::fromLocal8Bit("设备状态"));

	ModulesForm* pModulesForm = new ModulesForm(m_db);
	ui->tabWidget->addTab(pModulesForm, QString::fromLocal8Bit("模块信息"));
	connect(pModulesForm, &ModulesForm::callerEnable, pMapForm, &MapForm::onUpdateCallerEnable);
	connect(pModulesForm, &ModulesForm::callerEnable, m_srv, &Service::slotCallerEnable);
	connect(m_srv, &Service::signalUpdateCallerData, pModulesForm, &ModulesForm::onUpdateCallerData);

	UserForm* pUserForm = new UserForm(m_db, m_userName, m_userPasswd, m_userLevel);
	ui->tabWidget->addTab(pUserForm, QString::fromLocal8Bit("用户信息"));

	// 根据用户权限添加功能
	ConfigForm* pConfigForm = new ConfigForm();
	ui->tabWidget->addTab(pConfigForm, QString::fromLocal8Bit("系统配置"));
	QObject::connect(m_srv, &Service::signalServerStateChange, pConfigForm, &ConfigForm::onNetServerStateChange);
	QObject::connect(m_srv, &Service::signalSerialPortStateChange, pConfigForm, &ConfigForm::onSerialPortStateChange);

	switch (m_userLevel)
	{
	case UserLevel::UserLevel_High:
		// 只有最高权限需显示用户信息页的用户设置组
		pUserForm->visibleUsersetGroup();
		break;
	}

	AbnormalForm* pAbnormalForm = new AbnormalForm(m_db);
	QObject::connect(pAbnormalForm, &AbnormalForm::updateAbnormalExist, this, &MainWindow::onUpdateAbnormalExist);
	ui->tabWidget->addTab(pAbnormalForm, QString::fromLocal8Bit("                 ")); // 异常信息

	ui->tabWidget->setCurrentIndex(0);

	m_srv->Start(QJsonDocument(jsonConfig));
}

void MainWindow::onWindowClose()
{
	MsgBoxEx* msgBox = new MsgBoxEx();
	QObject::connect(msgBox, &MsgBoxEx::btnOkClicked, this, &MainWindow::onBtnOkClicked);
	QObject::connect(msgBox, &MsgBoxEx::btnCancelClicked, this, &MainWindow::onBtnCancelClicked);
	msgBox->setMsgBoxMode(QString::fromLocal8Bit("确定要退出系统吗？"), "", MsgBoxBtnType::MsgBoxBtnType_OkCancle);
	delete msgBox;
}
void MainWindow::onBtnOkClicked()
{
	if (m_srv)
	{
		m_srv->Exit();
	}

	delete this;
	qApp->quit();
}

void MainWindow::onBtnCancelClicked() {}

void MainWindow::onUpdateAbnormalExist(bool exist)
{
	if (exist)
	{
		ui->tabWidget->setStyleSheet("QTabWidget::pane{border-left:1px solid #31343B;}\
                                      QTabBar::tab:last:!selected {border-image: url(./res/icon/error_normal_corner.png);}\
                                      QTabBar::tab:last:selected {border-image: url(./res/icon/error_pressed_corner.png);}");

	}
	else
	{
		ui->tabWidget->setStyleSheet("QTabWidget::pane{border-left:1px solid #31343B;}\
                                       QTabBar::tab:last:!selected {border-image: url(./res/icon/error_normal.png);}\
                                       QTabBar::tab:last:selected {border-image: url(./res/icon/error_pressed.png);}");
	}
}

void MainWindow::slotUpdateMapRelateTable()
{
	if (m_srv)
	{
		m_srv->Exit();
	}

	return;
}

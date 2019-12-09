#include "DeviceStateForm.h"
#include "ui_DeviceStateForm.h"

DeviceStateForm::DeviceStateForm(const QSqlDatabase& db, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::DeviceStateForm),
	m_db(db)
{
	ui->setupUi(this);

	ui->groupBox_1->setTitle(QString::fromLocal8Bit("AGV状态"));
	ui->groupBox_2->setTitle(QString::fromLocal8Bit("呼叫器状态"));

	m_modelAgv = new QSqlQueryModel(this);
	m_modelCaller = new QSqlQueryModel(this);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onUpdateModel()));
	timer->start(1000);
}

DeviceStateForm::~DeviceStateForm()
{
	delete ui;
}

void DeviceStateForm::onUpdateModel()
{
	int index = 0;
	// AGV
	m_modelAgv->setQuery(QString("SELECT agv_id,agv_connection,agv_mode_info,agv_cur,agv_target,agv_status_info,agv_battery,\
                                agv_speed_info,agv_cargo_info,agv_action_info,agv_error_info FROM AGVDB_INFO_AGV"), m_db);
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("编号"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("连接状态"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("模式"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("当前地标卡"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("终点地标卡"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("状态"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("电量(%)"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("速度(min/m)"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("载货信息"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("动作信息"));
	m_modelAgv->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常信息"));
	ui->tableViewAgv->setTableModel(m_modelAgv, true);

	// 呼叫器
	m_modelCaller->setQuery(QString("SELECT caller_id,caller_scream_info,caller_call_info,caller_cancel_info FROM AGVDB_INFO_CALLER"), m_db);
	m_modelCaller->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("编号"));
	m_modelCaller->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("急停信号状态"));
	m_modelCaller->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("呼叫信号状态"));
	m_modelCaller->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("取消信号状态"));
	ui->tableViewCaller->setTableModel(m_modelCaller);
}

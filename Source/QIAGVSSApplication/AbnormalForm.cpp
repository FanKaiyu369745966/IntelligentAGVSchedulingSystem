#include "AbnormalForm.h"
#include "ui_AbnormalForm.h"

AbnormalForm::AbnormalForm(const QSqlDatabase& db, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::AbnormalForm),
	m_db(db)
{
	ui->setupUi(this);

	m_model = new QSqlQueryModel(this);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onUpdateModel()));
	timer->start(1000);
}

AbnormalForm::~AbnormalForm()
{
	delete ui;
}


void AbnormalForm::onUpdateModel()
{
	m_model->setQuery(QString("SELECT * FROM AGVDB_INFO_ABNORMAL WHERE abnormal_state!=3"), m_db);

	// 强制获取整个数据集,默认256
	while (m_model->canFetchMore())
		m_model->fetchMore();

	int index = 0;

	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常编号"));
	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常发生时间"));
	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常来源"));
	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常描述"));
	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常等级"));
	m_model->setHeaderData(index++, Qt::Horizontal, QString::fromLocal8Bit("异常状态"));
	ui->tableView->setTableModel(m_model);

	if (m_model->rowCount() > 0)
	{
		emit updateAbnormalExist(true);
	}
	else
	{
		emit updateAbnormalExist(false);
	}
}

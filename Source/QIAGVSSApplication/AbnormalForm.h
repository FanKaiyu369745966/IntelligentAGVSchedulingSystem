#ifndef ABNORMALFORM_H
#define ABNORMALFORM_H

#include <QWidget>
#include "CustomData.h"
#include "MsgBoxEx.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlDatabase>

namespace Ui {
	class AbnormalForm;
}

class AbnormalForm : public QWidget
{
	Q_OBJECT

public:
	explicit AbnormalForm(const QSqlDatabase& db, QWidget* parent = nullptr);
	~AbnormalForm();

private:
	Ui::AbnormalForm* ui;
	QSqlQueryModel* m_model;
	QSqlDatabase m_db;

private slots:
	void onUpdateModel();

signals:
	void updateAbnormalExist(bool);

};

#endif // ABNORMALFORM_H

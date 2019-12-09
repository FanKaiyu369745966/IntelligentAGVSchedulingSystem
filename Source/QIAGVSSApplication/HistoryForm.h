#ifndef HISTORYFORM_H
#define HISTORYFORM_H

#include <QWidget>
#include <QTimer>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>

namespace Ui {
	class HistoryForm;
}

class HistoryForm : public QWidget
{
	Q_OBJECT

public:
	explicit HistoryForm(const QSqlDatabase& db, QWidget* parent = nullptr);
	~HistoryForm();

private:
	Ui::HistoryForm* ui;
	QSqlQueryModel* m_model;
	QSqlDatabase m_db;

private slots:
	void onUpdateModel();
};

#endif // HISTORYFORM_H

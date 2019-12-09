#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TitleBarEx.h"
#include "CustomTabStyle.h"
#include "MapForm.h"
#include "TaskForm.h"
#include "DeviceStateForm.h"
#include "UserForm.h"
#include "HistoryForm.h"
#include "AbnormalForm.h"
#include "ConfigForm.h"
#include "ModulesForm.h"
#include "MsgBoxEx.h"
#include "CustomData.h"
#include "../QIAGVSSService/Service.h"


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(const QSqlDatabase& db, QString user, QString passwd, int level, QWidget* parent = nullptr);
	~MainWindow();


private:
	Ui::MainWindow* ui;

	QString m_userName;
	QString m_userPasswd;
	int m_userLevel;

	QSqlDatabase m_db;
	Service* m_srv;

private:
	void initWindow();

private slots:
	void onWindowClose();
	void onBtnOkClicked();
	void onBtnCancelClicked();

	void onUpdateAbnormalExist(bool exist);

	void slotUpdateMapRelateTable();
};

#endif // MAINWINDOW_H

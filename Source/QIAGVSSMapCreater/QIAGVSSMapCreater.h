#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QIAGVSSMapCreater.h"

class QIAGVSSMapCreater : public QMainWindow
{
	Q_OBJECT

public:
	QIAGVSSMapCreater(QWidget *parent = Q_NULLPTR);

private:
	Ui::QIAGVSSMapCreaterClass ui;
};

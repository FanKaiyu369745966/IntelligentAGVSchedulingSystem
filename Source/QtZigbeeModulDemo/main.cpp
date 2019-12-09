#include <QApplication>
#include <QObject>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug.h>
#include <QString>	
#include <chrono>	
#include <ctime>	
#include <map>
#include "../QIAGVSSService/ZigbeeControler.h"
#include "../QIAGVSSService/TaskBase.h"

class Tester : public QObject
{
	Q_OBJECT
public:
	ZigbeeControler zgbeCtrler;
public:
	Tester(const QString& com, const QJsonObject& obj);
protected slots:
	void slotUpdateCallerSignal(const ZgbCaller&);
};

Tester::Tester(const QString& com, const QJsonObject& obj)
{
	connect(&zgbeCtrler, SIGNAL(signalUpdateCallerSignal(const ZgbCaller&)), this, SLOT(slotUpdateCallerSignal(const ZgbCaller&)));

	zgbeCtrler.LoadCaller(obj);
	zgbeCtrler.Open(com);
}

void Tester::slotUpdateCallerSignal(const ZgbCaller& caller)
{
	CallerAttr attr = caller.GetAttribute();

	qDebug() << "呼叫器:" << caller.GetId();

	qDebug() << "急停信号:" << attr.GetSignalInfo(attr.GetScreamSignal()) << endl;
	qDebug() << "呼叫信号:" << attr.GetSignalInfo(attr.GetCallSignal()) << endl;
	qDebug() << "取消信号:" << attr.GetSignalInfo(attr.GetCancelSignal()) << endl;

	return;
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		qDebug() << "Name : " << info.portName();
		qDebug() << "Description : " << info.description();
		qDebug() << "Manufacturer: " << info.manufacturer();
		qDebug() << "Serial Number: " << info.serialNumber();
		qDebug() << "System Location: " << info.systemLocation();

		qDebug() << endl;
	}

	QJsonObject obj, attr;

	attr.insert("MAC", "0001");
	attr.insert("Bind", 0);
	obj.insert(QString("%1").arg(1), attr);

	Tester user("COM8", obj);

	return a.exec();
}

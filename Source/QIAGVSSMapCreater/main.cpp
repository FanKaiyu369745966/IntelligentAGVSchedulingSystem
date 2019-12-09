#include "QIAGVSSMapCreater.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QIAGVSSMapCreater w;
	w.show();
	return a.exec();
}

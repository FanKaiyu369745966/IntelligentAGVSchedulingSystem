#include <QApplication>
#include "LoginForm.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	LoginForm loginForm;
	loginForm.show();

	QSharedMemory shared("agv-ctrl-system");
	if (shared.attach()) { return 0; }
	shared.create(1);

	int e = a.exec();

	if (e == RETCODE_RESTART)
	{
		QProcess::startDetached(qApp->applicationFilePath(), QStringList());
		return 0;
	}

	return e;
}


#include <QtCore/QCoreApplication>
#include <QtNetwork>
#include "server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Server server(NULL);

	return a.exec();
}

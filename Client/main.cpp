#include "client.h"
#include <QtWidgets/QApplication>
#include "login.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTcpSocket *Socket = new QTcpSocket;
	CLogin l(Socket);
	if (l.exec() != QDialog::Accepted)
	{
		return -1;
 	}

	Client c(Socket);
	c.show();

	return a.exec();
}

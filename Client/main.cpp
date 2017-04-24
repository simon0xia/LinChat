#include "client.h"
#include <QtWidgets/QApplication>
#include "login.h"


// int main(int argc, char *argv[])
// {
// 	QApplication a(argc, argv);
// 
// 	QTcpSocket *Socket = new QTcpSocket;
// 	CLogin l(Socket);
// 	if (l.exec() != QDialog::Accepted)
// 	{
// 		return -1;
// 	}
// 
// 	Client c(Socket);
// 	c.show();
// 
// 	return a.exec();
// }


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CLogin *l;

	QTcpSocket *Socket = new QTcpSocket;
	l = new CLogin(Socket);
	if (l->exec() != QDialog::Accepted)
	{
		return -1;
	}
	delete l;			//must call this in there!!!

	Client c(Socket);
	c.show();

	return a.exec();
}
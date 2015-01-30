#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets/QtWidgets>
#include <QtNetwork>

class Server:public QObject
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

public slots:
	void NewClient();
	void processPendingDatagrams();

private:
	QTcpServer *server;
	qint16 port;
};

#endif // SERVER_H

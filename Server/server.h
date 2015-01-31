#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets/QtWidgets>
#include <QtNetwork>
#include "client.h"

class Server:public QObject
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

public slots:
	void NewClient();
	void RemoveClient();

private:
	void timerEvent(QTimerEvent *);

private:
	QTcpServer *server;
	qint16 port;

	QVector<Client *> clientVec;
};

#endif // SERVER_H

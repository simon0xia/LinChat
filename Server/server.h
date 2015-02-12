#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets/QtWidgets>
#include <QtNetwork>
#include "client.h"

class Server:public QTcpServer
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

protected:
	void incomingConnection(qintptr handle);

private:
//	void timerEvent(QTimerEvent *);
	
private slots:
	void clientDisconnected(qintptr descriptor);
	void Socketerror(QAbstractSocket::SocketError);

private:
	qint16 port, maxClient;
	qint16 ClientCount;

	QList<Client *> ClientList;
};

#endif // SERVER_H

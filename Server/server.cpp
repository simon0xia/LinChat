#include "server.h"
#include "MessageDefine.h"

Server::Server(QObject *parent)
: QObject(parent)
{
	server = new QTcpServer(this);
	port = 7750;
	
	if (!server->listen(QHostAddress::Any, port))
	{
		qDebug() << tr("LinChat Server"),tr("Unable to start the server: %1.").arg(server->errorString());
		return;
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
			ipAddress = ipAddressesList.at(i).toString();
			break;
		}
	}
	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	qDebug() << (tr("The server is running on\n\nIP: %1\nport: %2\n\n"
		"Run the Fortune Client example now.")
		.arg(ipAddress).arg(server->serverPort()));

	connect(server, SIGNAL(newConnection()), this, SLOT(NewClient()));

	startTimer(1000);
}

Server::~Server()
{

}

void Server::timerEvent(QTimerEvent *t)
{
	Client *client;
	for (QVector<Client *>::iterator iter = clientVec.begin(); iter != clientVec.end(); ++iter)
	{
		if (!(*iter)->isLive())
		{
			client = *iter;
			delete client;

			clientVec.erase(iter);
		}
	}
}

void Server::NewClient()
{
	Client *client = new Client(this, server->nextPendingConnection());

//	connect(client->getSocket(), SIGNAL(disconnected()), this, SLOT(deleteLater()));
//	connect(client->getSocket(), SIGNAL(disconnected()), this, SLOT(RemoveClient()));
//	connect(client->getSocket(), SIGNAL(readyRead()), client, SLOT(processPendingDatagrams()));

	clientVec.push_back(client);
}

void Server::RemoveClient()
{
	
}
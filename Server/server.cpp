#include "server.h"


Server::Server(QObject *parent)
: QObject(parent)
{
	server = new QTcpServer(this);
	port = 7000;
	
	if (!server->listen(QHostAddress::Any, port))
	{
		qDebug() << tr("LinChat Server"),
			tr("Unable to start the server: %1.")
			.arg(server->errorString());
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
}

Server::~Server()
{

}

void Server::NewClient()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	QTcpSocket *clientConnection = server->nextPendingConnection();
	connect(clientConnection, SIGNAL(disconnected()),
		clientConnection, SLOT(deleteLater()));
	connect(clientConnection, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

}

void Server::processPendingDatagrams()
{
	while (true)
	{

	}
}
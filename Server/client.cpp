#include "client.h"
#include "MessageDefine.h"
#include "Users.h"

Client::Client(QObject *parent, QTcpSocket	*socket)
: QObject(parent), s(socket), isLogin(false)
{
	startTimer(1000);
	heartbeat = life;

	connect(s, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

Client::~Client()
{

}

void Client::processPendingDatagrams()
{
	QByteArray datagram = s->readAll();
	QDataStream oStream(&datagram, QIODevice::ReadOnly);

	qint32 ID;
	oStream >> ID;
	switch (ID)
	{
	case Heartbeat:
		qDebug() << tr("Client:%1 was Heartbeat.\n").arg(m_No);
		heartbeat = life;
		break;

	case Login:
		break;

	case Logout:
		break;

	default:
		break;
	}
}

void Client::timerEvent(QTimerEvent *t)
{
	--heartbeat;
//	if (heartbeat <= 0)
//	{
//		this->parent()->RemoveClient();
//	}
}
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
	if (!isLogin && ID != Login)
	{
		return;	//未登陆时，只处理Login消息。
	}

	QString strTmp;
	qint32 n;
	switch (ID)
	{
	case Heartbeat:
		qDebug() << tr("Client:%1 was Heartbeat.\n").arg(UID);
		heartbeat = life;
		break;

	case Login:		
		oStream >> n >> strTmp;
		isLogin = checkLogin(n, strTmp);
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

bool Client::checkLogin(qint32 id, QString &password)
{
	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	qint16 reason = 0;
	qint32 i;
	bool bRes = false;
	for (i = 0; i < AccountNO; i++)
	{
		if (Account[i].ID == id && password == Account[i].Password)
		{
			UID = id;
			bRes = true;
			break;
		}
	}
	
	iStream << Ack_Login << bRes;
	if (!bRes)
	{
		iStream << reason;
	}
	s->write(iData);

	qDebug() << tr("UID:%1 was login.\n").arg(UID);
	return bRes;
}
#include "client.h"
#include "MessageDefine.h"
#include "Users.h"

Client::Client(QObject *parent)
: QTcpSocket(parent), isLogin(false)
{
	startTimer(1000);
	heartbeat = life;

	connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(this, SIGNAL(disconnected()), this, SLOT(ClientClose()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Socketerror(QAbstractSocket::SocketError)));
}

Client::~Client()
{

}

void Client::ClientClose()
{
	qDebug() << "client disconnected.\n";
	emit CloseSignal(socketDescriptor());
	deleteLater();
}

void Client::Socketerror(QAbstractSocket::SocketError socketError)
{
//	LogIns.FlashLog("Client:%d An error occurred. Code:%d\n", socketDescriptor(), socketError);
	qDebug() << "Client:" << socketDescriptor() << "An error occurred.Code:" << socketError;
}

void Client::receiveMessage()
{
	QDataStream oStream(this);

	qint32 msgID;
	oStream >> msgID;
	if (!isLogin && msgID != CS_Login)
	{
		return;	//未登陆时，只处理Login消息。
	}

	if (msgID >= Max_CS_MSG)
	{
		qDebug() << "Warning, Illegal message ID:" << msgID;
		return;
	}

	QString strTmp;
	qint32 n;
	switch (msgID)
	{
	case CS_Heartbeat:
		qDebug() <<"Client:" << UID <<" was Heartbeat.\n";
		heartbeat = life;
		break;

	case CS_Login:		msg_login(oStream);			break;
	case CS_Logout:		msg_logout(oStream);		break;
	case CS_Chat_text:	msg_chat_text(oStream);		break;
	case CS_Chat_voice:	msg_chat_voice(oStream);		break;
	case CS_Chat_img:	msg_chat_img(oStream);		break;

	default:
		break;
	}
}

void Client::timerEvent(QTimerEvent *t)
{
	--heartbeat;
}

void Client::msg_login(QDataStream &oStream)
{
	quint32 id;
	QString password;
	oStream >> id >> password;

	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	quint32 reason = 0;
	bool bRes = false;
	for (quint32 i = 0; i < AccountNO; i++)
	{
		if (Account[i].ID == id && password == Account[i].Password)
		{
			UID = id;
			bRes = true;
			isLogin = true;
			break;
		}
	}
	
	iStream << SC_Login << bRes;
	if (!bRes)
	{
		iStream << reason;
	}
	write(iData);

	qDebug() << __FUNCTION__ << "  " << UID << "  " << password;
}

void Client::msg_logout(QDataStream &oStream)
{
	//暂不处理，不响应。
}

void Client::msg_chat_text(QDataStream &oStream)
{

}
void Client::msg_chat_voice(QDataStream &oStream)
{

}
void Client::msg_chat_img(QDataStream &oStream)
{

}
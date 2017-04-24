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

	while (!oStream.atEnd())
	{
		oStream >> msgID;
		if (!isLogin && msgID != CS_Login)
		{
			return;	//未登陆时，只处理Login消息。
		}

		if (msgID >= Max_CS_MSG)
			qDebug() << "Warning, Illegal message ID:" << msgID;

		QString strTmp;
		qint32 n;
		switch (msgID)
		{
		case CS_Heartbeat:
	//		qDebug() <<"Client:" << UID <<" was Heartbeat.";
			heartbeat = life;
			break;

		case CS_Login:		msg_login(oStream);			break;
		case CS_Logout:		msg_logout(oStream);		break;
		case CS_get_OwnInfo:msg_get_OwnInfo(oStream);	break;
		case CS_get_friend:	msg_get_friend(oStream);	break;
		case CS_Chat_text:	msg_chat_text(oStream);		break;
		case CS_Chat_voice:	msg_chat_voice(oStream);	break;
		case CS_Chat_img:	msg_chat_img(oStream);		break;

		default:
			break;
		}
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

	qDebug() << __FUNCTION__ << "  UID:" << UID << "  PWD:" << password;
}

void Client::msg_logout(QDataStream &oStream)
{
	//暂不处理，不响应。
}

void Client::msg_get_OwnInfo(QDataStream &oStream)
{
	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream << SC_get_OwnInfo << UID  << getName(UID);
	write(iData);

	qDebug() << __FUNCTION__ << UID << getName(UID);
}

void Client::msg_get_friend(QDataStream &oStream)
{
	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream << SC_get_friend << (quint32)AccountNO;

	qDebug() << __FUNCTION__;
	for (quint16 i = 0; i < AccountNO; i++)
	{
		iStream << Account[i].ID << Account[i].Name;
		qDebug() << Account[i].ID << Account[i].Name;
	}
	write(iData);
}

void Client::msg_chat_text(QDataStream &oStream)
{
	QString str;
	oStream >> str;

	QTime t = QTime::currentTime();
	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream << SC_Chat_text << UID << t.toString() << str;
	write(iData);

	qDebug() << __FUNCTION__ << UID << t.toString() << str;
}
void Client::msg_chat_voice(QDataStream &oStream)
{

}
void Client::msg_chat_img(QDataStream &oStream)
{

}
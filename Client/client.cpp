#include "client.h"
#include <QMessageBox>
#include <QScrollBar>

Client::Client(QTcpSocket *s)
:Socket(s), uid(0)
{
	ui.setupUi(this);

	connect(Socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

	startTimer(1000);

	SendBasicMsg(CS_get_OwnInfo);
	SendBasicMsg(CS_get_friend);
}

Client::~Client()
{
	delete Socket;
}

void Client::Socketerror(QAbstractSocket::SocketError socketError)
{
	//	LogIns.FlashLog("Client:%d An error occurred. Code:%d\n", socketDescriptor(), socketError);
	QMessageBox::warning(this, tr("Error"), 
		tr("Client:%1 An error occurred. Code:%1!").arg(Socket->socketDescriptor()).arg(socketError), 
		QMessageBox::Yes);
}

inline void Client::SendBasicMsg(MessageType msgID)
{
	QByteArray datagram;
	QDataStream iStream(&datagram, QIODevice::WriteOnly);
	iStream << msgID;
	Socket->write(datagram);
}

void Client::timerEvent(QTimerEvent *t)
{
	SendBasicMsg(CS_Heartbeat);
}

void Client::receiveMessage()
{
	QDataStream oStream(Socket);

	quint32 msgID;
	oStream >> msgID;

	if (msgID >= Max_SC_MSG || msgID <= Max_CS_MSG)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Illegal message ID!"), QMessageBox::Yes);
		return;
	}

	switch (msgID)
	{
	case SC_Logout:				/*cannot receive this message*/	break;
	case SC_participant_join:	msg_participant_join(oStream);	break;
	case SC_participant_left:	msg_participant_left(oStream);	break;
	case SC_get_OwnInfo:		msg_get_OwnInfo(oStream);		break;
	case SC_get_friend:			msg_get_friend(oStream);		break;
	case SC_Chat_text:			msg_chat_text(oStream);			break;
	case SC_Chat_voice:			msg_chat_voice(oStream);		break;
	case SC_Chat_img:			msg_chat_img(oStream);			break;
	default:
		break;
	}
}

void Client::on_Send_clicked()
{
	QByteArray datagram;
	QDataStream iStream(&datagram, QIODevice::WriteOnly);

	QString str= get_Chat_text();
	if (!str.isEmpty())
	{
		iStream << CS_Chat_text << str;
		Socket->write(datagram);
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("The message was empty!"), QMessageBox::Yes);
	}
}
void Client::on_Close_clicked()
{
	SendBasicMsg(CS_Logout);

	this->destroy();
}

QString Client::get_Chat_text()  //获得要发送的信息
{
	QString msg = ui.textEdit_Send->toHtml();

	ui.textEdit_Send->clear();
	ui.textEdit_Send->setFocus();
	return msg;
}

void Client::msg_participant_join(QDataStream &oStream)
{

}
void Client::msg_participant_left(QDataStream &oStream)
{

}
void Client::msg_get_OwnInfo(QDataStream &oStream)
{
	oStream >> uid >> Username;
}
void Client::msg_get_friend(QDataStream &oStream)
{
	QString name;
	quint32 count,id,i=0;
	oStream >> count;
	while (count--)
	{
		oStream >> id >> name;
		map_friends.insert(id, name);

		ui.table_users->insertColumn(0);
		ui.table_users->setItem(0, 0, new QTableWidgetItem(id));
		ui.table_users->setItem(0, 1, new QTableWidgetItem(name));
	}
}
void Client::msg_chat_text(QDataStream &oStream)
{
	quint32 id;
	QString strTime, strChatText,str;

	oStream >> id >> strTime >> strChatText;
	str = map_friends.find(id).value();
	str += " " + strTime + " :" + strChatText;
	ui.textBrowser_Display->append(str);
}
void Client::msg_chat_voice(QDataStream &oStream)
{

}
void Client::msg_chat_img(QDataStream &oStream)
{

}
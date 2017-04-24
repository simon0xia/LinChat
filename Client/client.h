#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include "ui_client.h"
#include <QtNetwork>
#include "MessageDefine.h"

class Client : public QDialog
{
	Q_OBJECT

public:
	Client(QTcpSocket *s);
	~Client();

private:
	QString get_Chat_text();
	void SendBasicMsg(MessageType msgID);

	void timerEvent(QTimerEvent *);

	void msg_participant_join(QDataStream &oStream);
	void msg_participant_left(QDataStream &oStream);
	void msg_get_OwnInfo(QDataStream &oStream);
	void msg_get_friend(QDataStream &oStream);
	void msg_chat_text(QDataStream &oStream);
	void msg_chat_voice(QDataStream &oStream);
	void msg_chat_img(QDataStream &oStream);


private slots:
	void Socketerror(QAbstractSocket::SocketError);

	void on_button_Send_clicked();
	void on_Button_Close_clicked();

	void receiveMessage();

private:
	Ui::ClientClass ui;

	QTcpSocket *Socket;
	qint16 port;

	quint32 uid;
	QString Username;

	QMap<quint32, QString> map_friends;
};

#endif // CLIENT_H

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>

class Client : public QTcpSocket
{
	Q_OBJECT

public:
	Client(QObject *parent);
	~Client();

signals:
	void CloseSignal(qintptr);

private slots:
	void receiveMessage();
	void ClientClose();
	void Socketerror(QAbstractSocket::SocketError);

private:
	void timerEvent(QTimerEvent *);

	void msg_login(QDataStream &oStream);
	void msg_logout(QDataStream &oStream);
	void msg_chat_text(QDataStream &oStream);
	void msg_chat_voice(QDataStream &oStream);
	void msg_chat_img(QDataStream &oStream);

private:
	const qint16 life = 10;

	bool isLogin;
	qint32 UID;
	qint16 heartbeat;
};

#endif // CLIENT_H

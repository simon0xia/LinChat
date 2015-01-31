#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject *parent, QTcpSocket *socket);
	~Client();
	bool isLive(void) { return heartbeat > 0 ? true : false; }

public slots:
	QTcpSocket *getSocket(void) { return s; }
	void processPendingDatagrams();

private:
	void timerEvent(QTimerEvent *);

	bool checkLogin(qint32 id, QString &password);

private:
	const qint16 life = 10;

	bool isLogin;
	qint32 UID;
	qint16 heartbeat;
	QTcpSocket *s;
};

#endif // CLIENT_H

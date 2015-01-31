#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets/QWidget>
#include "ui_client.h"
#include <QtNetwork/QtNetwork>
#include "MessageDefine.h"

class Client : public QWidget
{
	Q_OBJECT

public:
	Client(QWidget *parent = 0);
	~Client();

private:
	QString getUserName();
	QString getMessage();
	QString getIP();

	void timerEvent(QTimerEvent *);

	void sendMessage(MessageType type, QString serverAddress = "");
	void newParticipant(QString userName, QString ipAddress);
	void participantLeft(QString userName);

private slots:
	void on_Send_clicked();
	void on_Close_clicked();

	void processConnection();

private:
	Ui::ClientClass ui;

	QTcpSocket *Socket;
	qint16 port;

	QString IP, Username;
};

#endif // CLIENT_H

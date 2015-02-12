#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "ui_login.h"
#include <QtNetwork>
#include "MessageDefine.h"

class CLogin : public QDialog
{
	Q_OBJECT

public:
	CLogin(QWidget *parent = 0);
	CLogin(QTcpSocket *s);
	~CLogin();

private slots:
	void on_button_login_clicked();
	void receiveMessage();

private:
	void msg_login(QDataStream &oStream);

private:
	Ui::Login ui;

	QTcpSocket *Socket;
	qint16 port;
};

#endif // LOGIN_H

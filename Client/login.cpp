#include "login.h"
#include <QMessageBox.h>

CLogin::CLogin(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	port = 7750;
}

CLogin::CLogin(QTcpSocket *s)
:Socket(s)
{
	ui.setupUi(this);

	port = 7750;
}

CLogin::~CLogin()
{

}

void CLogin::receiveMessage(void)
{
	QDataStream oStream(Socket);
	qint32 msgID;
	oStream >> msgID;

	switch (msgID)
	{
	case SC_Login: msg_login(oStream); break;
	default:
		break;
	}
}

void CLogin::on_button_login_clicked()
{
	QHostAddress addr(ui.edit_addr->text());

	Socket->connectToHost(addr, port);

	connect(Socket, SIGNAL(connected()), this, SLOT(processConnection()));
	connect(Socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

	quint32 uid = ui.edit_uid->text().toUInt();
	QString pwd = ui.edit_password->text();

	QByteArray datagram;
	QDataStream iStream(&datagram, QIODevice::WriteOnly);
	iStream << CS_Login << uid << pwd;
	Socket->write(datagram);
}

void CLogin::msg_login(QDataStream &oStream)
{
	bool bRes;
	oStream >> bRes;
	if (!bRes)
	{
		QMessageBox::warning(this, tr("Warning"), tr("username or password error!"), QMessageBox::Yes);
		ui.edit_uid->setText("");
		ui.edit_password->setText("");
		return;
	}

	accept();
}
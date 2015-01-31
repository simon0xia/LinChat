#include "client.h"
#include <QMessageBox>
#include <QScrollBar>

Client::Client(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.textEdit_Send->setFocusPolicy(Qt::StrongFocus);
	ui.textBrowser_Display->setFocusPolicy(Qt::NoFocus);

	port = 7750;
	QHostAddress addr("192.168.10.119");

	Socket = new QTcpSocket;
	Socket->connectToHost(addr, port);
	connect(Socket, SIGNAL(connected()), this, SLOT(processConnection()));
	connect(Socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

Client::~Client()
{

}

void Client::processConnection()
{
	qint32 id = qrand() % 10;
	QString password = "123456";
	QByteArray datagram;
	QDataStream iStream(&datagram, QIODevice::WriteOnly);
	iStream << Login << id << password;
	Socket->write(datagram);

	startTimer(1000);
}

void Client::timerEvent(QTimerEvent *t)
{
	QByteArray datagram;
	QDataStream iStream(&datagram, QIODevice::WriteOnly);
	iStream << Heartbeat;
	Socket->write(datagram);
}


void Client::processPendingDatagrams()
{
	QByteArray datagram = Socket->readAll();
	QDataStream oStream(&datagram, QIODevice::ReadOnly);

	qint32 ID;
	oStream >> ID;

	QString strTmp;
	qint32 n;
	bool bLogin;
	switch (ID)
	{
	case Ack_Login:
		oStream >> bLogin;
		if (!bLogin)
		{
			oStream >> n;
			QMessageBox::information(this, tr("Login Fail"), tr("Login Fail, reason:%1").arg(n));
			exit(0);
		}
		break;


	default:
		break;
	}
}

void Client::sendMessage(MessageType type, QString serverAddress)
{

}

void Client::on_Send_clicked()
{
	sendMessage(Message);
}
void Client::on_Close_clicked()
{
	this->destroy();
}

QString Client::getIP()  //获取ip地址
{
	QList<QHostAddress> list = QNetworkInterface::allAddresses();
	foreach(QHostAddress address, list)
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
			return address.toString();
	}
	return 0;
}


QString Client::getUserName()
{
	QStringList envVariables;
	envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
		<< "HOSTNAME.*" << "DOMAINNAME.*";
	QStringList environment = QProcess::systemEnvironment();
	foreach(QString string, envVariables)
	{
		int index = environment.indexOf(QRegExp(string));
		if (index != -1)
		{
			QStringList stringList = environment.at(index).split('=');
			if (stringList.size() == 2)
			{
				return stringList.at(1);
				break;
			}
		}
	}
	return false;
}


QString Client::getMessage()  //获得要发送的信息
{
	QString msg = ui.textEdit_Send->toHtml();

	ui.textEdit_Send->clear();
	ui.textEdit_Send->setFocus();
	return msg;
}
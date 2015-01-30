#include "client.h"
#include <QMessageBox>
#include <QScrollBar>

Client::Client(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.textEdit_Send->setFocusPolicy(Qt::StrongFocus);
	ui.textBrowser_Display->setFocusPolicy(Qt::NoFocus);

	udpSocket = new QUdpSocket(this);
	port = 7000;
	udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	
	QList<QHostAddress> list = QNetworkInterface::allAddresses();
	foreach(QHostAddress address, list)
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol){
			IP = address.toString();
			break;
		}
	}
	Username = getUserName();

	connect(ui.button_Send, SIGNAL(clicked()), this, SLOT(on_Send_clicked()));
	connect(ui.Button_Close, SIGNAL(clicked()), this, SLOT(on_Send_clicked()));

	sendMessage(NewParticipant);
}

Client::~Client()
{

}

void Client::processPendingDatagrams()
{
	while (udpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size());
		QDataStream oStream(&datagram, QIODevice::ReadOnly);
		int messageType;
		oStream >> messageType;
		QString userName, ipAddress, message;
		QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		switch (messageType)
		{
			case Message:
			{
				oStream >> userName >> ipAddress >> message;
				ui.textBrowser_Display->setTextColor(Qt::blue);
				ui.textBrowser_Display->setCurrentFont(QFont("Times New Roman", 12));
				ui.textBrowser_Display->append("[ " + userName + " ] " + time);
				ui.textBrowser_Display->append(message);
				break;
			}
			case NewParticipant:
			{
				oStream >> userName >> ipAddress;
				newParticipant(userName, ipAddress);
				break;
			}
			case ParticipantLeft:
			{
				oStream >> userName;
				participantLeft(userName);
				break;
			}
		}
	}
}

void Client::newParticipant(QString userName, QString ipAddress)
{
	bool bb = ui.table_users->findItems(userName, Qt::MatchExactly).isEmpty();
	if (bb)
	{
		ui.table_users->insertRow(0);
		ui.table_users->setItem(0, 0, new QTableWidgetItem(userName));
		ui.table_users->setItem(0, 1, new QTableWidgetItem(ipAddress));
		ui.textBrowser_Display->setTextColor(Qt::gray);
		ui.textBrowser_Display->setCurrentFont(QFont("Times New Roman", 10));
		ui.textBrowser_Display->append(tr("%1 online!").arg(userName));
		ui.lable_users->setText(tr("online user's: %1").arg(ui.table_users->rowCount()));
		
		sendMessage(NewParticipant);	//如果不加这一句，那么他将看不到前面的人。
	}
}

void Client::participantLeft(QString userName)
{
	int rowNum = ui.table_users->findItems(userName, Qt::MatchExactly).first()->row();
	ui.table_users->removeRow(rowNum);
	ui.textBrowser_Display->setTextColor(Qt::gray);
	ui.textBrowser_Display->setCurrentFont(QFont("Times New Roman", 10));
	ui.textBrowser_Display->append(tr("%1 left!").arg(userName));
	ui.lable_users->setText(tr("online user's: %1").arg(ui.table_users->rowCount()));
}

void Client::sendMessage(MessageType type, QString serverAddress)
{
	QByteArray data;
	QDataStream iStream(&data, QIODevice::WriteOnly);
	iStream << type << Username;

	switch (type)
	{
		case ParticipantLeft:
		{
			break;
		}
		case NewParticipant:
		{
			iStream << IP;
			break;
		}

		case Message:
		{
			if (ui.textEdit_Send->toPlainText() == "")
			{
				QMessageBox::warning(0, tr("Waring"), tr("message was empty!"), QMessageBox::Ok);
				return;
			}
			iStream << IP << getMessage();
			ui.textBrowser_Display->verticalScrollBar()->setValue(ui.textBrowser_Display->verticalScrollBar()->maximum());
			break;
		}
	}
	udpSocket->writeDatagram(data, data.length(), QHostAddress::Broadcast, port);
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
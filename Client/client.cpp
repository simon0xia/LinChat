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
	
	sendMessage(NewParticipant);

	connect(ui.button_Send, SIGNAL(clicked()), this, SLOT(on_Send_clicked()));
	connect(ui.Button_Close, SIGNAL(clicked()), this, SLOT(on_Send_clicked()));
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
		QDataStream in(&datagram, QIODevice::ReadOnly);
		int messageType;
		in >> messageType;
		QString userName, ipAddress, message;
		QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		switch (messageType)
		{
			case Message:
			{
				in >> userName >> ipAddress >> message;
				ui.textBrowser_Display->setTextColor(Qt::blue);
				ui.textBrowser_Display->setCurrentFont(QFont("Times New Roman", 12));
				ui.textBrowser_Display->append("[ " + userName + " ] " + time);
				ui.textBrowser_Display->append(message);
				break;
			}
			case NewParticipant:
			{
				in >> userName >> ipAddress;
				newParticipant(userName, ipAddress);
				break;
			}
			case ParticipantLeft:
			{
				in >> userName;
				participantLeft(userName);
				break;
			}
		}
	}
}


//处理新用户加入
void Client::newParticipant(QString userName, QString ipAddress)
{
	bool bb = ui.table_users->findItems(userName, Qt::MatchExactly).isEmpty();
	if (bb)
	{
		QTableWidgetItem *user = new QTableWidgetItem(userName);
		QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
		ui.table_users->insertRow(0);
		ui.table_users->setItem(0, 0, user);
		ui.table_users->setItem(0, 1, ip);
		ui.textBrowser_Display->setTextColor(Qt::gray);
		ui.textBrowser_Display->setCurrentFont(QFont("Times New Roman", 10));
		ui.textBrowser_Display->append(tr("%1 online!").arg(userName));
		ui.lable_users->setText(tr("online user's: %1").arg(ui.table_users->rowCount()));
		sendMessage(NewParticipant);
	}
}

//处理用户离开
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
	QDataStream out(&data, QIODevice::WriteOnly);
	QString address = getIP();
	out << type << getUserName();


	switch (type)
	{
		case ParticipantLeft:
		{
			break;
		}
		case NewParticipant:
		{
			out << address;
			break;
		}

		case Message:
		{
			if (ui.textEdit_Send->toPlainText() == "")
			{
				QMessageBox::warning(0, tr("Waring"), tr("message was empty!"), QMessageBox::Ok);
				return;
			}
			out << address << getMessage();
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


QString Client::getUserName()  //获取用户名
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
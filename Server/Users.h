#ifndef _USERS_H
#define _USERS_H

#include <QObject>

struct AccountData
{
	quint32 ID;
	QString Name;
	QString Password;
};

const qint16 AccountNO = 10;

AccountData Account[AccountNO] = {
	{ 1, "John", "123456", },
	{ 2, "Michelle", "123456",},
	{ 3, "Amy", "123456", },
	{ 4, "Kim", "123456", },
	{ 5, "Mary", "123456", },
	{ 6, "David", "123456", },
	{ 7, "Sunny", "123456", },
	{ 8, "James", "123456", },
	{ 9, "Maria", "123456", },
	{ 10, "Michael", "123456"}
};

QString getName(quint32 uid)
{
	for (quint16 i = 0; i < AccountNO; i++)
	{
		if (Account[i].ID == uid)
		{
			return Account[i].Name;
		}
	}
	return "";
}

#endif //#ifndef _USERS_H
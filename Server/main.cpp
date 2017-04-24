
#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//确保只运行一次
	QSystemSemaphore sema("ChatServerKey", 1, QSystemSemaphore::Open);
	//在临界区操作共享内存
	sema.acquire();
	QSharedMemory mem("ChatServerObject");

	// 如果全局对象已存在则退出
	if (!mem.create(1))
	{
		qDebug() << "A server has already been running.";
		sema.release();
		return 0;
	}
	sema.release();
	Server server();

	return a.exec();
}

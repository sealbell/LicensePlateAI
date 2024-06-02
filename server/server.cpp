#include "server.h"
#include "clientwork.h"
#include <QThread>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::startServer(int port)
{
    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "服务器无法启动！";
    } else {
        qDebug() << "服务器已启动并侦听端口" << port;
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "TCP连接" << endl;
    // 每当有新连接时，此方法被调用，我们为每个连接创建一个ClientWork对象和一个新线程
    QThread *thread = new QThread;
    ClientWork *worker = new ClientWork(nullptr, socketDescriptor);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &ClientWork::process);
    connect(worker, &ClientWork::finished, thread, &QThread::quit);
    connect(worker, &ClientWork::finished, worker, &ClientWork::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

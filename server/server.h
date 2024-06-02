#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer(int port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H

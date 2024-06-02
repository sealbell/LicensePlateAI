#ifndef CLIENTWORK_H
#define CLIENTWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <iostream>

class ClientWork : public QObject
{
    Q_OBJECT
public:
    explicit ClientWork(QObject *parent, qintptr socketDescriptor);
    void callBaiduAi(const QString &imageData);

signals:
    void finished();

public slots:
    void process();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QString result;

};

#endif // CLIENTWORK_H

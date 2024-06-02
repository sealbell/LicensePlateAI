#ifndef TCP_H
#define TCP_H

#include "camera.h"
#include "ui_camera.h"

#include <QWidget>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib> // for base64 encoding functions
#include <algorithm>
#include <vector>
#include <QString>
#include <QDebug>
#include <QImage>
#include <QByteArray>
#include <QFile>
#include <QBuffer>

#include <fcntl.h>
#include <termios.h>

#define SERVER_IP "YOUR_SERVER_IP"
#define SERVER_PORT 填写你服务器的端口
#define BUFFER_SIZE 1024
#define SHARED_SIZE 1024 * 1024

namespace Ui {
class Tcp;
}

class Tcp : public QWidget
{
    Q_OBJECT

public:
    explicit Tcp(QWidget *parent = nullptr);
    ~Tcp();

signals:
    void result(QString data);

private slots:
    int text();

private:
    Ui::Tcp *ui;
};

QString t_text();

#endif // TCP_H

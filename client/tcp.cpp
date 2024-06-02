#include "tcp.h"
#include "ui_tcp.h"

Tcp::Tcp(QWidget *parent) : QWidget(parent),
                            ui(new Ui::Tcp)
{
    ui->setupUi(this);
}

Tcp::~Tcp()
{
    delete ui;
}

int Tcp::text()
{
    QString data = t_text();
    // qDebug()<<"2"<<endl;

    emit result(data);
    return 0;
}

QString t_text()
{
    // 创建tcp套接字
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sfd >= 0);
    // 定义服务器地址
    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    // ip协议
    ser_addr.sin_family = AF_INET;
    // 端口号
    ser_addr.sin_port = htons(SERVER_PORT);
    //! ip地址
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // 连接服务器
    int ret = connect(sfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    // qDebug()<<"3"<<endl;
    //  判断连接
    if (ret != 0)
    {
        perror("connect error");
        // 关闭套接字
        close(sfd);
        return "fail";
    }
    else
    {
        // printf("连接成功");
        // qDebug()<<"4"<<endl;
    }
    // qDebug()<<"5"<<endl;
    //  打开文件图片
    const char *image_file_path = "path/to/your/image.jpg";
    FILE *image_file = fopen(image_file_path, "rb"); // 以二进制读模式打开
    if (!image_file)
    {
        perror("Failed to open image file");
        exit(1);
    }
    // qDebug()<<"6"<<endl;
    //  读取图片数据并发送到服务器
    char buf[BUFFER_SIZE];   // 定义缓存区用于临时存储读取的图片数据
    bzero(buf, BUFFER_SIZE); // 清空缓存区
    size_t read_size;        // 定义读取到的数据大小
    while (!feof(image_file))
    {
        // 读取文件内容
        read_size = fread(buf, 1, BUFFER_SIZE, image_file);
        // 写入sfd
        if (read_size != write(sfd, buf, read_size))
        {
            perror("Failed to send image data");
            exit(1);
        }
    }
    fclose(image_file); // 关闭文件
                        // qDebug()<<"7"<<endl;

    // 定义缓冲区，用于储存收到的数据
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read;
    // 循环接收 直到接收完整的车牌数据包为止

    // 将接收到的数据存入buffer中
    bytes_read = recv(sfd, buffer, BUFFER_SIZE - 1, 0);
    // qDebug()<<"8"<<endl;
    //  判断是否有接收到数据
    if (bytes_read < 0)
    {
        // 表示接收数据发生错误
        perror("recv");
        // 关闭套接字
        close(sfd);
    }
    else if (bytes_read == 0)
    {
        // 返回为0表示服务器已关闭连接
        // 关闭套接字
        close(sfd);
    }
    // 在接收到的数据末尾添加空字符，作为字符串处理
    buffer[bytes_read] = '\0';

    // printf("Received: %s\n", buffer);
    // qDebug()<<buffer<<endl;

    // camera *c=new camera;

    QString data = QString::fromUtf8(buffer);

    // c->ui->chepai_result->setText(data);

    close(sfd);

    return data;
}

#include "camera.h"
#include "ui_camera.h"

camera::camera(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::camera)
{
    ui->setupUi(this);

    v4l2_dev_init();
    v4l2_stream_on();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(video_show()));

    Tcp *t = new Tcp;
    connect(this, SIGNAL(send()), t, SLOT(text()));

    connect(t, &Tcp::result, this, &camera::setresult);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        // qDebug()<<1<<endl;
        if (serial.open(QIODevice::ReadWrite))
        {
            // qDebug()<<2<<endl;
            // ui->comboBox->addItem(serial.portName());
            serial.close();
        }
    }
}

camera::~camera()
{
    delete ui;
}

int camera::v4l2_dev_init()
{
    // 打开摄像头
    v4l2_fd = open("/dev/video0", O_RDWR);
    if (v4l2_fd < 0)
    {
        printf("open camera failed\n");
        return -1;
    }
    // 获取摄像头能力
    struct v4l2_capability cap;
    // 查询设备功能
    ioctl(v4l2_fd, VIDIOC_QUERYCAP, &cap);
    // 判断是否是视频采集设备
    if (!(V4L2_CAP_VIDEO_CAPTURE & cap.capabilities))
    {
        printf("Error:No capture video device!\n");
        return -1;
    }
    // 调用函数，设置摄像头格式
    if (v4l2_set_format() < 0)
    {
        printf("set format failed\n");
        return -1;
    }

    return 0;
}

int camera::v4l2_set_format()
{
    struct v4l2_format fmt;
    // 设置视频流
    struct v4l2_streamparm streamparm;

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;       // 摄像头采集
    fmt.fmt.pix.width = PIXWIDTH;                 // 设置宽
    fmt.fmt.pix.height = PIXHEIGHT;               // 设置高
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; // 选择MJPEG
    // 设置视频设备的格式
    if (ioctl(v4l2_fd, VIDIOC_S_FMT, &fmt) < 0)
    {
        printf("set format failed\n");
        return -1;
    }

    // 判断是否已经设置为我们要求的MJPEG像素格式,否则表示该设备不支持MJPEG像素格式
    if (V4L2_PIX_FMT_MJPEG != fmt.fmt.pix.pixelformat)
    {
        printf("Error: the device does not support MJPEG format!\n");
        return -1;
    }
    // 获取streamparm
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(v4l2_fd, VIDIOC_G_PARM, &streamparm) < 0)
    {
        printf("get parm failed\n");
        return -1;
    }

    // 帧率设置
    if (V4L2_CAP_TIMEPERFRAME & streamparm.parm.capture.capability)
    {
        // 设置摄像头的帧捕获参数 1/30
        streamparm.parm.capture.timeperframe.numerator = 1;
        streamparm.parm.capture.timeperframe.denominator = 30; // 30fps
        if (ioctl(v4l2_fd, VIDIOC_S_PARM, &streamparm) < 0)
        {
            printf("Error:device do not support set fps\n");
            return -1;
        }
    }

    return 0;
}

int camera::v4l2_init_buffer()
{
    struct v4l2_requestbuffers reqbuf;
    reqbuf.count = FRAMEBUFFER_COUNT; // 帧缓冲的数量
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    // 获取视频设备缓冲区
    if (ioctl(v4l2_fd, VIDIOC_REQBUFS, &reqbuf) < 0)
    {
        printf("request buffer failed\n");
        return -1;
    }
    // 建立内存映射
    struct v4l2_buffer buf;
    unsigned int n_buffers = 0;
    // 存储多个帧缓冲区的信息,初始化为0
    buffer_infos = (struct buffer_info *)calloc(FRAMEBUFFER_COUNT, sizeof(struct buffer_info));
    // 最大帧缓冲区数量
    for (n_buffers = 0; n_buffers < FRAMEBUFFER_COUNT; n_buffers++)
    {
        memset(&buf, 0, sizeof(buf));
        buf.index = n_buffers;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        // 查询视频缓冲区的信息
        if (ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buf) < 0)
        {
            printf("VIDIOC_QUERYBUF failed\n");
            return -1;
        };
        // 创建一个共享内存区域，存储捕获的图像数据
        buffer_infos[n_buffers].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2_fd, buf.m.offset);
        buffer_infos[n_buffers].length = buf.length;

        if (MAP_FAILED == buffer_infos[n_buffers].start)
        {
            printf("mmap error\n");
            return -1;
        }
    }

    // 入队
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
    {
        if (ioctl(v4l2_fd, VIDIOC_QBUF, &buf) < 0)
        {
            printf("入队失败\n");
            return -1;
        }
    }

    return 0;
}

int camera::v4l2_stream_on()
{
    // 调用函数，初始化buffer
    if (v4l2_init_buffer() < 0)
    {
        printf("------------------------------------\n");
        return -1;
    }

    // 打开摄像头、摄像头开始采集数据
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(v4l2_fd, VIDIOC_STREAMON, &type) < 0)
    {
        printf("open stream failed\n");
        return -1;
    }

    return 0;
}

int camera::v4l2_get_one_frame(FrameBuffer *framebuf)
{
    // 初始化select()来进行I/O端口复用
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(v4l2_fd, &fds);

    // 设置等待时间为2s
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    select(v4l2_fd + 1, &fds, NULL, NULL, &tv);

    // 将帧缓冲添加到队列
    struct v4l2_buffer one_buf;
    memset(&one_buf, 0, sizeof(one_buf));
    one_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    one_buf.memory = V4L2_MEMORY_MMAP;

    // 从视频缓冲区的输出队列中取得一个已经保存有一帧视频数据的视频缓冲区
    if (ioctl(v4l2_fd, VIDIOC_DQBUF, &one_buf) < 0)
    {
        printf("VIDIOC_DQBUF failed!\n");
        return -1;
    }

    // 将buffer_infos中已使用的字节数copy到framebuf中
    memcpy(framebuf->buf, (char *)buffer_infos[one_buf.index].start, one_buf.bytesused); // bytesused 表示buf中已经使用的字节数
    framebuf->length = one_buf.bytesused;

    if (ioctl(v4l2_fd, VIDIOC_QBUF, &one_buf) == -1)
    {
        printf("VIDIOC_QBUF failed\n");
        return -1;
    }

    return 0;
}

void camera::read()
{
    QByteArray temp = serial->readAll();
    ui->textEdit_2->insertPlainText(temp);
}

void camera::on_open_clicked()
{
    timer_start();
}

void camera::on_close_clicked()
{
    v4l2_device_release();
    this->close();
}

void camera::on_cut_clicked()
{
    FrameBuffer frame;
    // 获取一帧显示
    v4l2_get_one_frame(&frame);

    const char *image_file_path = "path/to/your/image.jpg";
    FILE *file = fopen(image_file_path, "w+");
    fwrite(frame.buf, frame.length, 1, file);
    fclose(file);

    QPixmap pix("/home/edu/image/1.jpg");
    ui->img_show->setPixmap(pix);
}

void camera::video_show()
{
    FrameBuffer frame;
    QPixmap pix;

    // 获取一帧显示
    v4l2_get_one_frame(&frame);
    pix.loadFromData(frame.buf, frame.length);
    pix.scaled(ui->video_show->width(), ui->video_show->height(), Qt::KeepAspectRatio);
    ui->video_show->setPixmap(pix);
}

void camera::timer_start()
{
    // 1000/33约等于30,每一秒显示30帧
    timer->start(30);
}

int camera::v4l2_device_release()
{
    // 释放映射缓冲区
    int i;
    // 关闭定时器
    timer->stop();
    for (i = 0; i < FRAMEBUFFER_COUNT; i++)
    {
        munmap(buffer_infos[i].start, buffer_infos[i].length);
    }
    return 0;
}

void camera::on_send_clicked()
{
    emit this->send();
    // qDebug()<<"1"<<endl;
}

void camera::on_open_dj_clicked()
{
    if (ui->open_dj->text() == tr("open_dj"))
    {
        serial = new QSerialPort;
        // 设置串口名
        serial->setPortName("/dev/ttyUSB0");
        if (serial->open(QIODevice::ReadWrite))
        {
            // qDebug()<<"1"<<endl;
            serial->setBaudRate(QSerialPort::Baud115200); // 设置波特率
            serial->setDataBits(QSerialPort::Data8);      // 设置数据位
            serial->setParity(QSerialPort::NoParity);     // 设置校验位
            serial->setStopBits(QSerialPort::OneStop);    // 设置停止位
            // 设置流控制
            serial->setFlowControl(QSerialPort::NoFlowControl);

            ui->open_dj->setText(tr("close_dj"));

            QObject::connect(serial, &QSerialPort::readyRead, this, &camera::read_data);
        }
    }
    else
    {
        // 关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        ui->open_dj->setText(tr("open"));
    }
}

void camera::on_send_dj_clicked()
{
    serial->write(ui->textEdit->toPlainText().toLatin1());
    // qDebug()<<"发送成功"<<endl;
}

void camera::read_data()
{
    // qDebug()<<"数据来了"<<endl;
    QByteArray buf;
    buf = serial->readAll();
    if (!buf.isEmpty())
    {
        QString str = ui->textEdit_2->toPlainText();
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QString str_buf = codec->toUnicode(buf);
        str = str + '\n' + str_buf;
        ui->textEdit_2->clear();
        ui->textEdit_2->append(str);
    }
    buf.clear();
}

void camera::on_pushButton_clicked()
{
    ui->textEdit_2->clear();
}

void camera::setresult(QString data)
{
    // qDebug()<<data<<endl;
    // data = "aaa";
    ui->chepai_result->setText(data);
}

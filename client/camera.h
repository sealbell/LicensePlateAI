#ifndef CAMERA_H
#define CAMERA_H

#include "tcp.h"

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QTimer>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/videodev2.h>
#include <stdbool.h>
#include <string>

#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextCodec>

/* 命名空间 */
using namespace std;

#define FRAMEBUFFER_COUNT 4
#define PIXWIDTH    800
#define PIXHEIGHT   400

// buffer描述信息
struct buffer_info {
    void *start;
    unsigned int length;
};

//摄像头像素格式及其描述信息
typedef struct camera_format {
    unsigned char description[32];  //字符串描述信息
    unsigned int pixelformat;       //像素格式
} cam_fmt;

//帧描述信息
typedef struct Frame_Buffer{
    unsigned char buf[PIXHEIGHT*PIXWIDTH*3];
    int length;
}FrameBuffer;

namespace Ui {
class camera;
}

class camera : public QWidget
{
    Q_OBJECT

    friend class Tcp;

public:
    explicit camera(QWidget *parent = nullptr);
    ~camera();

    Ui::camera *ui;

    /* v4l2函数 */
        // 初始化
        int v4l2_dev_init();
        // 获取摄像头支持的像素格式
        int v4l2_set_format();
        // 申请内存
        int v4l2_init_buffer();
        // 开启摄像头
        int v4l2_stream_on();
        //采集
        int v4l2_get_one_frame(FrameBuffer * framebuf);


        void read();


signals:
        void send();


private slots:
        void on_open_clicked();

        void on_close_clicked();

        void on_cut_clicked();

        //视频显示
            void video_show();
            //定时器启动
            void timer_start();
            int v4l2_device_release();

            void on_send_clicked();

            void on_open_dj_clicked();

            void on_send_dj_clicked();

            void read_data();

            void on_pushButton_clicked();

       void setresult(QString data);

private:


    /* 摄像头描述符 */
        int v4l2_fd;
        /* 定时器 */
        QTimer *timer;
        /* buffer */
        struct buffer_info *buffer_infos;

        cam_fmt cam_fmts[10];

        QSerialPort *serial;//串口对象

};

#endif // CAMERA_H

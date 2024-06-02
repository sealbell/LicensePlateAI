#include "clientwork.h"
#include <QDebug>

ClientWork::ClientWork(QObject *parent, qintptr socketDescriptor)
    : QObject(parent), socketDescriptor(socketDescriptor) {}

void ClientWork::process()
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit finished();
        return;
    }

    QByteArray imageData;
    connect(socket, &QTcpSocket::readyRead, this, [this, &imageData]() {
        while (socket->bytesAvailable()) {
            QByteArray chunk = socket->read(1024);
            imageData.append(chunk);
            if (chunk.size() < 1024) {
                // 当接收到的数据小于1024字节时，表示数据传输完成
                QString data = QString::fromUtf8(imageData);
                callBaiduAi(data); // 调用处理图片的函数
                qDebug() << result << endl;
                socket->write(result.toUtf8().constData());
                socket->disconnectFromHost();
                return;
            }
        }
    });

    connect(socket, &QTcpSocket::disconnected, this, &ClientWork::finished);
}


//void ClientWork::process()
//{
//    socket = new QTcpSocket();
//    if (!socket->setSocketDescriptor(socketDescriptor)) {
//        emit finished();
//        return;
//    }

//    connect(socket, &QTcpSocket::readyRead, this, [this]() {
//        QString imageData = QString::fromUtf8(socket->readAll());
//        qDebug() << "收到数据:" << imageData << endl;
//        callBaiduAi(imageData); // 调用处理图片的函数
//        //将结果字符串写回客户端
//        qDebug() << result << endl;
//        socket->write(result.toUtf8().constData());
//        socket->disconnectFromHost();
//    });

//    connect(socket, &QTcpSocket::disconnected, this, &ClientWork::finished);
//}

#include <iostream>
#include <curl/curl.h>
#include "aip-cpp-sdk-4.16.5/ocr.h"
#include "aip-cpp-sdk-4.16.5/base/base64.h"

using namespace std;

// 定义CURL写回调函数
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    size_t realSize = size * nmemb;
    userp->append((char*)contents, realSize);
    return realSize;
}

// 调用百度AI的代码，处理imageData
void ClientWork::callBaiduAi(const QString &imageData) {
    const static std::string request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate";

    std::string app_id = "YOUR_APP_ID";
    std::string api_key = "YOUR_API_KEY";
    std::string secret_key = "YOUR_SECRET_KEY";
    std::string access_token = "YOUR_ACCESS_TOKEN";

    // 创建客户端的代码这里不适用，因为我们使用的是CURL直接发送请求
    // aip::Ocr client(app_id, api_key, secret_key);

    std::string url = request_url + "?access_token=" + access_token;
    CURL *curl = NULL;
    CURLcode result_code;
    std::string readBuffer; // 用于存储响应数据

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_httppost *post = NULL;
        curl_httppost *last = NULL;
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "image", CURLFORM_COPYCONTENTS, imageData.toStdString().c_str(), CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // 设置写回调函数的用户数据
        result_code = curl_easy_perform(curl);
        if(result_code != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result_code));
        } else {
            // 将std::string转换为QString并存储在全局变量result中
            result = QString::fromStdString(readBuffer);
        }
        curl_easy_cleanup(curl);
        curl_formfree(post); // 清理表单
    } else {
        fprintf(stderr, "curl_easy_init() failed.\n");
    }
}

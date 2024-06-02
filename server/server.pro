QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        clientwork.cpp \
        main.cpp \
        server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    aip-cpp-sdk-4.16.5/base/base.h \
    aip-cpp-sdk-4.16.5/base/base64.h \
    aip-cpp-sdk-4.16.5/base/http.h \
    aip-cpp-sdk-4.16.5/base/utils.h \
    aip-cpp-sdk-4.16.5/body_analysis.h \
    aip-cpp-sdk-4.16.5/content_censor.h \
    aip-cpp-sdk-4.16.5/face.h \
    aip-cpp-sdk-4.16.5/image_censor.h \
    aip-cpp-sdk-4.16.5/image_classify.h \
    aip-cpp-sdk-4.16.5/image_process.h \
    aip-cpp-sdk-4.16.5/image_search.h \
    aip-cpp-sdk-4.16.5/kg.h \
    aip-cpp-sdk-4.16.5/machine_translation.h \
    aip-cpp-sdk-4.16.5/nlp.h \
    aip-cpp-sdk-4.16.5/ocr.h \
    aip-cpp-sdk-4.16.5/speech.h \
    aip-cpp-sdk-4.16.5/video_censor.h \
    aip-cpp-sdk-4.16.5/voice_censor.h \
    clientwork.h \
    server.h

# 添加头文件路径
INCLUDEPATH += /home/bell/ai_demo/ai_lib/openssl/include/
INCLUDEPATH += /home/bell/ai_demo/ai_lib/curl/include/
INCLUDEPATH += /home/bell/ai_demo/ai_lib/json/include/

# 添加库文件路径
LIBS += -L/home/bell/ai_demo/ai_lib/openssl/lib/
LIBS += -L/home/bell/ai_demo/ai_lib/curl/lib/
LIBS += -L/home/bell/ai_demo/ai_lib/json/lib/
LIBS += -lssl -lcrypto -lcurl -ljsoncpp

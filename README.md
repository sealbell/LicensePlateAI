# LicensePlateAi

这是一个基于Qt的图像处理和通信应用程序，包含前端用户界面和后端服务器，用于通过摄像头捕获图像并通过TCP/IP协议发送图像数据到服务器进行处理。

## 功能特性

- 使用Qt框架开发前端用户界面。
- 通过摄像头捕获图像并显示在界面上。
- 通过TCP/IP协议将图像数据发送到服务器进行处理。
- 处理结果显示在前端界面上。
- 支持通过串口与外部设备通信。
- 后端服务器处理接收的图像数据并调用百度AI接口进行识别。

## 文件结构

```
client/                   # 客户端代码目录
├── camera.cpp            # 摄像头功能实现文件
├── camera.h              # 摄像头功能头文件
├── camera.ui             # 摄像头界面文件
├── client.pro            # Qt项目文件
├── dijia.png             # 图像资源文件
├── dijia1.png            # 图像资源文件
├── main.cpp              # 客户端主程序入口
├── res.qrc               # 资源文件列表
├── tcp.cpp               # TCP通信功能实现文件
├── tcp.h                 # TCP通信功能头文件
├── tcp.ui                # TCP通信界面文件
├── widget.cpp            # 主窗口功能实现文件
├── widget.h              # 主窗口功能头文件
└── widget.ui             # 主窗口界面文件

server/                   # 服务器代码目录
├── aip-cpp-sdk-4.16.5/   # 百度AI SDK相关文件
├── clientwork.cpp        # 客户端工作功能实现文件
├── clientwork.h          # 客户端工作功能头文件
├── main.cpp              # 服务器主程序入口
├── server.cpp            # 服务器功能实现文件
├── server.h              # 服务器功能头文件
└── server.pro            # Qt项目文件

.gitignore                # Git忽略文件列表
README.md                 # 项目说明文件

```

## 文件说明

### 前端

- `client/main.cpp`: 应用程序入口，初始化并显示主窗口。
- `client/widget.h` 和 `client/widget.cpp`: 定义和实现主窗口类`Widget`，处理用户输入并启动摄像头窗口。
- `client/camera.h` 和 `client/camera.cpp`: 定义和实现摄像头窗口类`camera`，包括摄像头的初始化、图像采集和显示功能。
- `client/tcp.h` 和 `client/tcp.cpp`: 定义和实现TCP通信类`Tcp`，包括连接服务器、发送图像数据和接收处理结果。
- `client/*.ui`: Qt Designer生成的用户界面文件。

### 后端

- `server/server.h` 和 `server/server.cpp`: 定义和实现服务器类`Server`，处理客户端连接、接收图像数据并调用百度AI接口进行识别。
- `server/clientwork.h` 和 `server/clientwork.cpp`: 定义和实现客户端工作类，用于处理与客户端的通信和图像数据处理。
- `server/main.cpp`: 服务器程序入口，初始化并启动服务器。
- `server/aip-cpp-sdk-4.16.5/`: 百度AI SDK相关文件。

## 环境配置

### 依赖库

- Qt 5.x
- OpenCV
- libv4l2
- curl
- 百度AI SDK

### 编译与运行

1. 确保已安装上述依赖库。
2. 使用Qt Creator或命令行工具编译项目。

#### 使用Qt Creator

- 打开项目文件（`client.pro` 和 `server.pro`）。
- 点击运行按钮进行编译和运行。

#### 使用命令行

```bash
# 编译客户端
cd client
mkdir build
cd build
qmake ..
make
./client_executable

# 编译服务器
cd ../../server
mkdir build
cd build
qmake ..
make
./server_executable
```

## 使用说明

### 前端

1. 启动应用程序，主窗口将显示。
2. 输入用户名和密码（示例代码中用户名和密码均为`123`）。
3. 点击按钮进入摄像头窗口。
4. 点击“打开”按钮开始摄像头图像采集。
5. 点击“发送”按钮将图像数据发送到服务器。
6. 处理结果将在界面上显示。

### 后端

1. 启动服务器程序，监听客户端连接。
2. 接收来自客户端的图像数据并调用百度AI接口进行识别。
3. 将识别结果发送回客户端。

## 注意事项

- 请确保摄像头设备已连接并正确配置。
- 请确保服务器地址和端口正确无误。
- 保护敏感信息，不要将API密钥和密码等信息硬编码到代码中。

## 贡献

欢迎提交Issue或Pull Request来改进本项目。

## 许可证

该项目使用MIT许可证，详情请参见LICENSE文件。
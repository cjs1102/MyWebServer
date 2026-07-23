# MyWebServer


## 项目简介

MyWebServer 是一个基于 C++17 开发的轻量级 HTTP Server。

项目基于 Linux 网络编程模型，
采用 Reactor 架构，
使用 epoll 实现高并发 IO，
结合线程池提升请求处理能力。


## 技术栈

- C++17
- Linux
- Socket
- epoll
- Reactor
- Multi-thread
- ThreadPool
- CMake
- HTTP/1.1


## 核心模块


### 1. 网络模块

实现 TCP 长连接管理：

- Socket封装
- TcpServer
- TcpConnection
- Channel
- EventLoop


### 2. Reactor事件模型

基于：

epoll + EventLoop

实现：

- IO多路复用
- 非阻塞网络通信
- 事件分发


### 3. 多线程模块

实现：

- Thread
- ThreadPool
- EventLoopThread


支持：

主线程负责accept

工作线程负责IO处理


### 4. HTTP模块

支持：

- GET请求
- POST请求
- 静态资源访问
- HTTP请求解析


### 5. 定时器模块

实现：

- 连接超时检测
- Timer管理


## 项目结构


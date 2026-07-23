# MyWebServer


基于 C++17/Linux 实现的高性能 HTTP Server。

采用 Reactor 网络模型，
基于 Linux epoll 实现 IO 多路复用，
支持多线程 EventLoop、非阻塞 IO、
HTTP 请求解析以及定时器管理。


## Features

- Linux epoll ET模式
- Reactor事件驱动模型
- 主从 Reactor 架构
- 多线程 EventLoop
- eventfd 跨线程唤醒
- 非阻塞 socket
- HTTP/1.1 请求解析
- 静态资源访问
- 定时器管理


## Architecture


                 Main Thread

                    |
                Acceptor

                    |
        ---------------------
        |        |          |
     IO线程   IO线程    IO线程

     EventLoop EventLoop EventLoop



## Tech Stack

Language:
- C++17

System:
- Linux

Network:
- socket
- epoll

Build:
- CMake

Tools:
- Git

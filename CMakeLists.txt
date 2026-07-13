cmake_minimum_required(VERSION 3.10)

# 项目名字
project(MyWebServer)


# 使用C++17
set(CMAKE_CXX_STANDARD 17)


# 头文件路径
include_directories(
    include
)


# 生成可执行文件
add_executable(

    MyWebServer

    src/main.cpp

    src/net/Socket.cpp
)

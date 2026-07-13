cmake_minimum_required(VERSION 3.10)


project(MyWebServer)


# 使用C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Debug模式生成调试信息
set(CMAKE_BUILD_TYPE Debug)


# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)


# 收集源码
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)


# 生成可执行文件
add_executable(
    MyWebServer
    ${SOURCES}
)


# 多线程库
find_package(Threads REQUIRED)


target_link_libraries(
    MyWebServer
    Threads::Threads
)
#include "net/Socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

// Linux系统socket相关函数
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

using namespace std;

Socket::Socket()
{
	// 创建TCP socket
	fd_ = socket(

		AF_INET, // IPv4

		SOCK_STREAM, // TCP

		0 // 自动选择协议
	);

	if (fd_ < 0)
	{
		cout << "socket create failed" << endl;
	}
	else
	{
		cout << "socket create success" << endl;
	}
}

Socket::~Socket()
{
	// 关闭文件描述符
	if (fd_ >= 0)
	{
		close(fd_);
		std::cout
			<< "socket closed"
			<< std::endl;
	}
}

int Socket::fd() const
{
	return fd_;
}

void Socket::bind(uint16_t port)
{
	// 创建IPv4地址结构体
	sockaddr_in addr;
	addr.sin_family = AF_INET;		   // IPv4
	addr.sin_port = htons(port);	   // 端口号 转换为网络字节序
	addr.sin_addr.s_addr = INADDR_ANY; // 绑定到所有网卡

	// 绑定socket和地址
	int ret = ::bind(fd_, (sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
	{
		cout << "bind failed" << endl;
	}
	else
	{
		cout << "bind success" << endl;
	}
}

void Socket::listen(int backlog)
{
	int ret = ::listen(fd_, backlog);
	if (ret < 0)
	{
		cout << "listen failed" << endl;
	}
	else
	{
		cout << "listen success" << endl;
	}
}

int Socket::accept()
{
	int client_fd = ::accept(fd_, nullptr, nullptr);
	if (client_fd < 0)
	{
		cout << "accept failed" << endl;
		return -1;
	}

	cout << "client accepted" << endl;

	return client_fd;
}

void Socket::setNonBlocking()
{

	int flags =
		fcntl(
			fd_,
			F_GETFL,
			0);

	if (flags == -1)
	{
		return;
	}

	fcntl(
		fd_,
		F_SETFL,
		flags | O_NONBLOCK);
}
void Socket::setNonBlocking(int fd)
{

	int flags =
		fcntl(
			fd,
			F_GETFL,
			0);

	if (flags == -1)
	{
		return;
	}

	fcntl(
		fd,
		F_SETFL,
		flags | O_NONBLOCK);
}
#include "net/Socket.h"

#include <sys/socket.h>

#include <unistd.h>

#include <iostream>

using namespace std;

Socket::Socket()
{
	// 创建TCP socket
	sockfd = socket(

		AF_INET, // IPv4

		SOCK_STREAM, // TCP

		0 // 默认协议
	);

	if (sockfd < 0)
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
	close(sockfd);
}

int Socket::fd()
{
	return sockfd;
}

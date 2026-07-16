#include "net/Socket.h"

#include "iostream"
#include "unistd.h"

using namespace std;

// 程序入口
int main()
{
	// 创建Socket对象
	Socket socket;

	socket.bind(8080);

	socket.listen(128);

	int client = socket.accept();

	char buffer[1024];

	int n = read(client, buffer, sizeof(buffer));
	if (n > 0)
	{
		cout.write(buffer, n);
	}
	return 0;
}

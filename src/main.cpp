#include "net/TcpServer.h"
#include "log/Logger.h"
int main()
{

	TcpServer server(8080);

	Logger::info("MyWebServer start...");

	server.start();

	return 0;
}

#include "TcpServer.hh"
#include "debug.h"

int main()
{
	TcpServer MyTcpServer;
	MyTcpServer.doEventLoop();
	return 0;
}
#ifndef _TCP_SERVER_HH
#define _TCP_SERVER_HH

#ifndef _SOCKET_HH
#include "Socket.hh"
#endif

const PortNumBits kRecvPort = 5001;

class TcpServer{
public:
	virtual ~TcpServer();
	void Listen();
	int Accept(int sock);
	//int incomingConnectionHandler();
	Boolean getConnectStatus();
	virtual void doEventLoop(int timeout = 1000);
//protected:
	TcpServer(PortNumBits = kRecvPort);
private:
Socket fSocket;
SocketNumBits fClientSocket1;
SocketNumBits fClientSocket2;
};

#endif
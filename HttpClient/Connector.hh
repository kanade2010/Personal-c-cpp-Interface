#ifndef _CONNECTOR_HH
#define _CONNECTOR_HH
#include "InetAddress.hh"
#include <string>

extern int g_sockfd;

class Connector{
public:
	Connector(const InetAddress &serverAddr);
	Connector(const std::string domain);
	~Connector();
	
	const InetAddress& serverAddress() const { return m_serverAddr; }
	
	void connect();

private:
	InetAddress m_serverAddr;
};



#endif
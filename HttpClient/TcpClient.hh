#ifndef _TCPCLIENT_HH
#define _TCPCLIENT_HH

#include "Connector.hh"
#include <memory>
#include <string>

typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient
{
public:
	TcpClient(const InetAddress& serverAddr);
	TcpClient(const std::string& domain);
	~TcpClient();

	void connect();

private:
	ConnectorPtr m_connector;
};


#endif
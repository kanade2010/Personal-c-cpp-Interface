#include "TcpClient.hh"


TcpClient::TcpClient(const InetAddress &serverAddr)
	:m_connector(new Connector(serverAddr))
{

}

TcpClient::~TcpClient()
{

}

void TcpClient::connect()
{
	m_connector->connect();
}
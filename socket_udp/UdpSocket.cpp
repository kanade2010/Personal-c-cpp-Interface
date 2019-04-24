/*UdpSocket.cpp
*********************************************
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <unistd.h>
#include "UdpSocket.hpp"

UdpSocket::UdpSocket()
{
  bzero(clientip, 100);
  memset(buf, 0, kBufferSize);
  iClinetPort  = -1 ;
}


/*return 0 success;
* return -1 create socket error;
* return -2 bind socket error;
*/
ssize_t UdpSocket::Create(int Port, std::string strIp, bool reuse_addr)
{
  if(Port < 0 )
  {
     return -1;
  }
  int opt = 1;
  client.sin_family = AF_INET;
  client.sin_port   = htons(Port); 
  if(strIp.empty())
  {
    client.sin_addr.s_addr = INADDR_ANY;//bind ip owner to system;
  }
  else
  {
    client.sin_addr.s_addr = inet_addr(strIp.c_str());
  }
  serverfd = socket(AF_INET, SOCK_DGRAM, 0); //udp   
  if( -1 == serverfd ) {
    printf("create socket failed %s\n", strerror(errno));
    return -1;
  }

  if(reuse_addr)
	{
	  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}
  
  ret = bind(serverfd, (struct sockaddr *)&client, sizeof(client));
  if ( -1 == ret){
    printf("bind failed %s \n", strerror(errno));
    return -2;
  }
  
  return 0;//success!
}

size_t UdpSocket::ReceiveForm()
{
  memset(clientip, 0, 32);
  sock_len = sizeof(client);
  ret = recvfrom( serverfd, buf, kBufferSize, 0, (struct sockaddr*)&client, &sock_len);
  inet_ntop(AF_INET, &client.sin_addr.s_addr, clientip, 32);
  iClinetPort = ntohs(client.sin_port);

  printf("%s %d\n", clientip, iClinetPort);

  buf_len = ret;
  return ret;
}

char* UdpSocket::GetClientIp()
{
  return  clientip;
}

size_t UdpSocket::GetClientPort()
{
  return  iClinetPort;
}

char * UdpSocket::GetRevBuffer()
{
  return buf;
}

size_t UdpSocket::GetBufferLength()
{
  return buf_len;
}

size_t UdpSocket::SendTo(const char* msg, size_t msg_len, char*ClinetIP, int Port)
{
  if(!msg || !ClinetIP ||  Port < 0 )
  {
   return -1;
  }
  server.sin_family = AF_INET;
  server.sin_port   = htons(Port); 
  server.sin_addr.s_addr = inet_addr(ClinetIP);	
  ret = sendto(serverfd, msg , msg_len , 0, (struct sockaddr*)&server, sizeof(server));
  return ret;
}

size_t UdpSocket::Close()
{
  return ::close(serverfd);
}

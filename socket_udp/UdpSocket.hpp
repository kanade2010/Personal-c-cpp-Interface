#ifndef _UdpSocket
#define _UdpSocket

#include <netinet/in.h>
#include <string>

#define kBufferSize 4096

class UdpSocket
{
public:
  UdpSocket();

  ssize_t Create(int Port, std::string strIp = "", bool reuse_addr = true);
  
  size_t ReceiveForm();

  char* GetRevBuffer();
  size_t GetBufferLength();

  char* GetClientIp();
  size_t GetClientPort();

  size_t SendTo(const char* msg, size_t msg_len, char*ClinetIP, int Port);

  size_t Close();
private:

   struct sockaddr_in server, client;
   //struct sockaddr_in broadcast;
   char   clientip[32];
   size_t iClinetPort;
   char   buf[kBufferSize];//udp
   int    serverfd;
   int    ret;
   size_t buf_len;
   socklen_t sock_len;
};

#endif


#include <errno.h>
#include <thread>
#include <strings.h>
#include <poll.h>
#include "EventLoop.hh"
#include "Channel.hh"
#include "Poller.hh"
#include "Logger.hh"
#include "Connector.hh"
#include "SocketHelp.hh"
#include "InetAddress.hh"
#include "TcpClient.hh"
#include "TcpConnection.hh"
#include "Buffer.hh"
#include "HttpRequest.hh"

EventLoop* g_loop;

void newConnetion(const TcpConnectionPtr& conn)
{
  LOG_DEBUG << "newConnetion() : Connected a new connection.";
  conn->send("img.zcool.cn");
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, ssize_t len)
{
  LOG_DEBUG << "onMessage() : received " << buf->readableBytes() << " Bytes from connection [" << conn->name();
  LOG_DEBUG << "onMessage : " << buf->retrieveAsString(len);

}

void runLoop()
{
  EventLoop loop;
  g_loop = &loop;
  g_loop->loop();
}

void downImage()
{
  HttpUrl url("http://img.zcool.cn/community/01ddc256eb71586ac7257d209712b7.jpg@1280w_1l_2o_100sh.jpg");
  std::string ip = url.toIp();
  LOG_DEBUG << "toIp : " << ip;

  InetAddress serverAddr(ip, 80);

  HttpRequest httpReq(g_loop, serverAddr);
  httpReq.setRequestMethod("GET", url);
  httpReq.setRequestProperty("Cache-Control", "no-cache");
  httpReq.setRequestProperty("Content-Type", "application/octet-stream");
  httpReq.setRequestProperty("Connection", "close\r\n");

  LOG_DEBUG << "Http Message : \n" << httpReq.peek();

  httpReq.connect();
}

int main()
{
  std::thread t(runLoop);

  getchar();

  InetAddress serverAddr("119.29.29.29", 53);
  TcpClient cli(g_loop, serverAddr);
  cli.setConnectionCallBack(newConnetion);
  cli.setMessageCallBack(onMessage);

  cli.start();

  //std::thread t(runLoop);

  //cli.connection()->send("img.zcool.cn");

  getchar();
}

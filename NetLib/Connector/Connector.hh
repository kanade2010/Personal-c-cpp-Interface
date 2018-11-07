#ifndef _NET_CONNECTOR
#define _NET_CONNECTOR

#include <functional>
#include <memory>

#include "InetAddress.hh"
#include "Channel.hh"

class EventLoop;


class Connector
{
public:
  typedef std::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const InetAddress& serverAddr);
  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { m_newConnectionCallBack = cb; }

  void start();// can be called in any thread
  void restart();// must be called in loop thread
  void stop(); // can be called in any thread


  void connect();
  void connecting(int sockfd);

  void handleWrite();
private:

  EventLoop* p_loop;
  InetAddress m_serverAddr;
  std::unique_ptr<Channel> p_channel;
  NewConnectionCallback m_newConnectionCallBack;

};


#endif
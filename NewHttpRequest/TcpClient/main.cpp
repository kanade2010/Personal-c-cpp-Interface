#include <errno.h>
#include <thread>
#include <strings.h>
#include <poll.h>
#include <netdb.h>
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
#include "EventLoopThread.hh"
#include "Connector.hh"
#include "ThreadPool.hh"


class HttpUrl
{
public:
  HttpUrl(const std::string& url)
  :m_httpUrl(url),
   m_smatch(detachHttpUrl())
  {
    LOG_DEBUG << "URL : " << m_httpUrl;
  }
  ~HttpUrl(){};

  enum HttpUrlMatch
  {
    URL = 0,
    HOST = 1,
    URI = 2
  };

  std::vector<std::string> detachHttpUrl() const
  {
    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = m_httpUrl.find('/');
    assert(std::string::npos != pos2);
    pos1 = pos2 + 2;
    pos2 = m_httpUrl.find('/', pos1);
    assert(std::string::npos != pos2);
    v.push_back(m_httpUrl);
    v.push_back(m_httpUrl.substr(pos1, pos2 - pos1));
    v.push_back(m_httpUrl.substr(pos2 + 1));
    LOG_DEBUG << "detachHttpUrl() url :" << v[0];
    LOG_DEBUG << "detachHttpUrl() host :" << v[1];
    LOG_DEBUG << "detachHttpUrl() uri :" << v[2];
    return v;
  }

  std::string toIp()
  {
    struct hostent* phost = NULL;
    char ip[32] = {0};

    phost = gethostbyname(domain().c_str());
    if (NULL == phost)
    {
      LOG_ERROR << "HttpUrlToIp(): gethostbyname error : " << errno << " : "<< strerror(errno);
      return "";
      //LOG_SYSERR << "urlToIp(): gethostbyname error";
    }

    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, sizeof ip);

    m_ip = ip;

    return ip;
  }

  std::string domain() const { return getHttpUrlSubSeg(HOST); }

  std::string ip() const { return m_ip; }

  std::string uri() const { return getHttpUrlSubSeg(URI); }

private:
  std::string getHttpUrlSubSeg(HttpUrlMatch sub = HOST) const{ return m_smatch[sub]; }

  std::string m_httpUrl;
  std::string m_ip;
  std::vector<std::string> m_smatch;
};



EventLoop* g_loop;

void newConnetion(const TcpConnectionPtr& conn)
{
  LOG_DEBUG << "newConnetion() : Connected a new connection.";
  char data[] = {0x61, 0x07, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77, 0x05, 0x62, 0x61, 0x69, 0x64, 0x75, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01};
  conn->send(data, sizeof data);
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, ssize_t len)
{
  LOG_DEBUG << "onMessage() : received " << buf->readableBytes() << " Bytes from connection [" << conn->name();
//  LOG_DEBUG << "onMessage : " << buf->retrieveAsString(len);

  /*
  for(int i =0 ; i < len; i++ ){
    printf("%4d ", *(buf->peek()));
    buf->retrieve(1);
  }*/

  conn->send(buf->peek(), buf->readableBytes());

  buf->retrieve(buf->readableBytes());

}




std::mutex g_urlMutex;
Condition g_urlCond;

void domainToIpTask(HttpUrl* url)
{
  std::string ip;
  int cnt = 0;

  std::unique_lock<std::mutex> lock(g_urlMutex);
  while(ip.empty() && cnt < 10){
    std::this_thread::sleep_for(std::chrono::seconds(4));
    ip = url->toIp();
    cnt++;
    LOG_TRACE << "domainToIpTask() get ip is " << url->ip();
  }

  g_urlCond.notify();

}


int main()
{

  ThreadPool threadPool;
  threadPool.start();

  HttpUrl image("http://img.zcool.cn/community/01ddc256eb71586ac7257d209712b7.jpg@1280w_1l_2o_100sh.jpg");

  threadPool.addTask(std::bind(domainToIpTask, &image));

  std::this_thread::sleep_for(std::chrono::seconds(2));

  {
    std::unique_lock<std::mutex> lock(g_urlMutex);
    while(image.ip().empty())
    {
      g_urlCond.wait(lock);
    }
  }

  if(image.ip().empty())
  {
    LOG_DEBUG << "domainToIpTask() failed exit.";
    return 0;
  }

  LOG_DEBUG << image.domain() << " : " << image.ip();

/*
  EventLoopThread loopThread;

  InetAddress serverAddr(image.ip(), 80);


  TcpClient client(loopThread.startLoop(), serverAddr);


  client.setConnectionCallBack(newConnetion);
  client.setMessageCallBack(onMessage);
  client.start();
*/

  getchar();
}

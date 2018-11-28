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
#include "ThreadPool.hh"

const HttpUrl image("http://img.zcool.cn/community/01ddc256eb71586ac7257d209712b7.jpg@1280w_1l_2o_100sh.jpg");
const HttpUrl xml("https://usglmycar.x431.com/services/publicSoftService");
std::string xmlIp;
std::string imageIp;

const std::string kTestContent = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:x431=\"http://www.x431.com\"><soapenv:Header/><soapenv:Body><x431:getPublicSoftMaxVersionByName><serialNo>978290000089</serialNo><publicSoftName>Gtbox_Mini_main_app_DB</publicSoftName><versionNo>1.00.002</versionNo><displayLan>CN</displayLan></x431:getPublicSoftMaxVersionByName></soapenv:Body></soapenv:Envelope>";


class HttpProtocol
{
public:
  enum MethodE{GET, POST};

  HttpProtocol();
  ~HttpProtocol();

  void setRequestMethod(const std::string &method, const HttpUrl& url);
  void setRequestProperty(const std::string &key, const std::string &value);
  void setRequestBody(const std::string &content);

  Buffer* buffer() { return &m_buffer; }

private:
  static std::map<std::string, int> kRequestMethods;

  Buffer m_buffer;
};

const std::map<std::string, int>::value_type init_value[] =
{
  std::map<std::string, int>::value_type( "GET", HttpProtocol::GET),

  std::map<std::string, int>::value_type( "POST", HttpProtocol::POST)
};

std::map<std::string, int> HttpProtocol::kRequestMethods(init_value, init_value + (sizeof init_value / sizeof init_value[0]));

 HttpProtocol::HttpProtocol()
 {

 }

HttpProtocol::~HttpProtocol()
{

}

void HttpProtocol::setRequestMethod(const std::string &method, const HttpUrl& url)
{
  switch(HttpProtocol::kRequestMethods.at(method))
  {
    case HttpProtocol::GET :
      m_buffer << "GET " << "/" << url.uri() << " HTTP/1.1\r\n";
      break;
    case HttpProtocol::POST :
      m_buffer << "POST "  << "/" << url.uri() << " HTTP/1.1\r\n";
      break;
    default :
      LOG_ERROR << "No such Method : " << method.c_str();
      break;
  }

  m_buffer << "Host: " << url.domain() << "\r\n";
}


void HttpProtocol::setRequestProperty(const std::string &key, const std::string &value)
{
  m_buffer << key << ": " << value << "\r\n";
}

void HttpProtocol::setRequestBody(const std::string &content)
{
  m_buffer << content;
}


EventLoop* g_loop;

void newConnetion(const TcpConnectionPtr& conn)
{
  HttpProtocol httpStream;

  httpStream.setRequestMethod("POST", xml);
  httpStream.setRequestProperty("Content-type", "text/xml;charset=UTF-8");
  httpStream.setRequestProperty("Cache-Control", "no-cache");
  httpStream.setRequestProperty("Connection", "close");
  httpStream.setRequestProperty("Content-Length", std::to_string(kTestContent.size()) + "\r\n");
  httpStream.setRequestBody(kTestContent);

  conn->send(httpStream.buffer());

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
  imageIp = image.toIp();
  LOG_DEBUG << "toIp : " << imageIp;

  InetAddress serverAddr(imageIp, 80);

  HttpRequest httpReq(g_loop, serverAddr);
  httpReq.setRequestMethod("GET", image);
  httpReq.setRequestProperty("Cache-Control", "no-cache");
  httpReq.setRequestProperty("Content-Type", "application/octet-stream");
  httpReq.setRequestProperty("Connection", "close\r\n");

  LOG_DEBUG << "Http Message : \n" << httpReq.peek();

  httpReq.connect();
}

int main()
{



  //std::thread t(runLoop);

  //cli.connection()->send("img.zcool.cn");

  getchar();
}

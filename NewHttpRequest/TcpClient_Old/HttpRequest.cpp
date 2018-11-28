#include <string>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/uio.h>  // readv
#include <stdint.h>
#include <endian.h>
#include <unistd.h>
#include <map>
#include <fstream>
#include <functional>

#include "TcpConnection.hh"
#include "TcpClient.hh"
#include "HttpRequest.hh"

const std::map<std::string, int>::value_type init_value[] =
{
	std::map<std::string, int>::value_type( "GET", HttpRequest::GET),

	std::map<std::string, int>::value_type( "POST", HttpRequest::POST)
};

const static std::map<std::string, int> kRequestMethodMap(init_value, init_value + (sizeof init_value / sizeof init_value[0]));

static inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htobe16(host16);
}

HttpRequest::HttpRequest(EventLoop* loop, const InetAddress& servAddr)
  :p_loop(loop),
  p_tcpClient(new TcpClient(loop, servAddr))
{
  p_tcpClient->setConnectionCallBack(std::bind(&HttpRequest::newConnetion, this, std::placeholders::_1));
  p_tcpClient->setMessageCallBack(std::bind(&HttpRequest::handleRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::newConnetion(const TcpConnectionPtr& conn)
{
  LOG_DEBUG << "Connection Conn";
  send();
}

void HttpRequest::handleRead(const TcpConnectionPtr& conn, Buffer* buffer, ssize_t len)
{
  LOG_TRACE << "HttpRequest::handleRead()";
  ssize_t writtenBytes = 0;

  while(!m_haveHandleHead)
  {
    std::string line = buffer->retrieveOneLine();
    if(line.size() == 0)
    {
      m_haveHandleHead = true;
      break;
    }

    std::vector<std::string> v;
    SplitString(line, v, ":");
    if(v.size() == 3) { m_code = std::stoi(v[1]); }
    if(v.size() == 2){ m_ackProperty[v[0]] = v[1].erase(0,v[1].find_first_not_of(" ")); }

    LOG_TRACE << "retrieveOneLine() " << line.size() << " Bytes " << line;
  }

  LOG_TRACE << "HttpRequest::handleRead() Content-Type : " << m_ackProperty["Content-Type"];
  if(m_ackProperty["Content-Type"] == std::string("image/jpeg")){
    p_tcpClient->connection()->setMessageCallBack(std::bind(&HttpRequest::downloadFile, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  }
}


void HttpRequest::setRequestMethod(const std::string &method, const HttpUrl& url)
{
	switch(kRequestMethodMap.at(method))
	{
		case HttpRequest::GET :
			m_buffer << "GET " << "/" << url.uri() << " HTTP/1.1\r\n";
			break;
		case HttpRequest::POST :
			m_buffer << "POST "  << "/" << url.uri() << " HTTP/1.1\r\n";
			break;
		default :
			LOG_ERROR << "No such Method : " << method.c_str();
			break;
	}

	m_buffer << "Host: " << url.domain() << "\r\n";
}


void HttpRequest::setRequestProperty(const std::string &key, const std::string &value)
{
	m_buffer << key << ": " << value << "\r\n";
}

void HttpRequest::setRequestBody(const std::string &content)
{
	m_buffer << content;
}

void HttpRequest::send()
{
  p_tcpClient->connection()->send(&m_buffer);
}

void HttpRequest::downloadFile(const TcpConnectionPtr& conn, Buffer* buffer, ssize_t len)
{
  LOG_TRACE << "HttpRequest::downloadFile()";

  assert(m_haveHandleHead);

  std::ofstream output("./test.jpeg", std::ofstream::binary | std::ofstream::app);
  if (!output.is_open()){ // 检查文件是否成功打开
    LOG_SYSFATAL << "open file error";
  }

  output.write(buffer->peek(), buffer->readableBytes());
  LOG_TRACE << "HttpRequest::downloadFile() hasWritten " << buffer->readableBytes() << " Bytes.";
  buffer->retrieve(buffer->readableBytes());

  output.close();
}

/*

void HttpRequest::uploadFile(const std::string& file, const std::string& contentEnd)
{

	FILE* fp = fopen(file.c_str(), "rb");
	if(fp == NULL)
	{
		LOG_SYSFATAL << "fopen() File :" << file.c_str() << " Errno";
	}

	bool isEnd = false;
	ssize_t writtenBytes = 0;

	assert(m_buffer.writableBytes() == Buffer::kInitialSize);

	while(!isEnd)
	{
		ssize_t nread = fread(m_buffer.beginWrite(), 1, kBufferSize, fp);
		m_buffer.hasWritten(nread);
		while(m_buffer.writableBytes() > 0)
		{
			LOG_TRACE << "file read(): nread: " << nread << " remain: " << m_buffer.writableBytes();
			size_t n = fread(m_buffer.beginWrite(), 1, m_buffer.writableBytes(), fp);
			m_buffer.hasWritten(n);
			if(0 == n)
			{	int err = ferror(fp);
				if(err)
				{
					fprintf(stderr, "fread failed : %s\n", strerror(err));
				}
				LOG_DEBUG << "sockets::read finish";
				isEnd = true;
				break;
			}
		}

		ssize_t nwrite = sockets::write(m_sockfd, m_buffer.peek(), m_buffer.readableBytes());
		if(nwrite < 0) LOG_SYSFATAL << "sockets::write";
		writtenBytes += nwrite;
		LOG_TRACE << "sockets::write nread " << m_buffer.readableBytes() << " nwrite " << nwrite << " writtenBytes " << writtenBytes;
		m_buffer.retrieve(nwrite);
	}

	fclose(fp);

	m_buffer.retrieveAll();

	ssize_t n = sockets::write(m_sockfd, contentEnd.c_str(), contentEnd.size());
	if(n < 0) LOG_SYSFATAL << "sockets::write";
}
*/

void HttpRequest::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

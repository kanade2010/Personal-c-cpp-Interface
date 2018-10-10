#include "HttpClient.hh"
#include "InetAddress.hh"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "SocketHelp.hh"

const ssize_t kBufferSize = Buffer::kInitialSize;

std::string gServerMcuVersion;

HttpClient::HttpClient(std::string httpUrl)
	:m_httpUrl(httpUrl),
//	m_tcpClient(m_httpUrl.toInetAddr()),
	m_tcpClient(m_httpUrl.domain()),
	m_stream(),
	m_code(0),
	m_haveHandleHead(false)
{

}

HttpClient::~HttpClient()
{

}

const std::map<std::string, int>::value_type init_value[] =
{
	std::map<std::string, int>::value_type( "GET", HttpClient::GET),

	std::map<std::string, int>::value_type( "POST", HttpClient::POST)
};

const static std::map<std::string, int> kRequestMethodMap(init_value, init_value + (sizeof init_value / sizeof init_value[0]));

void HttpClient::setRequestMethod(const std::string &method)
{
	switch(kRequestMethodMap.at(method))
	{
		case HttpClient::GET :
			m_stream << "GET " << "/" << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.1\r\n";
			LOG_DEBUG << m_stream.str().c_str();
			break;
		case HttpClient::POST :
			m_stream << "POST "  << "/" << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.1\r\n";
			LOG_DEBUG << m_stream.str().c_str();
			break;
		default :
			LOG_ERROR << "No such Method : " << method.c_str();
			break;
	}

	m_stream << "Host: " << m_httpUrl.getHttpUrlSubSeg(HttpUrl::HOST) << "\r\n";
}

void HttpClient::setRequestProperty(const std::string &key, const std::string &value)
{
	m_stream << key << ": " << value << "\r\n";
}

void HttpClient::setRequestBody(const std::string &content)
{
	m_stream << content;
}

void HttpClient::handleRead()
{
	assert(!m_haveHandleHead);
	ssize_t nread = 0;
	ssize_t writtenBytes = 0;

	nread = sockets::read(g_sockfd, m_buffer.beginWrite(), kBufferSize);
	if(nread < 0) LOG_SYSFATAL << "sockets::read";
	m_buffer.hasWritten(nread);
	LOG_TRACE << "sockets::read(): nread: " << nread << " remain: " << m_buffer.writableBytes();
	size_t remain = kBufferSize - nread;
	while(remain > 0)
	{
		size_t n = sockets::read(g_sockfd, m_buffer.beginWrite(), remain);
		if(n < 0) LOG_SYSFATAL << "sockets::read";
		m_buffer.hasWritten(n);
		if(0 == n)
		{
			LOG_DEBUG << "sockets::read finish";
			break;
		}
		remain = remain - n;
	}
	//std::cout << m_buffer.peek();

	//for(int i = 0; i < nread; i++) printf("%02x%c",(unsigned char)buffer[i],i==nread - 1 ?'\n':' ');
	//LOG_DEBUG << "handleRead Recv Response : \n" << m_buffer.peek();
	int headsize = 0;
	std::string line;
	std::stringstream ss(m_buffer.peek());
	std::vector<std::string> v;
	getline(ss, line);
	//LOG_DEBUG << line;
	headsize += line.size() + 1;
	SplitString(line, v, " ");
	//for(int i = 0; i < v.size(); i++) std::cout << v[i] << std::endl;
	m_code = std::stoi(v[1]);
	if(v[1] != "200")
	{
	  LOG_ERROR << "Error Http Server Response : " << v[1].c_str();
	}

	do{
		getline(ss, line);
		headsize += line.size() + 1;
		if(!line.empty()) line.erase(line.end()-1); // remove '/r'
		//LOG_DEBUG << line;
		v.clear();
		SplitString(line, v, ": ");
		if(v[0] == "sign")
		{
			LOG_DEBUG << "sign : " << v[1];
			std::string mcuVersion(v[1], 32);
			LOG_DEBUG << "Server Mcu Version : " << mcuVersion;
			gServerMcuVersion = mcuVersion;
		}
	}while(!line.empty());

	LOG_DEBUG << "Http Head Size is " << headsize;
	std::string res(m_buffer.peek(), headsize);
	LOG_DEBUG << "Http Response :\n" << res;
	m_buffer.retrieve(headsize);

	m_haveHandleHead = true;

} 

void HttpClient::uploadFile(const std::string& file, const std::string& contentEnd)
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

		ssize_t nwrite = sockets::write(g_sockfd, m_buffer.peek(), m_buffer.readableBytes());
		if(nwrite < 0) LOG_SYSFATAL << "sockets::write";
		writtenBytes += nwrite;
		LOG_TRACE << "sockets::write nread " << m_buffer.readableBytes() << " nwrite " << nwrite << " writtenBytes " << writtenBytes;
		m_buffer.retrieve(nwrite);
	}

	fclose(fp);

	m_buffer.retrieveAll();

	ssize_t n = sockets::write(g_sockfd, contentEnd.c_str(), contentEnd.size());
	if(n < 0) LOG_SYSFATAL << "sockets::write";
}

void HttpClient::downloadFile(const std::string& file)
{
	assert(m_haveHandleHead);

	bool isEnd = false;
	ssize_t nread = 0;
	ssize_t writtenBytes = 0;
	bool haveHandleHead = false;
	bool isDownFile = false;

	std::ofstream output(file, std::ios::binary);
	if (!output.is_open()){ // 检查文件是否成功打开  
		LOG_SYSFATAL << "open file error" << file;
	}

	output.write(m_buffer.peek(), m_buffer.readableBytes());
	writtenBytes += m_buffer.readableBytes();
	m_buffer.retrieve(m_buffer.readableBytes());

	while(!isEnd)
	{
		nread = sockets::read(g_sockfd, m_buffer.beginWrite(), kBufferSize);
		if(nread < 0) LOG_SYSFATAL << "sockets::read";
		m_buffer.hasWritten(nread);
		LOG_TRACE << "sockets::read(): nread: " << nread << " remain: " << m_buffer.writableBytes();
		size_t remain = kBufferSize - nread;
		while(remain > 0)
		{
			size_t n = sockets::read(g_sockfd, m_buffer.beginWrite(), remain);
			if(n < 0) LOG_SYSFATAL << "sockets::read";
			m_buffer.hasWritten(n);
			if(0 == n)
			{
				LOG_DEBUG << "sockets::read finish";
				isEnd = true;
				break;
			}
			remain = remain - n;
		}

		//LOG_TRACE << "Before Write Buffer readableBytes() " << m_buffer.readableBytes() << " | Buffer writableBytes() " << m_buffer.writableBytes();
		output.write(m_buffer.peek(), m_buffer.readableBytes());
		writtenBytes += m_buffer.readableBytes();
		m_buffer.retrieve(m_buffer.readableBytes());
		//LOG_TRACE << "After Write Buffer readableBytes() " << m_buffer.readableBytes() << " | Buffer writableBytes() " << m_buffer.writableBytes();
		//LOG_TRACE << " writtenBytes " << writtenBytes;

		/*std::string msg(m_buffer.peek(), m_buffer.readableBytes());
		m_buffer.retrieve(m_buffer.readableBytes());
		LOG_DEBUG << "Http message :\n" << msg << '\n';*/
		//std::cout << (m_buffer.peek()) << std::endl;
		//for(int i = 0; i < m_buffer.readableBytes(); i++) printf("%02x%c",*reinterpret_cast<const unsigned char*>(m_buffer.peek() + i),i==nread - 1 ?'\n':' ');
	}
	LOG_DEBUG << " writtenBytes " << writtenBytes;

	output.close();
	sockets::close(g_sockfd);
}

void HttpClient::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
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
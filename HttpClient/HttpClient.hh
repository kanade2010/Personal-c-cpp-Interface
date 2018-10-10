#ifndef _HTTPCLIENT_HH
#define _HTTPCLIENT_HH

#include "TcpClient.hh"
#include "HttpUrl.hh"
#include "Buffer.hh"

class HttpClient
{
public:
	enum HttpRequestMethod{
		GET = 0,
		POST
	};
	HttpClient(std::string httpUrl);
	~HttpClient();

	void connect() {m_tcpClient.connect();}
	//void TEST(const std::string path,const std::string content);
	void setRequestMethod(const std::string &method);
	void setRequestProperty(const std::string &key, const std::string &value);
	void setRequestBody(const std::string &content);

	//void clear() { clearStream(); m_buffer.retrieveAll(); } 
	void clearStream() {m_stream.str("");}
	std::string strStream() const { return m_stream.str(); };

	int getResponseCode() const { 
		assert(m_haveHandleHead);
		return m_code; 
	}

	std::string getResponseContent() { 
		assert(m_haveHandleHead);
		return std::string(m_buffer.peek(), m_buffer.readableBytes()); 
	}

	void handleRead();
	void uploadFile(const std::string& file, const std::string& contentEnd);
	void downloadFile(const std::string& file);

	void send(){
		sockets::write(g_sockfd, strStream().c_str(), strStream().size());
	}

	void close(){ sockets::close(g_sockfd); }

private:
    void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

    Buffer m_buffer;
	HttpUrl m_httpUrl;
	TcpClient m_tcpClient;
	std::stringstream m_stream;
	int m_code;
	bool m_haveHandleHead;
};

#endif
#include "InetAddress.hh"
#include "TcpClient.hh"
#include "HttpClient.hh"
#include "Logger.hh"

int main()
{
	InetAddress serverAddr("212.100.220.116", 80);
	LOG_INFO << serverAddr.family();
	LOG_INFO << serverAddr.ipNetEndian();

	/*TcpClient client(serverAddr);
	client.connect();
	*/

	HttpClient httpClient("aep.machinestalk.com");
	httpClient.GET();

	getchar();
	return 0;
}







/*

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <regex>
#include <string>
#include "Edian.hh"


void POST()
{
	std::stringstream stream;
	stream << "POST " << "/api/devicelog/nzlSbg9sRQ/976890001624/upload";
	stream << " HTTP/1.0\r\n";
	stream << "Host: "<< "aep.machinestalk.com" << "\r\n";
	stream << "Cache-Control: no-cache\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n\r\n";

	stream << "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
	stream << "Content-Disposition: form-data; name=\"\"; filename=\"log.tar.gz\"\r\n";
	stream << "Content-Type: application/gzip\r\n\r\n\r\n";

	stream << "Content-Typ appliaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacation/gzip\r\n";

	stream << "------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n";
	std::cout << stream.str();
}

void fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    std::cout << "sockets::fromIpPort";
  }
}

int hostnameToIp(const char* hostname, char* ip)
{
    struct hostent* phost = NULL;
 
    phost = gethostbyname(hostname);
    if (NULL == phost)
    {
        return -1;    
    }
        
    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, 16);

    return 0;
}


const std::string kLogUploadAddr = "http://aep.machinestalk.com/api/devicelog/nzlSbg9sRQ/976890001624/upload";

std::string getHttpUrlDomain(const std::string url)
{
    std::regex httpExpression("((http|https)?://)?([0-9\\.a-z]*)(/.*)");
    std::smatch result;
    
    if(regex_match(url, result, httpExpression))
    {
    	return result[3].str();
    }
    else
    {
    	return "";
    }

}


#define READ_SIZE 4096




int main ()
{
    std::string domain = getHttpUrlDomain(kLogUploadAddr);
    if(!domain.empty())
    	std::cout << "get http url domain is : "<< domain << std::endl;
	
	int ret = -1;
	char recvbuf[4096];

	char ip[32] = {0};
	ret = hostnameToIp(domain.c_str(), ip);
	if(-1 == ret){
		printf("hostnameToIp failed\n");
		return -1;
	}

    struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof serv_addr);
	fromIpPort(ip, 80, &serv_addr);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
	{
		printf("get socket fd failed\n");
		return -1;
	}

	ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
	if(-1 == ret)
	{
		printf("connect to Server failed\n");
		return -1;
	}

	printf("connected successful!\n");


	std::stringstream stream;
	stream << "POST " << "/api/devicelog/nzlSbg9sRQ/976890001624/upload/" << " HTTP/1.0\r\n";
	stream << "Host: "<< "aep.machinestalk.com" << "\r\n";
	stream << "Cache-Control: no-cache\r\n";
	//stream << "Transfer-Encoding: chunked\r\n";
	stream << "Content-Length: 1306039\r\n";
	//stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Connection: close\r\n";
	stream << "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n\r\n";

	stream << "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
	stream << "Content-Disposition: form-data; name=\"upload\"; filename=\"test.tar.gz\"\r\n";
	//stream << "Content-Disposition: form-data; name=\"Upload\"\r\n";
	stream << "Content-Type: application/gzip\r\n\r\n";

	std::string end = "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n";


	

	send(sock, stream.str().c_str(), stream.str().size(), 0);

	std::cout<< stream.str() << std::endl;


	FILE* fp = fopen("test.tar.gz", "rb");
		if(fp == NULL) 
		{
			printf("fopen failed\n");
			exit(0);
		}


	int i = 0;
	unsigned char buffer[READ_SIZE] = {0};
	size_t nread = 0;
	size_t writtenBytes = 0;
	bool isEnd = false;

	while(!isEnd)
	{
		nread = fread(buffer, 1, READ_SIZE, fp);
		size_t remain = READ_SIZE - nread;
		while(remain > 0)
		{
			printf("wb+++ nread %d remain %d\n",nread, remain);
			size_t n = fread(buffer + nread, 1, remain, fp);
			if(0 == n)
			{	int err = ferror(fp);
				if(err)
				{
					fprintf(stderr, "fread failed : %s\n", strerror(err));
				}
				printf("wb+++ break\n");
				isEnd = true;
				break;
			}
			remain = remain - n;
			nread += n;
		}

		int nwrite = 0;
		nwrite = send(sock, buffer, nread, 0);



		writtenBytes += nread;
		printf("\n nread %d, nwrite %d writtenBytes %d\n", nread, nwrite, writtenBytes);
		
		//sleep(1);
	}

	
	//ret = system("rm /usrdata/log.tar.gz");

	//printf("ret is %d , rm end ~\n", ret);

	fclose(fp);

	//send(sock, "\r\nContent-Disposition: form-data; name=\"Upload\"\r\n", sizeof("\r\nContent-Disposition: form-data; name=\"Upload\"\r\n") ,0);
	//std::cout<< "\r\nContent-Disposition: form-data; name=\"Upload\"\r\n" << std::endl;


	send(sock, end.c_str(), end.size(), 0);

	std::cout<< end << std::endl;

	while((ret = recv(sock, recvbuf, 4096, 0)) > 0)
		std::cout << ret << "	:" << recvbuf << std::endl;
	
	getchar();


    return 0;
}



/*


int main(void)
{
    int i = 0;
    char str[32] = {0};
     struct hostent* phost = NULL;
 
    phost = gethostbyname("aep.machinestalk.com");
    if (NULL == phost)
    {
        return -1;    
    }
    
    printf("---Offical name:\n\t%s\n", phost->h_name);

    printf("---Alias name:\n");    
    for (i = 0;  phost->h_aliases[i]; i++)
    {
        printf("\t%s\n", phost->h_aliases[i]);
    }

    printf("---Address list:\n");
    for (i = 0; phost->h_addr_list[i]; i++)
    {
        printf("\t%s\n", inet_ntop(phost->h_addrtype,  phost->h_addr_list[i], str, sizeof(str)-1));
    }


    std::cout << getDomain("http://aep.machinestalk.com/api/devicelog/nzlSbg9sRQ/976890001624/upload") << std::endl;

    return 0;
}*/
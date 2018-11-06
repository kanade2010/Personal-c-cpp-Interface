#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define DEBUG_LOGE
#define DEBUG_LOGW
#define DEBUG_LOGI
#define DEBUG_LOGD

#ifdef DEBUG_LOGE
    #define LOGE(fmt, ...) \
            printf("\033[31mLOG_ERR>>> %s(%d) %s(): " fmt "\033[0m\n", __FILE__, __LINE__, __func__,## __VA_ARGS__)
#else
    #define LOGE(fmt, ...)
#endif

#ifdef DEBUG_LOGW
    #define LOGW(fmt, ...) \
            printf("\033[35mLOG_WARNING>>> %s(%d) %s(): " fmt "\033[0m\n", __FILE__, __LINE__, __func__,## __VA_ARGS__)
#else
    #define LOGW(fmt, ...)
#endif

#ifdef DEBUG_LOGI
    #define LOGI(fmt, ...) \
            printf("\033[32mLOG_INFO>>> %s(%d) %s(): " fmt "\033[0m\n", __FILE__, __LINE__, __func__,## __VA_ARGS__)
#else
    #define LOGE(fmt, ...)
#endif

#ifdef DEBUG_LOGD
    #define LOGD(fmt, ...) \
            printf("\033[33mLOG_DEBUG>>> %s(%d) %s(): " fmt "\033[0m\n", __FILE__, __LINE__, __func__,## __VA_ARGS__)
#else
    #define LOGD(fmt, ...)
#endif


static int g_socket_fd = -1;

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

#define SUCCESS 0
#define FAILURE -1

int main()
{
	int ret;
	struct sockaddr_in serv_addr;
	unsigned char buf[1024] = "GET /getPin?sn=978290000069 HTTP/1.1\r\n\r\n";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(SERVER_PORT);
	
	ret = inet_pton(AF_INET, SERVER_ADDR, (void *)&serv_addr.sin_addr);
	if(FAILURE == ret)
	{
		LOGE("inet_pton convert failed\n");
		return FAILURE;
	}

	g_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(FAILURE == g_socket_fd)
	{
		LOGE("get socket fd failed\n");
		return FAILURE;
	}

	ret = connect(g_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
	if(FAILURE == ret)
	{
		LOGE("connect to Server failed\n");
		return FAILURE;
	}

	printf("connected successful!\n");



	while(1)
	{
		send(g_socket_fd, buf, sizeof(buf), 0);
	ret = recv(g_socket_fd, buf, 1024, 0);
	int i = 0;
	printf("recv ");
	for(i=0; i < ret ; i++)
	printf("%c", buf[i]);
	printf("\n\n");
	sleep(1);
	}


getchar();
	return 0;
}





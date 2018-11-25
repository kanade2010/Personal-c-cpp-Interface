#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include<time.h>

#define MYPORT 53
#define SHOW_DATA
char* SERVERIP = "114.114.114.114";

#define ERR_EXIT(m)\
do \
{\
    perror(m); \
    exit(EXIT_FAILURE); \
} while(0)

void ask_ip(int sock,char * name,char show)
{
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVERIP);

    int ret;
    int i,j,k;
    char * s = name;
    unsigned char sendbuf[1024] = {0};
    unsigned char host[50] = {0};
    unsigned char nd1[10] = {1,0,0,1,0,0,0,0,0,0};  //数据标志，期望递归RD置位，一个问题
    unsigned char nd2[5] = {0,0,1,0,1};                  //主机名结尾0，查询类型1，查询类1
    unsigned char recvbuf[1024] = {0};

    //产生数据包标识
    sendbuf[0] = rand() % 256;
    sendbuf[1] = rand() % 256;
    //主机名处理
    i = k = 0;
    j = 1;
    while(*name != 0)
    {
        if(*name == '.')
        {
            host[i] = k;
            k = 0;
            i = j;
            j++;
            name++;
        }else
        {
            host[j] = *name;
            j++;
            k++;
            name++;
        }
    }
    host[i] = k;
    k = j ;
    j = 0;
    i = 2;
    for(j=0;j<10;i++,j++)
    {
        sendbuf[i] = nd1[j];
    }
    for(j=0;j<k;i++,j++)
    {
        sendbuf[i] = host[j];
    }
    for(j=0;j<5;i++,j++)
    {
        sendbuf[i] = nd2[j];
    }
    if(show)
    {
        printf("发送数据包:\n");
        for(j=0;j<i;j++)
        {
            printf("%4d",sendbuf[j]);
        }
        printf("\n长度:%d\n",i);
    }
    sendto(sock, sendbuf, i, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //write(sock, sendbuf, i);
    ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
    if (ret == -1)
    {
        if (errno == EINTR)
        {
            printf("失败\n");
            close(sock);
            return;
        }
        ERR_EXIT("recvfrom");
    }
    //recvbuf[7]保存资源记录数，即IP个数
    j = recvbuf[7];
    k = j * 16 + i;
    printf("主机  %s  对应的%2d个IP地址如下:\n",s,recvbuf[7]);
    i += 12;
    while(j--)
    {
        printf("%3d.%3d.%3d.%3d\n",recvbuf[i],recvbuf[i+1],recvbuf[i+2],recvbuf[i+3]);
        i += 16;
    }
    if(show)
    {
        printf("接收数据包:\n");
        for(j=0;j<k;j++)
        {
            printf("%4d",recvbuf[j]);
        }
        printf("\n长度:%d\n",k);
    }
    close(sock);
}

int main(int argc,char* argv[])
{
    int sock;
    char *inputbuf;
    char show;
    //随机函数种子
    srand((int)time(0));
    if(argc != 2 && argc != 3)
    {
        printf("请使用 \"./udp_dns.cpp -s (hostname)\" 或者 \"./udp_dns.cpp -u (hostname)\" 方式运行，主机名可写可不写\n");
        return 0;
    }else{
        if(strcmp("-s",argv[1]) == 0){
            show = 1;
        }else if(strcmp("-u",argv[1]) == 0){
            show = 0;
        }else
        {
            printf("请使用 \"./udp_dns.cpp -s \" 或者 \"./udp_dns.cpp -u \" 方式运行\n");
            return 0;
        }
        if(argc == 3){
            inputbuf = argv[2];
        }else{
            inputbuf = (char *)malloc(40 * sizeof(char));
            printf("请输入要解析的主机名(eg:baidu.com):\n");
            scanf("%s",inputbuf);
        }
    }

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        ERR_EXIT("socket");
    }
    ask_ip(sock,inputbuf,show);
    return 0;
}

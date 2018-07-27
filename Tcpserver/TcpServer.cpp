#include "TcpServer.hh"
#include "debug.h"

#define TAG "TcpServer"
#define LISTEN_BACKLOG_SIZE 2

TcpServer::TcpServer(PortNumBits port):fSocket(Socket(port)){
	fClientSocket1 = -1;
	fClientSocket2 = -1;
}

TcpServer::~TcpServer(){
}

void TcpServer::Listen(){
	if(listen(fSocket.socketNum(), LISTEN_BACKLOG_SIZE) < 0){
		printf("(%d)\n", fSocket.socketNum());
		LOG_E("Lisen recv Socket failed %d : %s\n", errno, strerror(errno));
		exit(-1);
	}
}

int TcpServer::Accept(int sock){
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	LOG_D("accept sendSocket\n");
	sock = accept(fSocket.socketNum(), (struct sockaddr *)&clientAddr, &clientAddrLen);
	if(sock < 0){
		LOG_E("sendSocket accept failed error %d: %s\n", errno, strerror(errno));
		return -1;
	}
	return sock;
}

Boolean TcpServer::getConnectStatus(){
	return (fClientSocket1 != -1) && (fClientSocket2 != -1);
} 

void TcpServer::doEventLoop(int timeout){
	int maxfd = fSocket.socketNum();
	unsigned char buffer[1024];
	Listen();

    while(true)
    {
    	fd_set readfd;    
	    FD_ZERO(&readfd);
	    if(fClientSocket1 == -1 | fClientSocket2 == -1)
	    FD_SET(fSocket.socketNum(), &readfd);

	    struct timeval vtime;
	    vtime.tv_sec = timeout / 1000;
	    vtime.tv_usec = (timeout % 1000) * 1000;
	    if(fClientSocket1!=-1) FD_SET(fClientSocket1, &readfd);
	    if(fClientSocket2!=-1) FD_SET(fClientSocket2, &readfd);

    	//LOG_D("wb+++7 maxfd %d \n", maxfd);
    	int ret = select(maxfd + 1, &readfd, NULL, NULL, &vtime);
	    if(0 == ret)
	    {
	        LOG_D("select1 timeout\n");
	        continue;
	    }
	    else if(ret < 0)
	    {
	        LOG_E("select1 error %d : %s\n",errno, strerror(errno));
	        exit(-1);
	    }
	    else
	    {
	    		if(FD_ISSET(fSocket.socketNum(), &readfd)){
	    			if(fClientSocket1 == -1 | fClientSocket2 == -1){
	    				int sock = -1;	    				
	    				ret = Accept(sock);
			    		if(fClientSocket1 == -1){
			    			fClientSocket1 = ret;
			    			if(fClientSocket1 > maxfd) maxfd = fClientSocket1;
			    			FD_SET(fClientSocket1, &readfd);
			    			LOG_D("wb+++4\n");
			    			continue;
			    		}
			    		if(fClientSocket2 == -1){
			    			fClientSocket2 = ret;
			    			if(fClientSocket2 > maxfd) maxfd = fClientSocket2;
			    			FD_SET(fClientSocket2, &readfd);
			    			LOG_D("wb+++5\n");
			    			continue;
			    		}
		    		}
	    		}
	    		else if(FD_ISSET(fClientSocket1, &readfd)){
		    		int nread = recv(fClientSocket1,buffer, 1024, 0);
		    		LOG_D("wb+++11 recv is %d\n",nread);		    		
		    		if(nread <= 0){
		    			FD_CLR(fClientSocket1, &readfd);
		    			fClientSocket1 = -1;
		    			LOG_D("wb+++6 %d %s\n", errno, strerror(errno));
		    			continue;
		    		}
		    		for(int i = 0; i < nread; i++) printf("0x%02x ", buffer[i]);
   	 				printf("\n");
		    		send(fClientSocket2,buffer, nread, 0);	
	    		}
	    		else if(FD_ISSET(fClientSocket2, &readfd)){
		    		int nread = recv(fClientSocket2,buffer, 1024, 0);
		    		LOG_D("wb+++10 recv is %d\n",nread);
		    		if(nread <= 0){
		    			fClientSocket2 = -1;
		    			FD_CLR(fClientSocket2, &readfd);
		    			LOG_D("wb+++7 %d %s\n", errno, strerror(errno));
		    			continue;
		    		}
		    		for(int i = 0; i < nread; i++) printf("0x%02x ", buffer[i]);
   	 				printf("\n");
		    		send(fClientSocket1,buffer, nread, 0);
	    	}
	    }
    }
}
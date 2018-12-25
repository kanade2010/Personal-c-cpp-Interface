#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <vector>

#define MAX_BUFFER_SIZE 1024
#define IN_FILES 1
#define MAX(a,b) ((a>b)?(a):(b))

typedef std::vector<struct epoll_event> PollFdList;

int main(int argc ,char **argv)
{

  int fd;
  char buf[1024];
  int i,res,real_read, maxfd;

  if((fd=open("/dev/stdin",O_RDONLY|O_NONBLOCK)) < 0)
  {
    fprintf(stderr,"open data1 error:%s",strerror(errno));
    return 1;
  }

  PollFdList m_pollfds;
  int epfd = epoll_create(111);

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLPRI;
  ev.data.fd = fd;

  epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
  m_pollfds.push_back(ev);

  while(1)
  {
    int ret = epoll_wait(epfd, m_pollfds.data(), 1000, 1000);
    if (ret < 0)
    {
      printf("Poll error : %s\n",strerror(errno));
      return 1;
    }

    if(ret == 0){
      printf("Poll timeout\n");
      continue;
    }

    for (i = 0; i< 1; i++)
    {
      if (m_pollfds[i].events & EPOLLIN)
      {
        memset(buf, 0, MAX_BUFFER_SIZE);
        real_read = read(m_pollfds[i].data.fd, buf, MAX_BUFFER_SIZE);
        if (real_read < 0)
        {
          if (errno != EAGAIN)
          {
            printf("read eror : %s\n",strerror(errno));
            continue;
          }
        }
        else if (!real_read)
        {
          close(m_pollfds[i].data.fd);
          m_pollfds[i].events = 0;
        }
        else
        {
          if (i == 0)
          {
            buf[real_read] = '\0';
            printf("%s", buf);
            if ((buf[0] == 'q') || (buf[0] == 'Q'))
            {
              printf("quit\n");
              return 1;
            }
          }
          else
          {
            buf[real_read] = '\0';
            printf("%s", buf);
          }
        }
      }
    }
  }

  exit(0);
}
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 1024
#define IN_FILES 1
#define MAX(a,b) ((a>b)?(a):(b))

int main(int argc ,char **argv)
{
  struct pollfd fds[3];
  char buf[1024];
  int i,res,real_read, maxfd;

  if((fds[0].fd=open("/dev/stdin",O_RDONLY|O_NONBLOCK)) < 0)
  {
    fprintf(stderr,"open data1 error:%s",strerror(errno));
    return 1;
  }

  for (i = 0; i < IN_FILES; i++)
  {
    fds[i].events = POLLIN | POLLPRI;
  }

  while(1) //|| fds[1].events || fds[2].events)
  {
    int ret = poll(fds, 1, 1000);
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
      if (fds[i].revents)
      {
        memset(buf, 0, MAX_BUFFER_SIZE);
        real_read = read(fds[i].fd, buf, MAX_BUFFER_SIZE);
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
          close(fds[i].fd);
          fds[i].events = 0;
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
#include <iostream>

#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <strings.h>

//Reactor Test
//单次触发定时器
#include <sys/timerfd.h>
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK |TFD_CLOEXEC);

  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 3;
  timerfd_settime(timerfd, 0, &howlong, NULL);

  fd_set rdset;
  FD_ZERO(&rdset);
  FD_SET(timerfd, &rdset);

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  while(1)
  {
    if(select(timerfd + 1, &rdset, NULL, NULL, &timeout) == 0)
    {
      std::cout << "timeout\n";
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      FD_SET(timerfd, &rdset);
        continue;
    }
    std::cout << " timer happend\n";
    break;
  }


  close(timerfd);

  return 0;
}
#include <errno.h>
#include <thread>
#include <strings.h>

#include "EventLoop.hh"
#include "Channel.hh"
#include "Poller.hh"


/*
void test()
{

	printf(" hello world  %d \n",CurrentThread::tid());

	CurrentThread::t_threadName = "test";

	printf("tid %s name : %s len : %d\n", CurrentThread::tidString(), CurrentThread::name(), CurrentThread::tidStringLength());
	sleep(2);
}


int main()
{
	std::thread thread1(test);
	std::thread thread2(test);
	std::thread thread3(test);
	CurrentThread::t_threadName = "main";

	printf(" main  name : %s %d \n",CurrentThread::name(), CurrentThread::tid());

	thread1.join();


}

*/

/*
EventLoop* g_loop;

void test()
{

  g_loop->loop();

}


int main()
{
  EventLoop testloop;

  //testloop.loop();

  g_loop = &testloop;

  std::thread test_thread(test);

  test_thread.join();

  return 0;
}

*/


//Reactor Test
//单次触发定时器
#include <sys/timerfd.h>

EventLoop* g_loop;

void timeout()
{
  printf("timeout!\n");
  g_loop->quit();
}

int main()
{

  EventLoop loop;
  g_loop = &loop;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK |TFD_CLOEXEC);

  Channel channel(&loop, timerfd);
  channel.setReadCallBack(timeout);
  channel.eableReading();

  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 3;
  timerfd_settime(timerfd, 0, &howlong, NULL);

  loop.loop();

  close(timerfd);

}



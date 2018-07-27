#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ThreadInterface.hh"
#include "MutexLock.hh"
#include "Thread.hh"
#include <unistd.h>
int cnt = 0;
MutexLock CntLock;


class TestThread : public Thread
{
	//TestThread()
	//~TestThread();
	void startThread(){
		MutexLockGuard loo(CntLock);
		//CntLock.lock();
		printf("%d\n", ++cnt);
		//CntLock.unlock();
		sleep(1);
	}
};


int main()
{
	printf("%d \n", cnt);
	TestThread Cntfun[10];
	int i ;
	for(i = 0; i < 10; i++){
		Cntfun[i].start();
	}

	for(i = 0; i < 10; i++){
		Cntfun[i].join();
	}

	getchar();
	return 0;
}

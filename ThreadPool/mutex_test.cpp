#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "MutexLock.hh"
#include "Thread.hh"
#include <unistd.h>
#include "Condition.hh"
#include "ThreadPool.hh"
#include <vector>

/*
int cnt = 0;
MutexLock CntLock;

Condition CondVal(CntLock);

int  x=0,y=5;

class TestThread : public Thread
{
	//TestThread()
	//~TestThread();
	void startThread(){
		MutexLockGuard loo(CntLock);
		//CntLock.lock();
		printf("%d\n", ++cnt);
		//CntLock.unlock();
		//sleep(1);
	}
};

class CondTestThread1 : public Thread
{
	void startThread(){
		MutexLockGuard lock(CntLock);
		while(x < y)
		{
			printf("thread1  wait ...\n");
			CondVal.wait();
		}
		printf("thread1  free ...\n");
	}

};

class CondTestThread2 : public Thread
{
	//TestThread()
	//~TestThread();
	void startThread(){
		while(1){
			x++;
			printf("%d\n", x);
			CondVal.notify();
			sleep(1);
		}
	}
};
*/
/*
//thread test

MutexLock CntLock;

int cnt = 0;

void test(void *arg){
	MutexLockGuard loo(CntLock);
	//CntLock.lock();
	printf("%d\n", ++cnt);
	//CntLock.unlock();
	//sleep(1);
}
*/


//threadPool test

MutexLock CntLock;

int cnt = 0;

void test(void){
	unsigned long i = 0xfffffff;
	//MutexLockGuard loo(CntLock);
	//CntLock.lock();
	while(i--);
	printf("%d\n", ++cnt);
	//CntLock.unlock();
	sleep(1);
}

int main()
{

	//thread test
/*	printf("%d \n", cnt);
	std::vector<Thread*> v;
	int i ;
	for(i = 0; i < 10; i++){
		v.push_back(new Thread(test, NULL));
	}

	for(i = 0; i < 10; i++){
		v[i]->start();
	}
*/

//Cond Test
/*
	CondTestThread1 t1;
	CondTestThread2 t2;

	t1.start();
	t2.start();
*/

//ThreadPool Test

	ThreadPool tp(5, 2);
	tp.start();

	sleep(1);
	for(int i = 0; i < 8; i++)
		tp.addTask(test);



	getchar();

	tp.stop();

	return 0;
}

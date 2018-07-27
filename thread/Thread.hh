#ifndef _THREAD_HH
#define _THREAD_HH

#include <pthread.h>
#include "MutexLock.hh"
/*
#define CHECK(exp) \
    if(!exp) \
{ \
    fprintf(stderr, "File:%s, Line:%d Exp:[" #exp "] check error, abort.\n",__FILE__, __LINE__); abort();\
}*/

class Thread{
public:
	Thread();
	~Thread();
	void start();
	void join();
	virtual void startThread() = 0;
	static void *threadRoutine(void *arg);
	pthread_t getThreadId() const{
		return m_threadId;
	}
private:
	pthread_t m_threadId;
	bool m_isRuning;
};//abstract class

#endif
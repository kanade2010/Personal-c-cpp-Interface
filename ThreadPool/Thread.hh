#ifndef _THREAD_HH
#define _THREAD_HH

#include <pthread.h>

class Thread{
public:
typedef void (*threadFun_t)(void *arg);

	explicit Thread(const threadFun_t &threadRoutine, void *arg);
	~Thread();
	void start();
	void join();
	static void *threadGuide(void *arg);
	pthread_t getThreadId() const{
		return m_threadId;
	}
private:
	pthread_t m_threadId;
	bool m_isRuning;
	threadFun_t m_threadRoutine;
	void *m_threadArg;
};

#endif
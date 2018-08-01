#ifndef _CONDITION_HH
#define _CONDITION_HH
#include "MutexLock.hh"
#include <pthread.h>

class Condition{
public:
	Condition(MutexLock &mutex);
	~Condition();
	void wait();//pthread_cond_wait
	void notify();//pthread_cond_signal
	void notifyAll();//pthread_cond_broadcast
private:
	MutexLock &m_mutex;
	pthread_cond_t m_cond;
};

#endif


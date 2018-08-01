//Condition.cpp

#include "Condition.hh"
#include "log.hh"

Condition::Condition(MutexLock &mutex):m_mutex(mutex){
	CHECK(!pthread_cond_init(&m_cond, NULL));
}

Condition::~Condition(){
	CHECK(!pthread_cond_destroy(&m_cond));
}

void Condition::wait(){
	//VERIFY(m_mutex.isLocking());
	CHECK(!pthread_cond_wait(&m_cond, m_mutex.getMutexPtr()));
	//m_mutex.restoreMutexStatus();
}

void Condition::notify(){
	CHECK(!pthread_cond_signal(&m_cond));
}

void Condition::notifyAll(){
	CHECK(!pthread_cond_broadcast(&m_cond));
}












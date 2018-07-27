#include "MutexLock.hh"

MutexLock::MutexLock():m_isLocking(false){
	CHECK(!pthread_mutex_init(&m_mutex, NULL));
}

MutexLock::~MutexLock(){
	CHECK(!isLocking());	//布尔值
	CHECK(!pthread_mutex_destroy(&m_mutex)); //整形值
}

void MutexLock::lock(){
	CHECK(!pthread_mutex_lock(&m_mutex));
	m_isLocking = true;
}

void MutexLock::unlock(){
	m_isLocking = false;  //先改状态再解锁,保证其原子性.
	CHECK(!pthread_mutex_unlock(&m_mutex));
}

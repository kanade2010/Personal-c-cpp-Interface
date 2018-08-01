#ifndef _THREADPOOL_HH
#define _THREADPOOL_HH

#include <stddef.h>
#include <vector>
#include <deque>
#include "Thread.hh"
#include "MutexLock.hh"
#include "Condition.hh"

class ThreadPool{
public:
	typedef void (*Task)(void);
	explicit ThreadPool(size_t tasksSize, size_t threadsSize);
	~ThreadPool();
	static void threadRoutine(void *arg);
	void start();
	void stop();
	Task take();
	void addTask(Task task);
private:
	std::vector<Thread *> m_threads;
	std::deque<Task> m_tasks;
	mutable MutexLock m_mutex;
	Condition m_tasksEmpty;
	Condition m_tasksFull;
	size_t m_tasksSzie;
	size_t m_threadsSize;
	bool m_isRuning;
};

#endif
#include "ThreadPool.hh"
#include "log.hh"

ThreadPool::ThreadPool(size_t tasksSize, size_t threadsSize)
	:m_tasksSzie(tasksSize),
	 m_threadsSize(threadsSize),
	 m_mutex(),
	 m_tasksEmpty(m_mutex),
	 m_tasksFull(m_mutex),
	 m_isRuning(false){
}

ThreadPool::~ThreadPool(){
	if(m_isRuning){
		stop();
	}
}

void ThreadPool::threadRoutine(void *arg){
	ThreadPool *p = static_cast<ThreadPool *>(arg);
	while(p->m_isRuning){
		ThreadPool::Task task(p->take());
		if(task){
			task();
		}
	}
}

ThreadPool::Task ThreadPool::take(){
	MutexLockGuard lock(m_mutex);
	while(m_tasks.empty() && m_isRuning){
		m_tasksEmpty.wait();
	}
	if(!m_tasks.empty()){
		Task task = m_tasks.front();
		m_tasks.pop_front();
		m_tasksFull.notify();
		return task;
	}
	return NULL;
}

void ThreadPool::addTask(Task task){
	if(m_threads.empty()){//如果线程池是空的,直接跑任务.
		task();
	}
	else{
		MutexLockGuard lock(m_mutex);
		while(m_tasksSzie > 0 && m_tasks.size() >= m_tasksSzie){
			m_tasksFull.wait();
		}

		m_tasks.push_back(task);
		m_tasksEmpty.notify();
	}
}

void ThreadPool::start(){
	m_isRuning = true;
	m_threads.reserve(m_threadsSize);
	for(size_t i = 0; i < m_threadsSize; i++){
		m_threads.push_back(new Thread(threadRoutine, this));
		m_threads[i]->start();
	}
}

void ThreadPool::stop(){
	{
		MutexLockGuard lock(m_mutex);
		m_isRuning = false;
		m_tasksEmpty.notifyAll();
	}
	for(int i = m_threadsSize - 1; i >= 0; i--){
		m_threads[i]->join();
		delete(m_threads[i]);
		m_threads.pop_back();
	}
}
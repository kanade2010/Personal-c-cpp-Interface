#include <stdio.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <pthread.h>

#define CHECK(exp) \
    if(!exp) \
{ \
    fprintf(stderr, "Error/(%s, %d):[" #exp "] check error, abort.\n", __FILE__, __LINE__); abort();\
}

#define VERIFY(exp) \
    if(!exp) \
{ \
    fprintf(stderr, "Error/(%s, %d):[" #exp "] verify error, abort.\n", __FILE__, __LINE__); abort();\
}

class Thread{
public:
typedef void (*threadFun_t)(void);

	explicit Thread(const threadFun_t &threadRoutine);
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
};



Thread::Thread(const threadFun_t &threadRoutine)
	:m_isRuning(false),
	 m_threadId(0),
	 m_threadRoutine(threadRoutine){
}

Thread::~Thread(){
	if(m_isRuning){//如果线程正在执行，则分离此线程.
		CHECK(!pthread_detach(m_threadId));
	}
}

void *Thread::threadGuide(void *arg){
	Thread *p = static_cast<Thread *>(arg);
	p->m_threadRoutine();
	return NULL;
}

void Thread::join(){
	VERIFY(m_isRuning);
	CHECK(!pthread_join(m_threadId, NULL));
	m_isRuning = false;
}

void Thread::start(){

	pthread_attr_t attr;

	CHECK(!pthread_attr_init(&attr));

	//CHECK(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));    //set thread separation state property

	CHECK(!pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));    //Set thread inheritance

	CHECK(!pthread_attr_setschedpolicy(&attr, SCHED_OTHER));                //set thread scheduling policy

	CHECK(!pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM));             //Set thread scope

	CHECK(!pthread_create(&m_threadId, &attr, threadGuide, this));

	m_isRuning = true;

}

typedef void (*Task)(void);

int cnt = 0;

void test(){
	printf("(%d)\n", ++cnt);
}

int main(){
	std::deque<Task> m_task;
	int i = 0;
	for(i = 0; i < 10; i++)
		m_task.push_back(test);

	for(i = 0; i< 10 ; i++){
		(*m_task.begin())();
		printf("task queue size %d\n", m_task.size());
		m_task.pop_front();
			if(m_task.empty()) printf("empty yes\n");

	}

	printf("task queue size %d\n", m_task.size());


	Thread t1(test);
	t1.start();
	printf("tag\n");
	Thread t2(test);
	t2.start();
	printf("tag2\n");

	std::vector<Thread*> vt;
	vt.push_back(new Thread(test));
	vt[0]->start();
		printf("tag3\n");

	vt.push_back(new Thread(test));
	vt[1]->start();
		printf("tag4\n");

	delete(vt[1]);
	vt.pop_back();
	
	delete(vt[0]);
	vt.pop_back();
		
		if(vt.empty())
		printf("tag4\n");

	getchar();
}

#include "EventLoop.hh"
#include "Logger.hh"
#include <assert.h>

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
	:m_looping(false),
  m_threadId(CurrentThread::tid())
{
  LOG_TRACE << "EventLoop Create " << this << " in thread " << m_threadId;
  if(t_loopInThisThread)
  {  //每个线程只有一个EventLoop对象 , 如果当前线程创建了其他 EventLoop对象,则终止程序.
    LOG_FATAL << "Anthor EventLoop " << t_loopInThisThread
              << " exists in this thread " << m_threadId;
  }
  else
  {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop()
{
  assert(!m_looping);
  t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
}

#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Logger.hh"
#include "TimerQueue.hh"

int createTimerfd()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}

TimerQueue::TimerQueue(EventLoop* loop)
  :m_pLoop(loop),
   m_timerfd(createTimerfd()),
   m_timerfdChannel(m_pLoop, m_timerfd)
{

}

TimerQueue::~TimerQueue()
{

}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeStamp now)
{
  std::vector<Entry> expired;
  Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>UINTPTR_MAX);
  TimerList::iterator it = m_timers.lower_bound(sentry);
  assert(it == m_timers.end() || now < it->first);
  std::copy(m_timers.begin(), it, back_inserter(expired));
  m_timers.erase(m_timers.begin(), it);

  return expired;
}


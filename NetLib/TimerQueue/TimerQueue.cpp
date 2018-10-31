#include <stdint.h>

#include "TimerQueue.hh"

TimerQueue::TimerQueue()
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


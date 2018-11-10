#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Logger.hh"
#include "EventLoop.hh"
#include "Timer.hh"
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

struct timespec howMuchTimeFromNow(TimeStamp when)
{
  int64_t microseconds = when.microSecondsSinceEpoch()
                         - TimeStamp::now().microSecondsSinceEpoch();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / TimeStamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % TimeStamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

void readTimerfd(int timerfd, TimeStamp now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}

void resetTimerfd(int timerfd, TimeStamp expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  }
}

TimerQueue::TimerQueue(EventLoop* loop)
  :p_loop(loop),
   m_timerfd(createTimerfd()),
   m_timerfdChannel(p_loop, m_timerfd),
   m_timers()
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



TimerId TimerQueue::addTimer(const NetCallBacks::TimerCallBack& cb, TimeStamp when, double interval)
{
  Timer* timer = new Timer(cb, when, interval);
  p_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->sequence());
}


void TimerQueue::addTimerInLoop(Timer* timer)
{
  p_loop->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if (earliestChanged)
  {
    resetTimerfd(m_timerfd, timer->expiration());
  }
}

bool TimerQueue::insert(Timer* timer)
{
  p_loop->assertInLoopThread();
  assert(m_timers.size() == m_activeTimers.size());
  bool earliestChanged = false;
  TimeStamp when = timer->expiration();
  TimerList::iterator it = m_timers.begin();
  if (it == m_timers.end() || when < it->first)
  {
    earliestChanged = true;
  }
  {
    std::pair<TimerList::iterator, bool> result
      = m_timers.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }
  {
    std::pair<ActiveTimerSet::iterator, bool> result
      = m_activeTimers.insert(ActiveTimer(timer, timer->sequence()));
    assert(result.second); (void)result;
  }

  assert(m_timers.size() == m_activeTimers.size());
  return earliestChanged;
}


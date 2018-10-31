#include "Timer.hh"

Timer::Timer(const NetCallBacks::TimerCallback& cb, TimeStamp when, double interval)
  :m_callBack(cb),
  m_expiration(when),
  m_interval(interval),
  m_repeat(interval > 0.0),
  m_sequence(s_numCreated.incrementAndGet())
{

}

void Timer::restart(TimeStamp now)
{
  if(m_repeat)
  {
    m_expiration = addTime(now, m_interval);
  }
  else
  {
    m_expiration = TimeStamp::invalid();
  }

}
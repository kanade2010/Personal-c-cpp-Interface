#ifndef _NET_TIMER_HH
#define _NET_TIMER_HH

#include "CallBacks.hh"
#include "TimeStamp.hh"
#include "Atomic.hh"

class Timer{
public:
  Timer(const NetCallBacks::TimerCallback& cb, TimeStamp when, double interval);

  void run() const
  {
    m_callBack();
  }

  TimeStamp expiration() const { return m_expiration; }
  bool repeat() const { return m_repeat; }
  int64_t sequence() const { return m_sequence; }
  void restart(TimeStamp now);

  static int64_t numCreated(){ return s_numCreated.get(); }

private:
  Timer& operator=(const Timer&);
  Timer(const Timer&);

  const NetCallBacks::TimerCallback m_callBack;
  TimeStamp m_expiration;
  const double m_interval;
  const bool m_repeat;
  const int64_t m_sequence;

  static AtomicInt64 s_numCreated;

};

#endif
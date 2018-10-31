#ifndef _NET_TIMERQUEUE_HH
#define _NET_TIMERQUEUE_HH

class TimerQueue
{
  TimerQueue();
  ~TimerQueue();

  // Schedules the callback to be run at given time,

  TimerId addTimer(const NetCallBacks::TimerCallback& cb, TimeStamp when, double interval);

  //void cancel(TimerId timerId);
  
private:
  typedef std::pair<TimeStamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;

  //called when timerfd alarms
  void handleRead();
  //move out all expired timers
  std::vector<Entry> getExpired(TimeStamp now);
  void reset(const std::vector<Entry>& expired, TimeStamp now);

  bool insert(Timer timer);

  EventLoop* m_pLoop;
  const int m_timerfd;
  Channel m_timerfdChannel;
  //Timer List sorted by expiration
  TimerList m_timers;

}

#endif

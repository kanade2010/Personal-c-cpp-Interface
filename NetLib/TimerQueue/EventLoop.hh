#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include <memory>
#include <vector>
#include <functional>

#include "CurrentThread.hh"

class Poller;
class Channel;

class EventLoop
{
public:
	typedef std::function<void()> Functor;

	EventLoop();
	~EventLoop();
	void loop();

	void assertInLoopThread()
	{
		if(!isInloopThread())
		{
			abortNotInLoopThread();
		}
	}

	bool isInloopThread() const {return m_threadId == CurrentThread::tid(); }
	void runInLoop(const Functor& cb);
  void queueInLoop(const Functor& cb);

	void quit();
	void updateChannel(Channel* channel);

	TimerId runAt(const TimeStamp& time, const NetCallBacks::TimerCallback& cb);
	TimerId runAfter(double delay, const NetCallBacks::TimerCallback& cb);
	TimerId runEvery(double interval, const NetCallBacks::& cb);

	static EventLoop* getEventLoopOfCurrentThread();

private:
	EventLoop& operator=(const EventLoop&);
	EventLoop(const EventLoop&);

	void abortNotInLoopThread();

	typedef std::vector<Channel*> ChannelList;

	bool m_looping;
	bool m_quit;
	const pid_t m_threadId;
	std::unique_ptr<Poller> m_poller;
	std::unique_ptr<TimerQueue> m_timerQueue;
	ChannelList m_activeChannels;


};

#endif
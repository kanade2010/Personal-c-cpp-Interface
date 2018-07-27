#ifndef _EVENT_LOOP_HH
#define _EVENT_LOOP_HH

class EventLoop
{
public:
	EventLoop();
	~EventLoop();
	void loop();
	void assertInLoopThread(){
		if(!isLoopThread())
		{
			abortNotInLoopThread();
		}
	}
	bool isLoopThread() const {return threadId == CurrentThread::tid();}

private:
	void abortNotInLoopThread();
	bool looping;
	const pid_t threadId;
}
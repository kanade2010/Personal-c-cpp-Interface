#ifndef _TIME_STAMP_HH
#define _TIME_STAMP_HH

#include <stdint.h>

class TimeStamp {
public:
//
// Constucts an invalid TimeStamp.
//
	TimeStamp()
	: m_microSecondsSinceEpoch(0){
	}

//
// Constucts a TimeStamp at specific time
//
// @param microSecondsSinceEpoch
	explicit TimeStamp(int64_t microSecondsSinceEpochArg)
	: m_microSecondsSinceEpoch(microSecondsSinceEpochArg){
	}

	int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }

//
// Get time of now.
//
	static TimeStamp now();

	static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
	int64_t m_microSecondsSinceEpoch;
};

#endif
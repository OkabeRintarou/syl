/****************************************************************************
STimer.h

A Timer class


Author : syl
Date : 2014/4/15

*****************************************************************************/
#include <windows.h>

#ifndef __SYL_STIMER_H__
#define __SYL_STIMER_H__

class STimer
{
public:
	STimer(){}
	~STimer(){}
public:
	bool Init()
	{
		if (!(QueryPerformanceFrequency(&_ticksPerSecond)))
		{
			return false;
		}
		
		QueryPerformanceCounter(&_startTime);
		return true;
	}

	float GetFPS(unsigned int elapsedFrames = 1)
	{
		static LARGE_INTEGER _lastTime = _startTime;
		LARGE_INTEGER currentTime;

		QueryPerformanceCounter(&currentTime);

		float ticksCounted = static_cast<float>(currentTime.QuadPart) - static_cast<float>(_lastTime.QuadPart);
		float frames = static_cast<float>(elapsedFrames) * static_cast<float>(_ticksPerSecond.QuadPart) / ticksCounted;

		_lastTime = currentTime;//update last time
		
		return frames;
	}
private:
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _ticksPerSecond;
	unsigned long _totalFrames;
};

#endif//__SYL_STIMER_H__
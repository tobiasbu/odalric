
#include "Timer.h"

namespace Odalric {

	Timer::Timer()
	{
#ifdef WIN32
		QueryPerformanceFrequency(&frequency);
		startCount.QuadPart = 0;
		endCount.QuadPart = 0;
#else
		startCount.tv_sec = startCount.tv_usec = 0;
		endCount.tv_sec = endCount.tv_usec = 0;
#endif

		stopped = 0;
		startTimeInMicroSec = 0;
		endTimeInMicroSec = 0;

		start();
	}

	Timer::~Timer()
	{

	}

	void Timer::start()
	{
		stopped = false;

		#ifdef WIN32
				QueryPerformanceCounter(&startCount);
		#else
				gettimeofday(&startCount, NULL);
		#endif
	}

	void Timer::stop()
	{
		stopped = true;
	}

	double Timer::update()
	{
#ifdef WIN32

		if (!stopped)
			QueryPerformanceCounter(&endCount);

		startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
		endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);

#else

		if (!stopped)
			gettimeofday(&endCount, NULL);

		startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
		endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;

#endif

		return endTimeInMicroSec - startTimeInMicroSec;
	}

	double Timer::elapsedMicro()
	{
		return update();
	}

	// in seconds
	double Timer::elapsed()
	{
		return this->elapsedMicro() * 0.000001;
	}

}
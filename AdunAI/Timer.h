//////////////////////////////////////////////////////////
// Timer.hpp
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////



#ifndef _OAI_TIMER_H_
#define _OAI_TIMER_H_

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

namespace Odarich {

	class Timer
	{
	private:

		double startTimeInMicroSec;                 // starting time in micro-second
		double endTimeInMicroSec;                   // ending time in micro-second
		bool    stopped = false;                             // stop flag 
		
#ifdef WIN32
		LARGE_INTEGER frequency;                    // ticks per second
		LARGE_INTEGER startCount;                   //
		LARGE_INTEGER endCount;                     //
#else
		timeval startCount;                         //
		timeval endCount;                           //
#endif

		double update();

	public:

		Timer();
		~Timer();
		void start();
		void stop();

		// micro seconds
		double elapsedMicro();

		// in seconds
		double elapsed();
	};

}

#endif

#ifndef _OAI_TASKCOMMONS_HPP_
#define _OAI_TASKCOMMONS_HPP_

#include <mutex>
#include <condition_variable>

namespace Odalric {

	static class TaskCommons
	{

	public:

		// global mutex between threads
		// use unique lock
		static std::mutex mutex;

		// status
		static std::condition_variable cv;

		// used by TerrainAnalyzer to check when we can access resouces areas
		static bool readyToAnalyze; // = false;
	

		
	};

}

#endif
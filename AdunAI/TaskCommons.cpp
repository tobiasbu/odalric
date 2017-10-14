
#include "TaskCommons.h"

namespace Odarich {

	std::mutex TaskCommons::mutex;
	std::condition_variable TaskCommons::cv;
}

/*namespace TaskCommons
{
	
	std::mutex & mutex()
	{
		return _mutex;
	}


	std::condition_variable & cv()
	{
		return _cv;
	}

	bool readyToAnalyze()
	{
		return _readyToAnalyze;
	};


}*/

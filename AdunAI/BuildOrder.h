
#ifndef _OAI_BUILDORDER_HPP_
#define _OAI_BUILDORDER_HPP_

#include "QueueConditions.h"
#include "PriorityQueue.h"
#include "BuildJob.h"
//#include "MetaMap.h"

#include <stack>



namespace Odarich {

	class BuildOrder : public PriotityQueue<BuildJob>
	{
	private:

		
		BuildJob * _currentJob = nullptr;
		

		//void updateAvailbleResources();
		
		//void jobFinished();
		//bool checkForRequeriments();
		//void doBuild();
		//bool doTrain(const MetaType & unitType); // return false if the build queue is full

	public:

		BuildOrder();
		//MetaMap meta;
		//void add(const BuildJob & job);
		void add(Action * action);
		void add(BuildJob & job, const int & quantity);
		void add(BuildJob & job, void * endCondition);
		
		

		void draw(const int & x, const int & y);

	};
	
}

#endif

#ifndef _OAI_WORKERSMANAGER_H_
#define _OAI_WORKERSMANAGER_H_

#include "WorkersInfo.h"

#include "Singleton.hpp"

namespace Odarich {

	class Building;

	class WorkersManager : public Singleton<WorkersManager>
	{

	private:

		int workersSentToAttack = 0;

		void manageIdle(Worker & unit);
		void manageReturnBurden(Worker & unit);
		void manageRefinaries();

		// this function check if the current workers really doing what we said he to do
		void checkWorkerConsistency(Worker & unit);
		BWAPI::Unit enemyAround(BWAPI::Unit worker);
		
		
		void setWorkerJobToClosestBase(Worker & unit, const Worker::Job & job);


	public:

		WorkersManager();
		WorkersInfo workersInfo;
		void update();
		void setWorkerJob(Worker * unit, const Worker::Job & job);
		void setBuilderWorkerJob(Worker & unit, const Building & building);
		void workComplete(Worker & worker);
		Worker & getFreeWorker();
		Worker * getBuilderWorker(const Building & building);
		Worker * getFreeWorkerForGas(BWAPI::Unit refinary);

		void onUnitShow(BWAPI::Unit unit);
		void onUnitDestroy(BWAPI::Unit unit);
		void onUnitMorph(BWAPI::Unit unit);
		void draw(const int & x, const int & y);
	};

}

#endif

#ifndef _OAI_BASE_H_
#define _OAI_BASE_H_

#include <BWAPI.h>
#include "Types.h"

#include "ResourcesArea.h"
#include "Worker.h"

namespace Odarich {

	class Base
	{
	private:

		bool _hasBuildBounds = false;
		std::vector<Bounds> _boundsList;
		//std::vector<Worker*> _assignedWorkers;

		// workers per mineral
		std::map<BWAPI::Unit, int> _workersOnMineral;  

		// worker mineral patch
		std::map<BWAPI::Unit, BWAPI::Unit> _workerMineral;

		// worker refinaries path
		std::map<BWAPI::Unit, BWAPI::Unit> _workerRefinaries;

		// workers per refinaries count
		std::map<BWAPI::Unit, int> _workersOnRefinaries;

		

		// assignment workers to this base
		std::unordered_set<Worker*> _assignedWorkersSet;
		std::map<const Worker::Job, uint32> _assignedWorkerCount;

		// resource depot it self
		BWAPI::Unit _unit;

		// resources arround
		ResourcesArea * _resources = nullptr;

	public:


		Base(BWAPI::Unit resourceDepot);

	
		//void assignWorker(Worker * unit);
		void assignGasWorker(Worker * unit, BWAPI::Unit geyser);
		void assignMineralWorker(Worker * unit, BWAPI::Unit mine);
		void unassignWorker(Worker * unit);
		void addMineralPath(BWAPI::Unit unit, const int & count);
		void addRefinaryPath(BWAPI::Unit refinary, const int & count);
		BWAPI::Unit getBestMine(BWAPI::Unit worker);
		BWAPI::Unit getBestRefinary();

		void setupResources(ResourcesArea * resources);
		void setHasBuildBounds(const bool & flag);
		void addBuildBounds(const Bounds & bounds);

		uint32 getWorkersJobCount(const Worker::Job & job) const;
		bool isFull(const Worker::Job & job) const;
		int32 refinariesCount();
		int32 refinariesUnderConstruction();
		bool hasBuildBounds() const;
		BWAPI::Unit & get();
		ResourcesArea * getResources() const;
		const std::vector<Bounds> & getBuildBounds() const;
	};

}

#endif
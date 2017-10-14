
#include "WorkersInfo.h"
#include "Log.h"

namespace Odalric {

	void WorkersInfo::addUnit(Worker & Worker)
	{
		//if (!Worker.get())
		//	return;

		//_workersUnit.push_back(Worker);
		_workersMap.insert(std::make_pair(Worker.get(), Worker)); //_workersUnit.size() - 1
	}

	void WorkersInfo::add(BWAPI::Unit worker)
	{
		if (!worker) 
			return;

		

		//std::map<BWAPI::Unit, int>::iterator it = workersMap.find(worker);

		//if (it != workersMap.end())
		//{
		std::pair<BWAPI::Unitset::iterator, bool> it = _workers.insert(worker);
		
			if (it.second) 
			{
				//addUnit(Worker(it.first));
				_workersMap.insert(std::make_pair(worker, Worker(worker)));
			}
		//}
	}

	void WorkersInfo::add(BWAPI::Unit worker, Worker::Job job)
	{
		if (!worker)
			return;

		std::pair<BWAPI::Unitset::iterator, bool> it = _workers.insert(worker);

		if (it.second)
		{
			addUnit(Worker(*it.first));
		}
	}

	void WorkersInfo::destroy(BWAPI::Unit worker)
	{
		if (!worker)
			return;

		WorkersMap::iterator it = _workersMap.find(worker);

		if (it != _workersMap.end()) {
			_workersMap.erase(it);
		}

		//_workersMap.erase(worker);
		_workers.erase(worker);
	}

	WorkersMap &  WorkersInfo::getWorkers()
	{
		return _workersMap;
	}

	BWAPI::Unitset & WorkersInfo::getWorkersSet()
	{
		return _workers;
	}
	
	int WorkersInfo::count() const
	{
		return _workers.size();
	}

}
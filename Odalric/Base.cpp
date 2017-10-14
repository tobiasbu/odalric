
#include "Commons.h"
#include "Base.h"
#include "Math.h"

namespace Odalric 
{
	Base::Base(BWAPI::Unit  resourceDepot)
	{
		_unit = resourceDepot;

		_assignedWorkerCount[Worker::Job::GatherMineral] = 0;
		_assignedWorkerCount[Worker::Job::GatherGas] = 0;
	}

	void Base::setupResources(ResourcesArea * resources)
	{
		OAI_ASSERT_MSG(resources || resources != nullptr, "Resources is NULL");

		if (!resources || resources == nullptr)
			_resources = nullptr;
		else
			_resources = resources;
	}

	/*void Base::assignWorker(Worker * unit)
	{

		//_assignedWorkers.push_back(unit);
		//_assignedWorkersMap.insert(std::make_pair(unit, _assignedWorkers.size() - 1));
		

		
	}*/

	void Base::assignGasWorker(Worker * unit, BWAPI::Unit geyser)
	{
		auto var = _assignedWorkersSet.insert(unit);
		if (var.second)
		{
			_assignedWorkerCount[Worker::Job::GatherGas]++;
			_workerRefinaries[unit->get()] = geyser;
			unit->setBase(this);
		}
	}

	void Base::assignMineralWorker(Worker * unit, BWAPI::Unit mine)
	{
		auto var = _assignedWorkersSet.insert(unit);
		if (var.second)
		{
			_assignedWorkerCount[Worker::Job::GatherMineral]++;
			_workerMineral[unit->get()] = mine;
			unit->setBase(this);
		}
	}

	void Base::unassignWorker(Worker * unit)
	{
		//std::map<Worker*, uint32>::iterator it = _assignedWorkersSet.find(unit);
		std::unordered_set<Worker*>::iterator it =  _assignedWorkersSet.find(unit);

		if (it != _assignedWorkersSet.end())
		{
			BWAPI::Unit worker = unit->get();

			_assignedWorkersSet.erase(it);
			_assignedWorkerCount[unit->getJob()]--;
			

			if (unit->getJob() == Worker::Job::GatherMineral)
			{
				addMineralPath(_workerMineral[worker], -1);
				_workerMineral.erase(worker);
			}
			else if (unit->getJob() == Worker::Job::GatherGas)
			{
				addRefinaryPath(_workerRefinaries[worker], -1);
				_workerRefinaries.erase(worker);
			}

			unit->setBase(nullptr);
		}

		/*if (it != _assignedWorkersMap.end())
		{
			_assignedWorkers.erase(_assignedWorkers.begin() + it->second);
			_assignedWorkerCount[unit->getJob()]--;
		}*/
	}

	void Base::addMineralPath(BWAPI::Unit unit, const int & count)
	{
		if (_workersOnMineral.find(unit) == _workersOnMineral.end())
		{
			_workersOnMineral[unit] = count;
		}
		else
		{
			_workersOnMineral[unit] += count;
		}
	}

	void Base::addRefinaryPath(BWAPI::Unit unit, const int & count)
	{
		if (_workersOnRefinaries.find(unit) == _workersOnRefinaries.end())
		{
			_workersOnRefinaries[unit] = count;
		}
		else
		{
			_workersOnRefinaries[unit] += count;
		}

	}

	BWAPI::Unit Base::getBestMine(BWAPI::Unit worker)
	{
		if (_resources == nullptr)
			return nullptr;

		if (!worker) 
			return nullptr;

		int bestAssignedCount = 100;
	
		//int distx, disty;
		//int bestDistx = 10000, bestDisty = 10000;
		BWAPI::Unit bestMineral = nullptr;

		for each (auto mineral in _resources->getMinerals())
		{
			if (!mineral)
				continue;

			int assignedCount = _workersOnMineral[mineral];
			
			//Math::manhattan(_unit->getPosition(), worker->getPosition(), distx, disty);

			if (assignedCount < bestAssignedCount ||
				assignedCount == bestAssignedCount)
				//(distx < bestDistx && disty < bestDisty))
			{
				//bestDistx = distx;
				//bestDisty = disty;
				bestMineral = mineral;
				bestAssignedCount = assignedCount;
			}
		}

		return bestMineral;
	}

	BWAPI::Unit Base::getBestRefinary()
	{


		if (_resources != nullptr)
		{
			const BWAPI::Unitset & geysers = _resources->getGas();
			int geysersCount = geysers.size();


			if (geysersCount >= 1)
			{
				int bestAssignedCount = 100;
				BWAPI::Unit bestRefinary = nullptr;
	

				for each (auto gas in geysers)
				{
					if (!gas)
						continue;

					if (!gas->getType().isRefinery())
						continue;

					if (gas->isConstructing())
						continue;

					int assignedCount = _workersOnRefinaries[gas];

					if (assignedCount < bestAssignedCount ||
						assignedCount == bestAssignedCount)
						//(distx < bestDistx && disty < bestDisty))
					{
						//bestDistx = distx;
						//bestDisty = disty;
						bestRefinary = gas;
						bestAssignedCount = assignedCount;
					}
				}

				return bestRefinary;
			}
			else
				return nullptr;
		}
		else
			return nullptr;

		
	}

	void Base::setHasBuildBounds(const bool & flag)
	{
		_hasBuildBounds = flag;
	}

	void Base::addBuildBounds(const Bounds & bounds)
	{
		_boundsList.push_back(bounds);
	}

	uint32 Base::getWorkersJobCount(const Worker::Job & job) const
	{
		std::map<const Worker::Job, uint32>::const_iterator it =  _assignedWorkerCount.find(job);

		if (it == _assignedWorkerCount.end())
			return 0;
		else {
			return it->second;
		}
	}

	bool Base::isFull(const Worker::Job & job) const
	{
		if (!_unit) // !_unit->exists()
			return false;

		if (_resources != nullptr) {

			uint32 count = getWorkersJobCount(job);
			uint32 resCount = 0;
			if (job == Worker::Job::GatherMineral)
				resCount = _resources->getMinerals().size();
			else if (job == Worker::Job::GatherGas)
				resCount = _resources->getGas().size() * 3;

			if (count >= resCount)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	int32 Base::refinariesCount()
	{
		if (_resources != nullptr)
		{

			const BWAPI::Unitset & geysers = _resources->getGas();
			int geysersCount = geysers.size();
			int counter = 0;

			if (geysersCount >= 1)
			{

					for each (auto gas in geysers)
					{
						if (gas->getType().isRefinery() && gas->isCompleted())
						{
							counter++;
						}
						else
							continue;
					}
			}

			return counter;
			/*const BWAPI::Unitset & set = _resources->getGas();

			if (set.empty())
				return 0;
			else {
				
			*/
		}
		else
			return 0;
	}

	int32 Base::refinariesUnderConstruction()
	{
		if (_resources != nullptr)
		{

			const BWAPI::Unitset & geysers = _resources->getGas();
			int geysersCount = geysers.size();
			int counter = 0;

			if (geysersCount >= 1)
			{
				// update availble refinaries
				//if (geysersCount != _workerRefinaries.size())
				//{

				for each (auto gas in geysers)
				{
					if (gas->getType().isRefinery() && gas->isConstructing())
					{
						counter++;
					}
					else
						continue;
				}
				//}
			}

			return counter;
			/*const BWAPI::Unitset & set = _resources->getGas();

			if (set.empty())
			return 0;
			else {

			*/
		}
		else
			return 0;
	}

	BWAPI::Unit & Base::get()
	{
		return _unit;
	}

	bool Base::hasBuildBounds() const
	{
		return _hasBuildBounds;
	}

	ResourcesArea * Base::getResources() const
	{
		return _resources;
	}

	const std::vector<Bounds> &  Base::getBuildBounds() const
	{
		return _boundsList;
	}
}
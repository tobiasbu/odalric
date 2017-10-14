
#include "Worker.h"
#include "Log.h"
#include "Base.h"
#include "ResourcesAreaManager.h"
#include "Utils.h"
#include "Building.h"

namespace Odarich {

	

	Worker::Worker()
	{
		_job = Worker::Job::Idle;
	}

	/*Worker::Worker(BWAPI::Unitset::iterator & it)
	{
		this->_it = it;
		this->_unit = (*it);
		_job = WorkerJob::Idle;
	}*/

	Worker::Worker(BWAPI::Unit bwunit)
	{
		_unit = bwunit;
		_job = Worker::Job::Idle;
		
	}

	Worker::Worker(const Worker::Job & job)
	{
		_previousJob = _job;
		this->_job = job;
	}

	void Worker::setJob(const Worker::Job & job)
	{
		if (!_unit) // || !_unit->exists()
			return;

		_previousJob = this->_job;
		_job = job;
	}

	void Worker::setBuilding(const Building * building)
	{
		_building = building;
	}

	void Worker::setUnitJob(BWAPI::Unit unitJob)
	{
		_unitJob = unitJob;
	}

	void Worker::setBase(Base * base)
	{
		_base = base;
	}

	BWAPI::Unit Worker::get()
	{
		return _unit;
	}

	Worker::Job Worker::getJob() const
	{
		return _job;
	}

	Worker::Job Worker::getPreviousJob() const
	{
		return _previousJob;
	}


	Base * Worker::getBase() const
	{
		return _base;
	}


	BWAPI::Unit Worker::getUnitJob()
	{
		return _unitJob;
	}

	const Building * Worker::getBuilding() const
	{
		return _building;
	}

	//void Worker::setJob(const WorkerJob & job, Base * base)
	//{
	//	if (!bwunit || !bwunit->exists())
	//		return;

	//	previousJob = this->job;
	//	this->job = job;

	//	if (job == WorkerJob::ReturnBurden)
	//	{
	//		if (!base->get())
	//			return;

	//		Utils::smartRightClick(this->bwunit, base->get());

	//	}
	//	else if (job == WorkerJob::GatherMineral)
	//	{
	//		ResourcesArea * area = base->getResources();

	//		//Log::info("get area");

	//		if (area != nullptr) {

	//			//Log::info("get mine");

	//			const BWAPI::Unitset & set = area->getMinerals();

	//			

	//			if (!set.empty()) {

	//				//Log::info("mines: %d", set.size());
	//				
	//				BWAPI::Unitset::iterator it = set.begin();

	//				Utils::smartRightClick(this->bwunit, (*it));
	//				//base->assignMineralWorker(this);

	//				/*if (bwunit->gather((*it)))
	//				{
	//					base->assignWorker(this);
	//				}
	//				else {

	//				}*/

	//			}
	//			else {
	//				//Log::info("no mines");
	//			}
	//		}
	//		else {

	//			// Harvest from the nearest mineral patch
	//			if (!bwunit->gather(bwunit->getClosestUnit(BWAPI::Filter::IsMineralField)))
	//			{
	//				// If the call fails, then print the last error message
	//				Log::error(BWAPI::Broodwar->getLastError().toString());

	//				// fallback
	//				// check to all knowed mineral fields
	//				BWAPI::Unit mineralField = ResourcesAreaManager::getNearestResourceFromAll(base->get(), BWAPI::UnitTypes::Resource_Mineral_Field, 300);


	//				if (bwunit->gather(mineralField))
	//				{
	//					//base->assignMineralWorker(this);

	//				}
	//			}

	//			// BWAPI::UnitTypes::Enum::Resource_Mineral_Field
	//			
	//		}
	//	}
	//}

	

}
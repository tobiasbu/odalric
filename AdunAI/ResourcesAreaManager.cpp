
#include "ResourcesAreaManager.h"
#include "TerrainAnalyzer.h"
#include "BaseManager.h"
#include "Math.h"
#include "Utils.h"
#include "Log.h"
#include "TaskCommons.h"
#include "Draw.h"


namespace Odarich {


	ResourcesAreaManager::ResourcesAreaManager()
	{
		
	}

	ResourcesAreaManager::~ResourcesAreaManager()
	{
	
	}

	void ResourcesAreaManager::init()
	{
		// add all resources to find places
		addSet(BWAPI::Broodwar->getMinerals());
		addSet(BWAPI::Broodwar->getGeysers());


		_taskAnalyzeResourcesArea = new std::thread(&Odarich::ResourcesAreaManager::taskJoinClusters, this);
	}

	void ResourcesAreaManager::addMineral(const BWAPI::Unit & mineral)
	{
		if (!mineral)
			return;

		if (mineral->getResources() <= 0)
			return;

		_minerals.insert(mineral);

		//_mutex.lock();
		//TaskCommons::mutex.lock();
		std::unique_lock<std::mutex> lock(TaskCommons::mutex);
		if (!_analyzedResources.contains(mineral))
		{
			_unanalyzedResources.insert(mineral);
			_unanalyzedCount++;
		}
		lock.unlock();
		//_mutex.unlock();
		//TaskCommons::mutex.unlock();
	}

	void ResourcesAreaManager::addGas(const BWAPI::Unit & gas)
	{
		if (!gas)
			return;

		if (gas->getResources() <= 0)
			return;

		_gas.insert(gas);



		//TaskCommons::mutex.lock();
		std::unique_lock<std::mutex> lock(TaskCommons::mutex);
		if (!_analyzedResources.contains(gas))
		{
			_unanalyzedResources.insert(gas);
			_unanalyzedCount++;
		}
		lock.unlock();
	}

	void ResourcesAreaManager::addSet(const BWAPI::Unitset & set)
	{
		for each(auto var in set)
		{
			if (!var)
				continue;

			const BWAPI::UnitType & type = var->getType();

			if (type == BWAPI::UnitTypes::Resource_Mineral_Field)
				addMineral(var);
			else if (type == BWAPI::UnitTypes::Resource_Vespene_Geyser)
				addGas(var);
		}
	}

	void ResourcesAreaManager::update()
	{
		if (_taskFinished)
		{
			/*if (_taskAnalyzeResourcesArea->joinable())
				_taskAnalyzeResourcesArea->join();

			delete _taskAnalyzeResourcesArea;

			_taskAnalyzeResourcesArea = nullptr;*/
			_nextFrameAnalyze = BWAPI::Broodwar->getFrameCount() + 10;
			_taskFinished = false;
		}
		else {
			if (!_taskRunning)
			{
				if (_unanalyzedResources.size() > 2)
				{

					if (BWAPI::Broodwar->getFrameCount() - _nextFrameAnalyze >= 0)
					{

						_taskRunning = true;

					}

				}
				else {

					/*if (Math::abs(BWAPI::Broodwar->getFrameCount() - _nextFrameAnalyze) >= 100)
					{

						_taskRunning = true;

					}*/
				}
			}
		}

	}

	void ResourcesAreaManager::taskJoinClusters()
	{
		while (_runTask) {

			if (_taskRunning) {

				if (_taskStatus == 0)
				{

					_taskStatus = 1;

					BWAPI::Unitset analyze;

					//_mutex.lock();
					std::unique_lock<std::mutex> lock(TaskCommons::mutex);
					analyze.insert(_unanalyzedResources.begin(), _unanalyzedResources.end());
					_unanalyzedResources.clear();
					//_unanalyzedGas.clear();
					_unanalyzedCount = 0;
					lock.unlock();

					//Log::info("hi TASK: %d", analyze.size());
					ResourcesAreasList newResourcesAreas;

					{
						
						BWAPI::Unitset::iterator itmineral; //= analyzeMinerals.begin();
						//BWAPI::Unitset::iterator itgas;


						// for each mineral
						itmineral = analyze.begin();
						while (itmineral != analyze.end())
						{
							if (*itmineral)
							{
								bool added = false;

								// check for knowed resources area
								for each (auto resource in _resourcesAreas)
								{
									if (!resource)
										continue;

									added = checkResourceOnArea(resource, *itmineral);

									if (added)
									{
										lock.lock();
										_analyzedResources.insert(*itmineral);
										resource->add(*itmineral);
										lock.unlock();
										itmineral = analyze.erase(itmineral);
										break;
									}
								}

								// check for future resources areas
								if (!added) {
									for each (auto resource in newResourcesAreas)
									{
										if (!resource)
											continue;

										added = checkResourceOnArea(resource, *itmineral);

										if (added)
										{
											lock.lock();
											_analyzedResources.insert(*itmineral);
											resource->add(*itmineral);
											lock.unlock();
											itmineral = analyze.erase(itmineral);
											break;
										}
									}
								}

								// create a new resources area for the future
								if (!added) {


									ResourcesArea * r = new ResourcesArea(*itmineral);
									r->add(*itmineral);
									newResourcesAreas.push_back(r);
									lock.lock();
									_analyzedResources.insert(*itmineral);
									lock.unlock();
									itmineral = analyze.erase(itmineral);

								}

							}
							else
								itmineral++;
						}

					}

					

					// for the new resources areas check if we can merge					
					ResourcesAreasList::iterator aIt = newResourcesAreas.begin();
					ResourcesAreasList::iterator bIt;

					while (aIt != newResourcesAreas.end())
					{
						if ((*aIt))
						{

							//ResourcesArea * areaToMerge = nullptr;
							bool merge = false;
							bIt = newResourcesAreas.begin();

							while (bIt != newResourcesAreas.end())
							{
								if ((*bIt) && (*aIt) != (*bIt))
								{
									if (Utils::isClose((*aIt)->getBounds(), (*bIt)->getBounds(), 150))
									{
										merge = true;
										break;
									}
								}

								if (!merge)
									bIt++;
							}

							if (merge)
							{
								(*aIt)->addSet((*bIt)->getMinerals());
								(*aIt)->addSet((*bIt)->getGas());
								ResourcesArea * r = *bIt;
								newResourcesAreas.erase(bIt);
								delete r;
								// restart loop
								aIt = newResourcesAreas.begin();
							}
							else
							{
								aIt++;
							}
						}
						else
						{
							aIt++;
						}
					}

					// add new resources areas
					lock.lock();
					_resourcesAreas.insert(_resourcesAreas.end(), newResourcesAreas.begin(), newResourcesAreas.end());
					lock.unlock();

					_taskStatus = 2;
					TaskCommons::cv.notify_one();
				}
				else if (_taskStatus == 2)
				{
					_runTask = false;
					_taskRunning = false;
					_taskFinished = true;
				}

				
				/*
				WE DONT NEED MORE THAT
				BasesMap & bases = BaseManager::getInstance().getBases();
				BasesMap::iterator itbase = bases.begin();

				for (; itbase != bases.end(); ++itbase)
				{

					ResourcesArea * r = itbase->second.getResources();
					//bool requireNew = false;

					// this base has not a resources area
					if (r == nullptr) {

						// for each resources area
						for each (auto resource in _resourcesAreas)
						{
							if (!resource)
								continue;



							// check the closest
							if (Utils::isClose(itbase->first->getPosition(), BWAPI::Position(resource->getBounds().centroid().x, resource->getBounds().centroid().y), 300))
							{
								_mutex.lock();
								itbase->second.setupResources(resource);
								_mutex.unlock();
								break;
							}

						}
					}
					else
						continue;
				}*/

				//Log::info("we had: %d", analyze.size());
				
			}
	 else
		 continue;
			
			
		}

		//Log::info("task finished");
				/*r = new ResourcesArea(&it->second);
				//requireNew = true;



				// for each mineral
				itmineral = analyzeMinerals.begin();
				while (itmineral != analyzeMinerals.end())
				{
					if ((*itmineral))
					{

						if (Utils::isClose(it->first->getPosition(), (*itmineral)->getPosition(), 300))
						{
							r->addMineral((*itmineral));
							itmineral = analyzeMinerals.erase(itmineral);
						}
						else {
							itmineral++;
						}
					}
					else
						itmineral++;
				}

				// for each gas vespene, check nearest of this base
				itgas = analyzeGas.begin();
				while (itgas != analyzeGas.end())
				{
					if ((*itgas))
					{
						if (Utils::isClose(it->first->getPosition(), (*itgas)->getPosition(), 300))
						{
							r->addGas((*itgas));
							itgas = analyzeGas.erase(itgas);
						}
						else {
							itgas++;
						}
					}
					else
						itgas++;
				}

				//if (requireNew)
				//{
				// add new resources area
				

				// add resources area to the base
				_mutex.lock();
				it->second.setupResources(r);
				_mutex.unlock();
				//}
			}
		}*/


		// for each mineral
		/*while (itmineral != analyzeMinerals.end()) {

			// is valid
			if ((*itmineral))
			{
				// for each base
				for (; it != bases.end() && itmineral != analyzeMinerals.end(); ++it)
				{
					// this base has a resources area?
					bool requireNew = false;
					ResourcesArea * r = it->second.getResources();

					if (r == nullptr) {
						r = new ResourcesArea();
						requireNew = true;
					}

					if ((*itmineral))
					{

						if (Utils::isClose(it->first->getPosition(), (*itmineral)->getPosition(), 300))
						{
							r->addMineral((*itmineral));
							itmineral = analyzeMinerals.erase(itmineral);
						}
						else {
							itmineral++;
						}
					}

				}

			}
			else
				itmineral++;
		}*/

		// for each base
		/*
		for (; it != bases.end(); ++it)
		{
				// this base has a resources area
				ResourcesArea * r = it->second.getResources();
				//bool requireNew = false;

				if (r == nullptr) {

					r = new ResourcesArea(&it->second);
					//requireNew = true;



				// for each mineral
					itmineral = analyzeMinerals.begin();
					while (itmineral != analyzeMinerals.end())
					{
						if ((*itmineral))
						{

							if (Utils::isClose(it->first->getPosition(), (*itmineral)->getPosition(), 300))
							{
								r->addMineral((*itmineral));
								itmineral = analyzeMinerals.erase(itmineral);
							}
							else {
								itmineral++;
							}
						}
						else
							itmineral++;
					}

					// for each gas vespene, check nearest of this base
					itgas = analyzeGas.begin();
					while (itgas != analyzeGas.end())
					{
						if ((*itgas))
						{
							if (Utils::isClose(it->first->getPosition(), (*itgas)->getPosition(), 300))
							{
								r->addGas((*itgas));
								itgas = analyzeGas.erase(itgas);
							}
							else {
								itgas++;
							}
						}
						else
							itgas++;
					}

					//if (requireNew)
					//{
						// add new resources area
						_resourcesAreas.push_back(r);

						// add resources area to the base
						_mutex.lock();
						it->second.setupResources(r);
						_mutex.unlock();
					//}
				}
		}*/

		// if we still have minerals

		
	}

	bool ResourcesAreaManager::checkResourceOnArea(ResourcesArea * area, BWAPI::Unit resource)
	{

		Bounds bounds = area->getBounds();
		bounds.grow(200);

		if (bounds.intersects(resource))
		{
			return true;
			//break;
		}
		else
		{
			for each (auto mineral in area->getMinerals())
			{
				if (!mineral)
					continue;

				if (resource->getDistance(Utils::unitMidPoint(mineral)) < 150)
				{

					return true;
					break;
				}
				else
					continue;
			}

			//if (!added) {

				for each (auto gas in area->getGas())
				{
					if (!gas)
						continue;

					if (resource->getDistance(Utils::unitMidPoint(gas)) < 150)
					{

						//added = true;
						return true;
						break;
					}
					else
						continue;
				}
			//}
		}

		return false;
	}

	ResourcesArea * ResourcesAreaManager::findResourcesAt(const BWAPI::PositionOrUnit & target, const int & maxRadius)
	{

		BWAPI::Unitset analyze;

		for (auto & unit : BWAPI::Broodwar->neutral()->getUnits())
		{
			if (!unit)
				continue;
			// too slow
			//unit->getDistance(target) <= radius

			// instead use simple manhattan distance 
			int dx = 0, dy = 0;

			Math::manhattan(
				unit->getPosition().x, unit->getPosition().y,
				target.getPosition().x, target.getPosition().y,
				dx, dy
			);

			if (unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
			{
				if (dx <= maxRadius && dy <= maxRadius)
				{
					//addMineral(unit);//_minerals.insert(unit);
					analyze.insert(unit);
				}
			}
			else if (unit->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
			{
				if (dx <= maxRadius && dy <= maxRadius)  // unit->getDistance(target) < gasRadius
				{
					//_gas.insert(unit);
					//addGas(unit);
					analyze.insert(unit);

				}
			}
			else
				continue;
		}

		if (!analyze.empty())
		{

			ResourcesArea * r = new ResourcesArea(target);
			// lock
			std::unique_lock<std::mutex> lock(TaskCommons::mutex);
			_analyzedResources.insert(analyze.begin(), analyze.end());
			r->addSet(analyze);
			_resourcesAreas.push_back(r);
			lock.unlock();
			return r;
		} 

		
		return nullptr;
	}

	const BWAPI::Unitset & ResourcesAreaManager::getMinerals()
	{
		return _minerals;
	}

	const BWAPI::Unitset & ResourcesAreaManager::getGas()
	{
		return _gas;
	}
	//const BWAPI::UnitTypes::Enum::Enum
	BWAPI::Unit ResourcesAreaManager::getNearestResourceFromAll(const BWAPI::PositionOrUnit & from, const BWAPI::UnitType & type, const int & maxRadius)
	{
		
		//int closeDistx = maxRadius, closeDisty = maxRadius;
		//int distx, disty;
		BWAPI::Unit closeResource = Utils::getNearestUnit(from, type, BWAPI::Broodwar->neutral()->getUnits(), maxRadius); //nullptr;;

		/*for (auto & unit : BWAPI::Broodwar->neutral()->getUnits())
		{
			if (!unit)
				continue;

			if (type != unit->getType())
				continue;


				distx = 0, disty = 0;

				Math::manhattan(
					unit->getPosition(),
					target.getPosition(),
					distx, disty
					);

				if (distx <= closeDistx && disty <= closeDisty && unit->getResources() > 0)
				{
					closeDistx = distx;
					closeDisty = disty;
					closeResource = unit;
				}
			
			
		}*/

		return closeResource;
	}

	const ResourcesAreasList & ResourcesAreaManager::getResourcesAreas()
	{
		return _resourcesAreas;
	}

	short ResourcesAreaManager::getTaskStatus() const
	{
		return _taskStatus;
	}

	bool  ResourcesAreaManager::isTaskFinished() const
	{
		return _taskFinished;
	}

	void ResourcesAreaManager::onUnitShow(const BWAPI::Unit & unit)
	{

		if (!unit)
			return;

		//int id = unit->getType().getID();

		if (unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) // BWAPI::UnitTypes::Enum::Enum::Resource_Mineral_Field
		{
			addMineral(unit);
		}
		else if (unit->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
		{
			addGas(unit);
		}

	}


	void ResourcesAreaManager::draw(const int & x, const int & y)
	{
		BWAPI::Broodwar->drawTextScreen(x, y, "Minerals: %d", _minerals.size());
		BWAPI::Broodwar->drawTextScreen(x, y+16, "Task: %d", _taskRunning);
		
		BWAPI::Broodwar->drawTextScreen(x, y + 32, "Un: %d", _unanalyzedResources.size());
		BWAPI::Broodwar->drawTextScreen(x, y + 48, "res: %d", _resourcesAreas.size());

		if (!_taskRunning)
		{
			for each (auto resource in _resourcesAreas)
			{
				Draw::bounds(resource->getBounds(), BWAPI::Colors::Blue);

				if (resource->getBaseLocation() != BWAPI::TilePositions::None)
				{
					Draw::tile4x3(resource->getBaseLocation(), BWAPI::Colors::Green);
				}
				/*BWAPI::Broodwar->drawBoxMap(
					resource->getBounds().getMin().x, resource->getBounds().getMin().y,
					resource->getBounds().getMax().x, resource->getBounds().getMax().y,
					BWAPI::Colors::Blue);*/
			}
		}
	}

	void ResourcesAreaManager::dispose()
	{
		if (_taskAnalyzeResourcesArea != nullptr) {

			_runTask = false;

		if (_taskAnalyzeResourcesArea->joinable())
			_taskAnalyzeResourcesArea->join();

		delete _taskAnalyzeResourcesArea;

		_taskAnalyzeResourcesArea = nullptr;
		_taskFinished = false;
		}
	}

}
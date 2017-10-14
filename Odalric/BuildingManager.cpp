
#include "BuildingManager.h"
#include "WorkersManager.h"
#include "Self.h"
#include "Log.h"
#include "Commander.h"


namespace Odalric {

	BuildingManager::BuildingManager()
	{
	}

	

	BWAPI::TilePosition BuildingManager::getBuildingPlace(const Building & building)
	{
		//building.getType()

		return _plan.getBuildSpot(building,BWAPI::Positions::None);
	}

	void BuildingManager::addFuture(ActionBuild * action, const BWAPI::TilePosition & location)
	{
		// check if the refinary spot has already a refinary
		
		Resources::getInstance().reserveMinerals(action->getMetaType().mineralPrice());
		Resources::getInstance().reserveGas(action->getMetaType().gasPrice());
		_futureBuildings.push_back(Building(action,location));

		
	}

	void BuildingManager::update()
	{
		_plan.update();

		// 1 step
		// check if the buildings are obsolets 
		validateConstructions();
		
		// 2 step
		// assign worker to build a unssigned building
		assignWorkersToBuild();

		// 3 step
		// start new constuction
		constructFutureBuildings();;

		// 4 step
		// check for started constructions
		verifyStartedConstructions();
	
		// 5 step
		// check done buildings
		verifyDoneBuildings();
	

	}

	void BuildingManager::validateConstructions()
	{
		BuildingsList::iterator it = _futureBuildings.begin();

		while (it != _futureBuildings.end())
		{
			if (it->isObsolete())
			{
				it = _futureBuildings.erase(it);
			}
			else {

				if (it->getState() != Building::State::UnderConstruction)
				{
					it++;
				}
				else
				{

					BWAPI::Unit unit = it->get();

					if (!unit || !unit->exists() ||
						unit->getHitPoints() <= 0 ||
						!unit->getType().isBuilding() ||
						it->isObsolete()
						)
					{
						it = _futureBuildings.erase(it);
					}
					else
					{


						it++;
					}
				}
			}
		}
	}

	void BuildingManager::assignWorkersToBuild()
	{
		BuildingsList::iterator it = _futureBuildings.begin();

		while (it != _futureBuildings.end())
		{
			if (it->getState() != Building::State::Unassigned)
			{
				it++;
			}
			else 
			{
				BWAPI::TilePosition buildPos = getBuildingPlace(*it);

				if (buildPos.isValid())
				{
					bool skip = false;
					it->setPosition(buildPos);

					Worker * worker = WorkersManager::getInstance().getBuilderWorker(*it);

					BWAPI::Unit workerUnit = worker->get();

					if (!worker || !workerUnit || !workerUnit->exists() || worker == nullptr)
						skip = true;

					if (!skip)
					{
	
							WorkersManager::getInstance().setBuilderWorkerJob(*worker, *it);
							it->setWorker(worker);

							_plan.reseveTiles(*it);
							it->setState(Building::State::Assigned);
						
					}
				}
	
				it++;
			}

		}
	}

	void BuildingManager::constructFutureBuildings()
	{
		for (auto & b : _futureBuildings)
		{
			if (b.getState() != Building::State::Assigned)
			{
				continue;
			}

			Worker * worker = b.getWorker();

			// worker dont exits
			if (!worker || worker == nullptr)
			{
				continue;
			}

			BWAPI::Unit workerUnit = worker->get();

			// worker died
			if (!workerUnit || !workerUnit->exists())
			{
				continue;
			}

			if (!workerUnit->isConstructing())
			{

				if (b.getSentCommandToWorker()) // something wrong
				{
					WorkersManager::getInstance().workComplete(*worker);

					b.assignWorker(nullptr);
					b.setSentCommandToWorker(false);
					b.setState(Building::State::Unassigned);
					

					// unreserve tiles
					_plan.freeTiles(b);
				}
				else {

					bool skip = false;

	
					if (b.getType().isRefinery())
					{

						BWAPI::Unitset set = BWAPI::Broodwar->getUnitsOnTile(b.getPosition());


						for each (auto u in set)
						{
							if (u->getType().isRefinery() && u->isConstructing())
							{
								skip = true;
								break;
							}
						}
					}

					if (!skip)
					{
						bool result = workerUnit->build(b.getType(), b.getPosition());

						if (!result)
						{
							//BWAPI::Error lastErr = BWAPI::Broodwar->getLastError();
							//Log::error("BuildManager: %s. Tile: %d, %d", lastErr.c_str(), b.getPosition().x, b.getPosition().y);
						}
						b.setSentCommandToWorker(result);
					}
					else {
						b.assignWorker(nullptr);
						b.setSentCommandToWorker(false);
						b.setState(Building::State::Unassigned);
						WorkersManager::getInstance().workComplete(*worker);
						b.markAsObsolte();
					}
	
					
					
					 
				


				}
			}

		}
	}

	void BuildingManager::verifyStartedConstructions()
	{
		for (const auto building : SELF.me()->getUnits())
		{

			// filter only buldings
			if (!building->getType().isBuilding() || !building->isBeingConstructed())
			{
				continue;
			}

			for (auto & futureBuilding : _futureBuildings)
			{
				if (futureBuilding.getState() != Building::State::Assigned)
				{
					continue;
				}

				/*if (!futureBuilding.getSentCommandToWorker())
				{
					continue;
				}*/

				if (futureBuilding.getPosition() == building->getTilePosition())
				{
					// unserve resources
					Resources::getInstance().unreserveMinerals(futureBuilding.getType().mineralPrice());
					Resources::getInstance().unreserveGas(futureBuilding.getType().gasPrice());

					// set this build to under construction
					futureBuilding.setState(Building::State::UnderConstruction);
					futureBuilding.setUnit(building);

					// if we are zerg, the dron has been morphed
					if (SELF.race() == BWAPI::Races::Zerg)
					{
						futureBuilding.setWorker(nullptr);
					}

					break;
				}


			}
		}

	}

	void BuildingManager::verifyDoneBuildings()
	{
		BuildingsList::iterator it = _futureBuildings.begin();

		while (it != _futureBuildings.end())
		{
			if (it->getState() == Building::State::UnderConstruction)
			{
				BWAPI::Unit unit = it->get();

				if (unit->isCompleted())
				{
					_buildingInfo.add((*it));

					Commander::getInstance().onActionDone(it->getAction());					

					// set that we are finish with worker that was working on it
					if (SELF.race() == BWAPI::Races::Terran)
					{
						WorkersManager::getInstance().workComplete(*it->getWorker());
					}

					it = _futureBuildings.erase(it);
				}
				else
					it++;


			} else
				it++;
		}
	}

}
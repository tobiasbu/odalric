
#include "ScoutManager.h"
#include "Self.h"
#include "TacticalData.h"
#include "MicroCommands.h"
#include "ResourcesAreaManager.h"
#include "TerrainAnalyzer.h"
#include "Math.h"

namespace Odarich {

	ScoutUnit::ScoutUnit(BWAPI::Unit unit, const int & pathIndex)
		:
		unit(unit),
		scoutPathIndex(pathIndex),
		toExplore(BWAPI::Positions::None)
	{

	}

	void ScoutUnit::update()
	{

	}

	ScoutManager::ScoutManager()
	{
	}

	void ScoutManager::add(BWAPI::Unit unit)
	{
		ScoutMap::iterator it = _scouts.find(unit);

		if (it == _scouts.end())
		{
			_paths.push_back(ScoutPath());
			int size = _paths.size();
			_scouts.insert(std::make_pair(unit, ScoutUnit(unit, size - 1)));
		}
	}

	void ScoutManager::release(BWAPI::Unit unit)
	{
		ScoutMap::iterator it = _scouts.find(unit);

		if (it != _scouts.end())
		{
			_scouts.erase(unit);
			
		}
	}

	void ScoutManager::remove(ScoutMap::iterator & it)
	{
		it = _scouts.erase(it);
	}

	void ScoutManager::update()
	{
		if (_scouts.empty())
		{
			return;
		}

		ScoutMap::iterator it = _scouts.begin();

		while (it != _scouts.end())
		{
			BWAPI::Unit unit = it->first;

			// check if unit has died
			if (!unit || !unit->exists() || unit->getHitPoints() <= 0 ||
				unit->getPlayer() != SELF.me()) // mind control!
			{
				remove(it);
			}
			else 
			{

				
				//EnemySet & enemies = TacticalData::getInstance().getEnemies();
				// !unit->isMoving() &&
				if (
					unit->isIdle() ||
					it->second.toExplore == BWAPI::Positions::None ||
					unit->getOrder() != BWAPI::Orders::Move
					)
				{
					//
					getNextLocationToExplore(it->second);
				}

				it++;
			}

			//var.second.update();

		}
	}

	void ScoutManager::getNextLocationToExplore(ScoutUnit & scout)
	{
		if (TacticalData::getInstance().enemiesCount() >= 1)
		{
			EnemyInfo * enemy = TacticalData::getInstance().getEnemy();

			// if we dont know where is the enemy base
			if (enemy != nullptr && enemy->baseLocation == BWAPI::TilePositions::None)
			{
			
				BWAPI::Position toExplore;
				int farDist = 0;
				//int farDistanceY = 0;

				// check if terrain analyzer is done
				if (TerrainAnalyzer::getInstance().getTaskStatus() == 5)
				{
					// check if we can get from resources area manager the farthest place
					const ResourcesAreasList & areas = ResourcesAreaManager::getInstance().getResourcesAreas();
					Base * main = BaseManager::getInstance().getMainBase();

				

					for each (const auto & area in areas)
					{
						if (area->getBase() == nullptr)
						{
							if (!BWAPI::Broodwar->isExplored(area->getBaseLocation()))
							{

								//int distx, disty;
								BWAPI::Position pos(area->getBaseLocation());

								//Math::manhattan(scout.unit->getPosition(), pos, distx, disty);
								int dist;

								//if (distx >= farDistanceX && distx >= farDistanceY)
								if (main->get())
									dist = main->get()->getDistance(pos);
								else
									dist = scout.unit->getDistance(pos);

								if (dist > farDist)
								{
									farDist = dist;
									//farDistanceY = disty;
									toExplore = pos;
								}
							}
							else
								continue;

						}
						else
							continue;
					}

					
					// move the unit 
					scout.toExplore = toExplore;
					MicroCommands::move(scout.unit, toExplore);
				}
				else // otherwise
				{
					//std::vector<BWAPI::TilePosition> & pos = TacticalData::getEnemiesPossibleLocations();
				}
			}
		}
	}
	

}
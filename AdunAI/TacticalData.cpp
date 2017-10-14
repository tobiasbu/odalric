
#include "TacticalData.h"
#include "Self.h"
#include "Assert.h"
#include "ResourcesAreaManager.h"
#include "Math.h"

namespace Odarich {


	EnemyInfo::EnemyInfo(BWAPI::Player enemy)
		:
		player(enemy)
	{
		baseLocation = BWAPI::TilePositions::None;//enemy->getStartLocation();
	}
	

	TacticalData::TacticalData()
	{

	}


	void TacticalData::init()
	{
		// create our enemies
		BWAPI::Playerset enemies = BWAPI::Broodwar->enemies();
		const auto & locations = BWAPI::Broodwar->getStartLocations();

		for each (auto & var in enemies)
		{
			_enemies.push_back(EnemyInfo(var));
		}

		// define possible enemies locations
		const BWAPI::TilePosition myStart = SELF.startPosition();
		const int locationsSize = locations.size();
		BWAPI::TilePosition diff;
		BWAPI::TilePosition minPos = getPossibleDifferenceBetweenBases(locationsSize, myStart, diff);
		BWAPI::TilePosition current = minPos;
	
		for (int i = 0; i < locationsSize; i++)
		{
			const BWAPI::TilePosition & loc = locations[i];

			if (loc != myStart)
			{
				int x = i % locationsSize;
				int y = i / locationsSize;

				current.x = minPos.x + (diff.x * x);
				current.y = minPos.y + (diff.y * y);

				if (current != myStart)
				{

					BWAPI::TilePosition pos;

					if (loc != BWAPI::TilePositions::None ||
						loc != BWAPI::TilePositions::Unknown ||
						loc != BWAPI::TilePositions::Invalid)
					{
						pos = loc;
					}
					else
						pos = current;

					_enemyBaseLocations.push_back(pos);
				}
			}
			else
				continue;
		}
	}

	BWAPI::TilePosition TacticalData::getPossibleDifferenceBetweenBases(const int & locationsSize, const BWAPI::TilePosition & myStartLocation, BWAPI::TilePosition & difference)
	{
		const int mapWidth = BWAPI::Broodwar->mapWidth();
		const int mapHeight = BWAPI::Broodwar->mapHeight();
		const int wb = mapWidth / locationsSize;
		const int hb = mapHeight / locationsSize;
		BWAPI::TilePosition pos;
		
		difference.x = mapWidth - myStartLocation.x;
		difference.y = mapHeight - myStartLocation.y;

		// check horizontal sides
		if (myStartLocation.x > wb)
			pos.x = mapWidth - difference.x;
		else
			pos.x = difference.x;
			
		// check vertical sides
		if (myStartLocation.y > hb)
			pos.y = mapHeight - difference.y;
		else
			pos.y = difference.y;
		

		return pos;
		

	}

	uint32 TacticalData::enemiesCount() const
	{
		return _enemies.size();
	}

	EnemyList & TacticalData::getEnemies()
	{
		return _enemies;
	}

	EnemyInfo * TacticalData::getEnemy()
	{
		if (_enemies.empty())
			return nullptr;
		else
			return (&_enemies[0]);
	}

	const std::vector<BWAPI::TilePosition> & TacticalData::getEnemiesPossibleLocations() const
	{
		return _enemyBaseLocations;
	}

	void TacticalData::onUnitShow(BWAPI::Unit unit)
	{
		if (unit->getType().isResourceDepot())
			baseFound(unit);
	}

	void TacticalData::baseFound(BWAPI::Unit unit)
	{
		OAI_ASSERT_MSG(unit->getType().isResourceDepot(), "TaticalData: is not Resource Depot.");

		EnemyInfo * ene = getEnemy();

		if (unit->getPlayer() == ene->player && ene->baseLocation == BWAPI::TilePositions::None)
		{
			const ResourcesAreasList & areas = ResourcesAreaManager::getInstance().getResourcesAreas();

			int distx, disty;

			for each (const auto & area in areas)
			{
				if (area->getBase() == nullptr)
				{

					Math::manhattan(area->getBaseLocation(), unit->getTilePosition(), distx, disty);
					

					if (distx <= 4 && disty <= 4)
					{
						ene->baseLocation = unit->getTilePosition();
						break;
					}
				}
				else
					continue;
			}

		}
	}

	
}
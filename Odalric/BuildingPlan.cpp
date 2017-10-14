
#include "BuildingPlan.h"
#include "BaseManager.h"
#include "Self.h"
#include "Log.h"

namespace Odalric {


	BuildingPlan::BuildingRequeriment::BuildingRequeriment(const Building & buildingToBuild)
	:
	building(buildingToBuild)
	{
		const BWAPI::UnitType & type = buildingToBuild.getType();

		int widthTiles = type.tileWidth();
		int heightTiles = type.tileHeight();

		// set margins
		
		margin = 1;
		horizontalMarginsOnly = false;

		// if we are terran
		if (SELF.race() == BWAPI::Races::Terran)
		{
			// add space for buildings that have addons
			if (type == BWAPI::UnitTypes::Terran_Command_Center ||
				type == BWAPI::UnitTypes::Terran_Factory ||
				type == BWAPI::UnitTypes::Terran_Starport ||
				type == BWAPI::UnitTypes::Terran_Science_Facility)
			{
				widthTiles += 2;
			}
			 
		}
		else if (SELF.race() == BWAPI::Races::Protoss)
		{
			// Try to pack protoss buildings more closely together. Space can run out.
			
			if (type == BWAPI::UnitTypes::Protoss_Pylon)
			{
				int numPylons = 0;

				if (numPylons < 3)
				{
					// Early pylons may be spaced differently than other buildings.
					margin = 3; //Config::Macro::PylonSpacing;
				}
				else
				{
					// Building spacing == 1 is usual. Be more generous with pylons.
					margin = 2;
				}
			}

			if (type == BWAPI::UnitTypes::Protoss_Gateway ||
				type == BWAPI::UnitTypes::Protoss_Forge ||
				type == BWAPI::UnitTypes::Protoss_Stargate ||
				type == BWAPI::UnitTypes::Protoss_Citadel_of_Adun ||
				type == BWAPI::UnitTypes::Protoss_Templar_Archives ||
				type == BWAPI::UnitTypes::Protoss_Gateway)
			{
				horizontalMarginsOnly = true;
			}
		}

		if (type == BWAPI::UnitTypes::Terran_Bunker ||
			type == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
			type == BWAPI::UnitTypes::Zerg_Creep_Colony)
		{
			// Pack defenses tightly together.
			margin = 0;
		}



		size.x = widthTiles;
		size.y = heightTiles;
	}

	BuildingPlan::BuildingPlan()
	{
		// initializr reserved tiles for constructions
		_reservedTiles.reserve(BWAPI::Broodwar->mapWidth() * BWAPI::Broodwar->mapHeight());
		std::fill(_reservedTiles.begin(), _reservedTiles.end(), BuildableTile());
		_tilesMapReady = false;
	}

	void BuildingPlan::update()
	{
		if (!_tilesMapReady) {
			if (TerrainAnalyzer::getInstance().isTileMapReady())
			{

				const TileMap & tileMap = TerrainAnalyzer::getInstance().getTileMap();
				int size = tileMap.size();

				for (int i = 0; i < size; i++)
				{
					_reservedTiles[i].parentTile = &tileMap[i];
					//_reservedTiles[i].reserved = tileMap[i].buildable;

					
					
				}

				_tilesMapReady = true;
			}
		}
	}

	BWAPI::TilePosition BuildingPlan::getBuildSpot(const Building & building, const BWAPI::Position & startPosition)
	{
		//BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());

		if (building.getType().isRefinery())
		{
			Base * base = BaseManager::getInstance().getClosest(startPosition);

			const BWAPI::Unitset & geysers = base->getResources()->getGas();
			int possibleRefinaries = geysers.size();

			if (possibleRefinaries >= 1)
			{
				for each (const auto & gas in geysers)
				{
					if (!gas->getType().isRefinery() || !gas->isConstructing())
					{
						return gas->getTilePosition();
					}
				}
				
			}

			return BWAPI::TilePositions::None;
			
		}
		else {

			if (_tilesMapReady)
			{
				// general buildings
				Base * base = BaseManager::getInstance().getClosest(startPosition);
				return findBuildSpot(building, BWAPI::TilePositions::None, base);
			}
			else
			{
				return BWAPI::TilePositions::None;
			}

		}

		
	}

	BWAPI::TilePosition BuildingPlan::findBuildSpot(const Building & buildingToBuild, BWAPI::TilePosition startPosition, Base * nearestBase)
	{
		

		//int margin = 0; 
		//bool horizontalMargin = false;
		//Vector2i buildSize = getBuildSize(buildingToBuild, margin, horizontalMargin);

		ResourcesArea * resources = nullptr;

		if (nearestBase != nullptr)
		{
			resources = nearestBase->getResources();

			Bounds r = resources->getBounds();
			Vector2i center = r.centroid();
			startPosition.x = center.x / 32;
			startPosition.y = center.y / 32;
		}
		else {

		}

		BuildingRequeriment req(buildingToBuild);
		bool found = false;
		BWAPI::TilePosition resultTile;
		Vector2i start;
		Vector2i end;
		int stepw = req.size.x / 2;

		if (stepw <= 0)
			stepw = 1;

		int steph = req.size.y / 2;

		if (steph <= 0)
			steph = 1;

		//Log::info("SIZE: %d %d", req.size.x, req.size.y);
		//Log::info("TYPE: %s", req.building.getType().toString().c_str());

		while (!found)
		{
			// check top side
			start.x = startPosition.x - stepw;
			start.y = startPosition.y - steph;
			end.x = startPosition.x + stepw;
			end.y = startPosition.y - steph;

			if (checkSpot(req, start, end, resultTile))
			{
				return resultTile;
				found = true;
				break;
			}

			// check bottom side
			start.x = startPosition.x - stepw;
			start.y = startPosition.y + steph;
			end.x = startPosition.x + stepw;
			end.y = startPosition.y + steph;

			if (checkSpot(req, start, end, resultTile))
			{
				return resultTile;
				found = true;
				break;
			}

			// check left side
			start.x = startPosition.x - stepw;
			start.y = startPosition.y - steph;
			end.x = startPosition.x - stepw;
			end.y = startPosition.y + steph;

			if (checkSpot(req, start, end, resultTile))
			{
				return resultTile;
				found = true;
				break;
			}

			// check right side
			start.x = startPosition.x + stepw;
			start.y = startPosition.y - steph;
			end.x = startPosition.x + stepw;
			end.y = startPosition.y + steph;

			if (checkSpot(req, start, end, resultTile))
			{
				return resultTile;
				found = true;
				break;
			}

			stepw += req.size.x / 2;
			steph += req.size.y / 2;

			if (stepw > _maxSpotRange || steph > _maxSpotRange) 
			{
				found = true;
				break;
			}
		}

		return resultTile;
	}

	bool BuildingPlan::checkSpot(const BuildingRequeriment & requirement, const Vector2i & start, const Vector2i & end, BWAPI::TilePosition & resultTile)
	{

		

		Bounds bounds;
		Vector2i pos = start;

		int stepx = end.x - start.x;

		if (stepx != 0) 
			stepx = 1;

		int stepy = end.y - start.y;

		if (stepy != 0)
			stepy = 1;


		

		while (true) {

			//Log::info("Finding: pos { %d, %d } end { %d, %d }", pos.x, pos.y, end.x, end.y);

			// update bounds
			bounds.set(
				pos.x - requirement.margin,
				pos.y - requirement.margin,
				pos.x + requirement.size.x + requirement.margin,
				pos.y + requirement.size.y + requirement.margin
				);

			if (requirement.horizontalMarginsOnly)
			{
				bounds[0].y += requirement.margin;
				bounds[1].y -= requirement.margin;
			}

			/*
			if (bounds[0].x >= 0 &&
			bounds[0].x < TerrainAnalyzer::getInstance().getMapWidth() - bounds[1].x - 1 &&
			bounds[0].y >= 0 &&
			bounds[0].y < TerrainAnalyzer::getInstance().getMapHeight() - bounds[1].y - 1)
		{
			
			*/
			
			// check if we can build
				if (canBuild(bounds))
				{
					resultTile.x = bounds[0].x;
					resultTile.y = bounds[0].y;
					return true;
					break;
				}

			pos.x += stepx;
			pos.y += stepy;

			

			if ((pos.x >= end.x && pos.y >= end.y)) 
			{
				return false;
				break;
			}

			/*if (type.isAddon())
			{
				// get the who is owner of this addon
				const BWAPI::UnitType & owner = type.whatBuilds().first;
				BWAPI::TilePosition ownerTile(position.x - owner.tileWidth(), position.y + 2 - owner.tileHeight());
				bounds[0].x = ownerTile.x - margin;
				bounds[0].y = ownerTile.y - margin;
				bounds[1].x = ownerTile.x + widthTiles + margin;
				bounds[1].y = ownerTile.y + heightTiles + margin;

			}
			else
			{
				
			}*/

			


		}

		return false;
	}

	Vector2i BuildingPlan::getBuildSize(const Building & building, int & margin, bool & horizontalMargins)
	{

		const BWAPI::UnitType & type = building.getType();

		int widthTiles = building.getType().tileWidth();
		int heightTiles = building.getType().tileHeight();

		// set margins
		Vector2i size;
		margin = 1;
		horizontalMargins = false;

		// if we are terran
		if (SELF.race() == BWAPI::Races::Terran)
		{
			// add space for buildings that have addons
			if (type == BWAPI::UnitTypes::Terran_Command_Center ||
				type == BWAPI::UnitTypes::Terran_Factory ||
				type == BWAPI::UnitTypes::Terran_Starport ||
				type == BWAPI::UnitTypes::Terran_Science_Facility)
			{
				widthTiles += 2;
			}
		}
		else if (SELF.race() == BWAPI::Races::Protoss)
		{
			// Try to pack protoss buildings more closely together. Space can run out.
			if (type == BWAPI::UnitTypes::Terran_Bunker ||
				type == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
				type == BWAPI::UnitTypes::Zerg_Creep_Colony)
			{
				// Pack defenses tightly together.
				margin = 0;
			}
			else if (type == BWAPI::UnitTypes::Protoss_Pylon)
			{
				int numPylons = 0;

				if (numPylons < 3)
				{
					// Early pylons may be spaced differently than other buildings.
					margin = 3; //Config::Macro::PylonSpacing;
				}
				else
				{
					// Building spacing == 1 is usual. Be more generous with pylons.
					margin = 2;
				}
			}

			if (type == BWAPI::UnitTypes::Protoss_Gateway ||
				type == BWAPI::UnitTypes::Protoss_Forge ||
				type == BWAPI::UnitTypes::Protoss_Stargate ||
				type == BWAPI::UnitTypes::Protoss_Citadel_of_Adun ||
				type == BWAPI::UnitTypes::Protoss_Templar_Archives ||
				type == BWAPI::UnitTypes::Protoss_Gateway)
			{
				horizontalMargins = true;
			}
		}


		size.x = widthTiles;
		size.y = heightTiles;

		return size;
	}

	bool BuildingPlan::canBuild(const Bounds & bounds) const
	{
		// check if this bounds is out of map
		if (bounds[0].x < 0 || bounds[0].y < 0 ||
			bounds[1].x >= BWAPI::Broodwar->mapWidth() ||
			bounds[1].x < bounds[0].x + bounds.getSize().x ||
			bounds[1].y >= BWAPI::Broodwar->mapHeight())
		{
			return false;
		}

		

			// check if overlaps bases
			if (boundsOverlapsBases(bounds, true))
			{
				return false;
			}

			// and to finish, check the reserved map and if there is buildable
			int index = 0;

			for (int y = bounds[0].y; y < bounds[1].y; y++)
			{
				for (int x = bounds[0].x; x < bounds[1].x; x++)
				{
			
					index = x  * TerrainAnalyzer::getInstance().getMapHeight() + y;

					//if (!b.type.isRefinery())
					//{

					if (_reservedTiles[index].parentTile) 
					{
						if (!_reservedTiles[index].parentTile->buildable)
						{
							return false;
						}
					}

					if (_reservedTiles[index].reserved ||
						!isBuildable(x, y)) /*||
						hasUnits(x, y, building.getWorker()->get()) ||
						((type != BWAPI::UnitTypes::Protoss_Photon_Cannon)))*/
					{
						return false;
					}
					else if (SELF.race() == BWAPI::Races::Zerg)
					{
						if (!BWAPI::Broodwar->hasCreep(x, y))
						{
							return false;
						}
					}
					//}
				}
			}

			return true;

		}
		
	bool BuildingPlan::canBuild(const Building & building, const BWAPI::TilePosition & position) const
	{
		// first check by bwapi
		if (!BWAPI::Broodwar->canBuildHere(position, building.getType(), building.getWorker()->get()))
		{
			return false;
		}

		// check if overlaps resouces areas
		/*if (overlapsBases(building, position))
		{
			return false;
		}*/

		// check the reserved map of tiles
		int maxW = position.x + building.getType().tileWidth();
		int maxH = position.y + building.getType().tileHeight();
		int index = 0;

		for (int x = position.x; x < maxW; x++)
		{
			for (int y = position.y; y < maxH; y++)
			{
				index = x  * TerrainAnalyzer::getInstance().getMapHeight() + y;

				

				if (_reservedTiles[index].reserved)
				{
					return false;
				}
				else if (!_reservedTiles[index].parentTile->buildable)
				{
					return false;
				}
			}
		}
	}

	bool BuildingPlan::margiedCanBuild(const Building & building, const BWAPI::TilePosition & position, const int & margin, const bool & horizontalMargins) const
	{
		// check if we can build here
		if (!canBuild(building, position))
		{
			return false;
		}

		int widthTiles = position.x + building.getType().tileWidth();
		int heightTiles = position.x + building.getType().tileHeight();

		const BWAPI::UnitType & type = building.getType();

		// if we are terran
		if (SELF.race() == BWAPI::Races::Terran)
		{
			// add space for buildings that have addons
			if (type == BWAPI::UnitTypes::Terran_Command_Center ||
				type == BWAPI::UnitTypes::Terran_Factory ||
				type == BWAPI::UnitTypes::Terran_Starport ||
				type == BWAPI::UnitTypes::Terran_Science_Facility)
			{
				widthTiles += 2;
			}
		}

		// create bounds
		Bounds bounds;
		//int boundsx, boundsy, boundsw, boundsh;
		/*= position.x - margin;
		int boundsy = position.y - margin;
		int boundsw = position.x + widthTiles + margin;
		int boundsh = position.y + heightTiles + margin;*/

		// if is addon
		if (type.isAddon())
		{
			// get the who is owner of this addon
			const BWAPI::UnitType & owner = type.whatBuilds().first;
			BWAPI::TilePosition ownerTile(position.x - owner.tileWidth(), position.y + 2 - owner.tileHeight());
			bounds[0].x = ownerTile.x - margin;
			bounds[0].y = ownerTile.y - margin;
			bounds[1].x = ownerTile.x + widthTiles + margin;
			bounds[1].y = ownerTile.y + heightTiles + margin;

		}
		else 
		{
			bounds.set(
				position.x - margin,
				position.y - margin,
				position.x + widthTiles + margin,
				position.y + heightTiles + margin
				);
			/*boundsx = position.x - margin;
			boundsy = position.y - margin;
			boundsw = position.x + widthTiles + margin;
			boundsh = position.y + heightTiles + margin;*/
		}

		if (horizontalMargins)
		{
			bounds[0].y += margin;
			bounds[1].y -= margin;
		}

		// now we check if this not intersects the map
		if (bounds[0].x < 0 || bounds[0].y < 0 ||
			bounds[1].x > BWAPI::Broodwar->mapWidth() ||
			bounds[1].x < position.x + widthTiles ||
			bounds[1].y > BWAPI::Broodwar->mapHeight())
		{
			return false;
		}


		// check if overlaps bases
		if (boundsOverlapsBases(bounds, true))
		{
			return false;
		}

		// and to finish, check the reserved map
		int index = 0;

		for (int x = bounds[0].x; x < bounds[1].x; x++)
		{
			for (int y = bounds[0].y; y < bounds[1].y; y++)
			{
				index = x  * TerrainAnalyzer::getInstance().getMapHeight() + y;

				//if (!b.type.isRefinery())
				//{
				if (_reservedTiles[index].reserved ||
					!_reservedTiles[index].parentTile->buildable ||
					isBuildable(x,y) ||
					hasUnits(x,y,building.getWorker()->get()) ||
					((type != BWAPI::UnitTypes::Protoss_Photon_Cannon)))
				{
					return false;
				}
				else if (SELF.race() == BWAPI::Races::Zerg)
				{
					if (!BWAPI::Broodwar->hasCreep(x, y))
					{
						return false;
					}
				}
				//}
			}
		}

		return true;
	}

	bool BuildingPlan::overlapsBases(const Building & building, const BWAPI::TilePosition & position) const
	{
		BasesMap & bases = BaseManager::getInstance().getBases();
		Bounds buildingBounds(building.getType(), position);

		for each (auto & base in bases)
		{
			ResourcesArea * resources = base.second.getResources();

			if (resources != nullptr)
			{
				if (resources->getBounds().intersects(buildingBounds))
				{
					return true;
				}
			}
			else 
			{
				const Bounds & baseBounds(base.first);

				if (baseBounds.intersects(buildingBounds))
				{
					return true;
				}
			}
			

		}

		return false;
	}

	bool BuildingPlan::boundsOverlapsBases(const Bounds & bounds, const bool & convertTileToMap) const
	{
		BasesMap & bases = BaseManager::getInstance().getBases();
		
		Bounds b(bounds);

		if (convertTileToMap)
		{
			b[0] *= 32;
			b[1] *= 32;
		}

		for each (auto & base in bases)
		{
			ResourcesArea * resources = base.second.getResources();

			if (resources != nullptr)
			{
				if (resources->getBounds().intersects(b))
				{
					return true;
				}
			}
			else
			{
				const Bounds & baseBounds(base.first);

				if (baseBounds.intersects(b))
				{
					return true;
				}
			}


		}

		return false;
	}

	bool BuildingPlan::isBuildable(const int & x, const int & y) const
	{
		BWAPI::TilePosition tp(x, y);

		if (!tp.isValid())
		{
			return false;
		}

		if (!BWAPI::Broodwar->isBuildable(x, y, true))
		{
			// Unbuildable according to the map, or because the location is blocked
			// by a visible building. Unseen buildings (even if known) are "buildable" on.
			return false;
		}

		if (SELF.race() == BWAPI::Races::Terran)
		{
			if (blocksAddon(x,y))
			{
				return false;
			}
		}
		

		return true;
	}

	bool BuildingPlan::hasUnits(const int & x, const int & y, BWAPI::Unit except) const
	{
		for (auto & unit : BWAPI::Broodwar->getUnitsOnTile(x, y))
		{
			if ((except != nullptr) && (unit != except))
			{
				return false;
			}
		}

		return true;
	}

	bool BuildingPlan::blocksAddon(const int & x, const int & y) const
	{
		for (int i = 0; i <= 2; ++i)
		{
			if (x - i < 0)
			{
				return true;
			}

			for (auto unit : BWAPI::Broodwar->getUnitsOnTile(x - i, y))
			{
				if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center ||
					unit->getType() == BWAPI::UnitTypes::Terran_Factory ||
					unit->getType() == BWAPI::UnitTypes::Terran_Starport ||
					unit->getType() == BWAPI::UnitTypes::Terran_Science_Facility)
				{
					return true;
				}
			}
		}

		return false;
	}

	void BuildingPlan::reseveTiles(const Building & building)
	{

		BWAPI::TilePosition start = building.getPosition();
		BWAPI::TilePosition end = start;
		end.x += building.getType().tileWidth();
		end.y += building.getType().tileHeight();

		int index = 0;

		for (int y = start.y; y < end.y; y++)
		{
			for (int x = start.x; x < end.x; x++)
			{
				index = x  * TerrainAnalyzer::getInstance().getMapHeight() + y;
				_reservedTiles[index].reserved = true;
			}
		}
	}

	void BuildingPlan::freeTiles(const Building & building)
	{
		BWAPI::TilePosition start = building.getPosition();
		BWAPI::TilePosition end = start;
		end.x += building.getType().tileWidth();
		end.y += building.getType().tileHeight();

		int index = 0;

		for (int y = start.y; y < end.y; y++)
		{
			for (int x = start.x; x < end.x; x++)
			{
				index = x  * TerrainAnalyzer::getInstance().getMapHeight() + y;
				_reservedTiles[index].reserved = false;
			}
		}
	}
}


#ifndef _OAI_BUILDINGPLAN_H_
#define _OAI_BUILDINGPLAN_H_

#include "Bounds.h"
#include "Building.h"
#include "BaseManager.h"
#include "TerrainAnalyzer.h"

namespace Odalric {

	struct BuildableTile
	{
		bool reserved;
		const Tile * parentTile;

		BuildableTile()
			:
			reserved(false),
			parentTile(nullptr)
		{

		}
	};



	class BuildingPlan
	{
	private:

		struct BuildingRequeriment
		{
			// build margin in tile
			int margin;
			// consider only horizontal margins
			bool horizontalMarginsOnly;
			// building size in tiles
			Vector2i size;
			// the building it self
			const Building & building;

			BuildingRequeriment(const Building & buildingToBuild);
		};
		
		int _maxSpotRange = 80;
		bool _tilesMapReady = false;
		
		std::vector<BuildableTile> _reservedTiles;


		Vector2i getBuildSize(const Building & building, int & margin, bool & horizontalMargins);
		BWAPI::TilePosition findBuildSpot(const Building & buildingToBuild, BWAPI::TilePosition startPosition, Base * nearestBase = nullptr);
		bool checkSpot(const BuildingRequeriment & requirement, const Vector2i & start, const Vector2i & end, BWAPI::TilePosition & resultTile);

	public:

		BuildingPlan();

		void update();

		bool canBuild(const Building & building, const BWAPI::TilePosition & position) const;
		bool canBuild(const Bounds & bounds) const;
		bool margiedCanBuild(const Building & building, const BWAPI::TilePosition & position, const int & margin, const bool & horizontalMargins) const;
		bool overlapsBases(const Building & building, const BWAPI::TilePosition & position) const;
		bool boundsOverlapsBases(const Bounds & bounds, const bool & convertTileToMap) const;
		bool isBuildable(const int & x, const int & y) const;
		bool hasUnits(const int & x, const int & y, BWAPI::Unit except) const;
		bool blocksAddon(const int & x, const int & y) const;

		void reseveTiles(const Building & building);
		void freeTiles(const Building & building);
		BWAPI::TilePosition getBuildSpot(const Building & building, const BWAPI::Position & startPosition);

	};

}

#endif
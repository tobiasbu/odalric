
#include "Draw.h"


namespace Odarich {

	namespace Draw
	{
		// draw normal bounds in map
		void bounds(const Bounds & bounds, const BWAPI::Color & color, const bool & solid )
		{
			BWAPI::Broodwar->drawBoxMap(
				bounds.getMin().x, bounds.getMin().y,
				bounds.getMax().x, bounds.getMax().y,
				color, solid);
		}

		// draw tiled bounds. convert to map position
		void tiledBounds(const Bounds & bounds, const BWAPI::Color & color, const bool & solid)
		{
			BWAPI::Broodwar->drawBoxMap(
				bounds.getMin().x * 32, bounds.getMin().y * 32,
				bounds.getMax().x * 32, bounds.getMax().y * 32,
				color, solid);
		}

		// draw tile on map
		void tile(const int & x, const int & y, const BWAPI::Color & color, const bool & solid)
		{
			BWAPI::Broodwar->drawBoxMap(
				x * 32, y * 32,
				(x * 32) + 32, (y * 32) + 32,
				color, solid);
		}
		
		void tile4x3(const BWAPI::TilePosition & tile, const BWAPI::Color & color, const bool & solid)
		{
			BWAPI::Broodwar->drawBoxMap(
				tile.x * 32, tile.y * 32,
				(tile.x * 32) + 128, (tile.y * 32) + 96,
				color, solid);
		}

		


	}

}
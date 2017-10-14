
#ifndef _OAI_DRAW_H_
#define _OAI_DRAW_H_

#include "Bounds.h"
#include <BWAPI.h>

namespace Odarich {

	namespace Draw
	{
		// draw normal bounds in map
		void bounds(const Bounds & bounds, const BWAPI::Color & color, const bool & solid = false);

		// draw tiled bounds. convert to map position
		void tiledBounds(const Bounds & bounds, const BWAPI::Color & color, const bool & solid = false);

		// draw tile on map
		void tile(const int & x, const int & y, const BWAPI::Color & color, const bool & solid = false);

		// draw 4x3 area in tiles (used for resourcesDepot)
		void tile4x3(const BWAPI::TilePosition & tile, const BWAPI::Color & color, const bool & solid = false);
	}

}

#endif
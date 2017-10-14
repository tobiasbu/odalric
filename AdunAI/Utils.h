
#ifndef _OAI_UTILS_HPP_
#define _OAI_UTILS_HPP_

#include <BWAPI.h>
#include "Rect.h"
#include "Bounds.h"

namespace Odalric {

	namespace Utils
	{
		BWAPI::Unit getNearestUnit(const BWAPI::PositionOrUnit & from, const BWAPI::UnitType & type, const BWAPI::Unitset & unitSets, const int & maxRadius = 100000);
		bool isClose(const int & x0, const int & y0, const int & x1, const int & y1, const int & radius = 100000);
		bool isClose(const BWAPI::PositionOrUnit & from, const BWAPI::PositionOrUnit & to, const int & maxRadius = 100000);
		bool isClose(const Bounds & from, const Bounds & to, const int & maxRadius = 100000);
		bool isClose(const BWAPI::Position & from, const BWAPI::Position & to, const int & maxRadius = 100000);
		
		bool isInRadius(const int & distx, const int & disty, const int & radius);
		
		BWAPI::Position unitMidPoint(const BWAPI::Unit unit);
		Rect unitRect(BWAPI::Unit unit);
	}

}

#endif
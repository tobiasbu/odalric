
#include "Utils.h"
#include "Math.h"
#include "Log.h"

namespace Odarich {

	BWAPI::Unit Utils::getNearestUnit(const BWAPI::PositionOrUnit & from, const BWAPI::UnitType & type, const BWAPI::Unitset & unitSets, const int & maxRadius)
	{
		int closeDistx = maxRadius, closeDisty = maxRadius;
		int distx, disty;
		BWAPI::Unit _unit = nullptr;

		for (auto & unit : unitSets)
		{
			if (!unit)
				continue;

			if (type != unit->getType())
				continue;


			distx = 0, disty = 0;

			Math::manhattan(
				unit->getPosition(),
				from.getPosition(),
				distx, disty
				);

			if (distx < closeDistx && disty < closeDisty)
			{
				if (unit->getType().isMineralField() && unit->getResources() <= 0)
					continue;

				closeDistx = distx;
				closeDisty = disty;
				_unit = unit;
			}


		}

		return _unit;
	}

	bool Utils::isClose(const int & x0, const int & y0, const int & x1, const int & y1, const int & radius)
	{
		int distx = radius, disty = radius;
		Math::manhattan(x0,y0,x1,y1, distx, disty);

		if (distx < radius && disty < radius)
			return true;
		else 
			return false;
		
	}

	bool Utils::isClose(const BWAPI::PositionOrUnit & from, const BWAPI::PositionOrUnit & to, const int & maxRadius)
	{
		return isClose(from.getPosition(), to.getPosition(), maxRadius);
	}

	bool Utils::isClose(const Bounds & from, const Bounds & to, const int & maxRadius)
	{
		if (from.intersects(to))
			return true;
		else {

			//int distx = maxRadius, disty = maxRadius;

			//const Vector2i & sizeFrom = from.getSize();
			//const Vector2i & sizeTo = to.getSize();

			// *--* *--*
			//  x2b < x1
			bool left = to[1].x < from[0].x; 

			// x1b < x2
			bool right = from[1].x < to[0].x; 

			// y2b < y1
			bool bottom = to[1].y < from[0].y;

			//  y1b < y2
			bool top = from[1].y < to[0].y;

			if (top && left) // ((x1, y1b), (x2b, y2))
				return isClose(from[0].x, from[1].y, to[1].x, to[0].y, maxRadius);

			else if (left && bottom) // ((x1, y1), (x2b, y2b))
				return isClose(from[0].x, from[0].y, to[1].x, to[1].y, maxRadius);

			else if (bottom && right) // ((x1b, y1), (x2, y2b))
				return isClose(from[1].x, from[0].y, to[0].x, to[1].y, maxRadius);

			else if (right && top) // ((x1b, y1b), (x2, y2))
				return isClose(from[1].x, from[1].y, to[0].x, to[0].y, maxRadius);

			else if (left) // x1 - x2b
				return (Math::abs(from[0].x - to[1].x) < maxRadius) ? true : false;

			else if (right) // x2 - x1b
				return (Math::abs(to[0].x - from[1].x) < maxRadius) ? true : false;

			else if (bottom) //  y1 - y2b
				return (Math::abs(from[0].y - to[1].y) < maxRadius) ? true : false;

			else if (top) //  y2 - y1b
				return (Math::abs(to[0].y - from[1].y) < maxRadius) ? true : false;

			else  // rectangles intersect
				return true;

		}
			// min, min
			/*Math::manhattan(from[0], to[0], distx, disty);


			// a[-] b[-]
			if (from[0].x < to[0].x)
			{
				distx -= sizeFrom.x;
			}
			else if (from[0].x > to[0].x) // b[-] a[-]
			{
				distx -= sizeTo.x;
			}

			if (from[0].y < to[0].y)
			{
				disty -= sizeFrom.y;
			}
			else if (from[0].y > to[0].y) // b[-] a[-]
			{
				disty -= sizeTo.y;
			}

			minMin = isInRadius(distx, disty, maxRadius);


			// max, max
			Math::manhattan(from[1], to[1], distx, disty);

			if (from[1].x < to[1].x)
			{
				distx -= sizeTo.x;
			}
			else if (from[1].x > to[1].x) // b[-] a[-]
			{
				distx -= sizeFrom.x;
			}

			if (from[1].y < to[1].y)
			{
				disty -= sizeTo.y;
			}
			else if (from[1].y > to[1].y) // b[-] a[-]
			{
				disty -= sizeFrom.y;
			}

			maxMax = isInRadius(distx, disty, maxRadius);

			// max, min
			Math::manhattan(from[0], to[1], distx, disty);
			if (from[0].x < to[1].x)
			{
				distx -= (sizeTo.x + sizeFrom.x);
			}

			if (from[0].y < to[1].y)
			{
				disty -= (sizeTo.y + sizeFrom.y);
			}

			minMax = isInRadius(distx, disty, maxRadius);*/

		
		
	}

	bool Utils::isInRadius(const int & distx, const int & disty, const int & radius)
	{
		if (distx < radius && disty < radius)
			return true;
		else
			return false;
		
	}

	bool Utils::isClose(const BWAPI::Position & from, const BWAPI::Position & to, const int & maxRadius)
	{
		int distx = maxRadius, disty = maxRadius;

		Math::manhattan(from, to, distx, disty);

		if (distx < maxRadius && disty < maxRadius)
		{
			return true;
		}
		else {
			return false;
		}
	}

	BWAPI::Position Utils::unitMidPoint(const BWAPI::Unit unit)
	{
		return BWAPI::Position(
			unit->getLeft() + ((unit->getRight() - unit->getLeft()) / 2),
			unit->getTop() + ((unit->getBottom() - unit->getTop()) / 2));
	}





	Rect Utils::unitRect(BWAPI::Unit unit)
	{
		return Rect(unit->getLeft(), unit->getTop(), unit->getLeft() - unit->getRight(), unit->getTop() - unit->getBottom());
	}

}
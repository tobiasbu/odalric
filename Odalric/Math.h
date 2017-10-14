
#ifndef _OAI_MATH_HPP_
#define _OAI_MATH_HPP_

#include <BWAPI.h>
#include "Bounds.h"
#include "Vector2.h"
#include "Types.h"

namespace Odalric {

	namespace Math
	{
		int abs(const int & value);

		// manhattan distance
		void manhattan(const int & x0, const int & y0, const int & x1, const int & y1, int & distx, int & disty, const bool & absoulute = true);

		// manhattan distance
		void manhattan(const Bounds & a, const Bounds & b, int & distx, int & disty, const bool & absoulute = true);

		// manhattan distance
		void manhattan(const BWAPI::Position & from, const BWAPI::Position & to, int & distx, int & disty);

		// manhattan distance
		void manhattan(const Vector2i & from, const Vector2i & to, int & distx, int & disty);

		int absDistance(const int & x0, const int & y0, const int & x1, const int & y1);

		// eucliden distance
		int distance(const int & x0, const int & y0, const int & x1, const int & y1);

		// rounded eucliden distance
		int roundedDistance(const int & x0, const int & y0, const int & x1, const int & y1);

		// rounded eucliden distance
		int roundedDistance(const Vector2i & from, const Vector2i & to);

		int square(const int & x0, const int & y0);

		// sqrt
		float norm(const int & x0, const int & y0);

		// distance between point and bounds
		int pointDistance(const int & x, const int & y, const Bounds & bounds);
		
	
		const int & max(const int & left, const int & right);
		const int8 & maxIndex(const int & left, const int & right);
		const int & min(const int & left, const int & right);
	}

}

#endif
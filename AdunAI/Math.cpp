

#include "Math.h"
#include <limits.h>

namespace Odarich {

	namespace Math
	{
		int abs(const int & value)
		{

			
			///int const mask = value >> sizeof(int) * CHAR_BIT - 1;

			//return (value + mask) ^ mask;

			return ((value<0) ? (-value) : (value));
		}

		void manhattan(const int & x0, const int & y0, const int & x1, const int & y1, int & distx, int & disty, const bool & absoulute)
		{
			if (absoulute)
			{
				distx = abs(x1 - x0);
				disty = abs(y1 - y0);
			}
			else
			{
				distx = x1 - x0;
				disty = y1 - y0;
			}

		}

		inline int square(const int & x0, const int & y0)
		{
			return x0*x0 + y0*y0;
		}

		// sqrt
		inline float norm(const int & x0, const int & y0)
		{
			return sqrtf(square(x0, y0));
		}

		// rounded eucliden distance
		int roundedDistance(const int & x0, const int & y0, const int & x1, const int & y1)
		{
			return int(0.5 + norm(x1 - x0, y1 - y0));
		}

		int roundedDistance(const Vector2i & from, const Vector2i & to)
		{
			return int(0.5 + norm(from.x - to.x, from.y - to.y));
		}

		int pointDistance(const int & x, const int & y, const Bounds & bounds)
		{

			if (x >= bounds[0].x) // topleft x
			{
				if (x <= bounds[1].x) // bottomright x
				{
					if (y > bounds[1].y)	// bottomright y
						return y - bounds[1].y;// y - bottoright.y										// S
					else if (y < bounds[0].y)	 // topleft.y
						return bounds[0].y - y; // topleft.y - y										// N
					else
						return 0;															// intersects
				}
				else
				{
					if (y > bounds[1].y) // bottomright y
						return roundedDistance(x, y, bounds[1].x, bounds[1].y);		// bottomRight							// SE
					else if (y < bounds[0].y)	// topleft y
						return roundedDistance(x, y, bounds[1].x, bounds[0].y); // bottomRight.x, topLeft.y	// NE
					else
						return x - bounds[1].x; // x - bottomright.x									// E
				}
			}
			else
			{
				if (y > bounds[1].y)	// bottomRight.y	
					return roundedDistance(x, y, bounds[0].x, bounds[1].y); // topLeft.x, bottomRight.y	// SW
				else if (y < bounds[0].y)	// topleft.y	
					return roundedDistance(x, y, bounds[0].x, bounds[0].y);										// NW
				else
					return bounds[0].x - x; // topleft - x
			}
		}

		void manhattan(const Bounds & a, const Bounds & b, int & distx, int & disty, const bool & absoulute)
		{

				// *--* *--*
				//  x2b < x1
				bool left = b[1].x < a[0].x;

				// x1b < x2
				bool right = a[1].x < b[0].x;

				// y2b < y1
				bool bottom = b[1].y < a[0].y;

				//  y1b < y2
				bool top = a[1].y < b[0].y;

				distx = 0;
				disty = 0;


				if (top && left) // ((x1, y1b), (x2b, y2))
					return manhattan(a[0].x, a[1].y, b[1].x, b[0].y, distx, disty, absoulute);

				else if (left && bottom) // ((x1, y1), (x2b, y2b))
					return manhattan(a[0].x, a[0].y, b[1].x, b[1].y, distx, disty, absoulute);

				else if (bottom && right) // ((x1b, y1), (x2, y2b))
					return manhattan(a[1].x, a[0].y, b[0].x, b[1].y, distx, disty, absoulute);

				else if (right && top) // ((x1b, y1b), (x2, y2))
					return manhattan(a[1].x, a[1].y, b[0].x, b[0].y, distx, disty, absoulute);

				if (left) {
					// x1 - x2b
					distx = a[0].x - b[1].x;
					
				}

				if (right)	{
					// x2 - x1b
					distx = b[0].x - a[1].x;
					//disty = 0;
				}
				if (bottom) {
					//  y1 - y2b
					disty = a[0].y - b[1].y;
					
				}
				if (top) {
					//  y2 - y1b
					disty = b[0].y - a[1].y;
					//distx = 0;
				}
				/*else {
					// rectangles intersect
					distx = 0;
					disty = 0;
				}*/

				if (absoulute)
				{
					distx = Math::abs(distx);
					disty = Math::abs(disty);
				}
		}

		void manhattan(const BWAPI::Position & from, const BWAPI::Position & to, int & distx, int & disty)
		{
			distx = abs(to.x - from.x);
			disty = abs(to.y - from.y);
		}

		void manhattan(const Vector2i & from, const Vector2i & to, int & distx, int & disty)
		{
			distx = abs(to.x - from.x);
			disty = abs(to.y - from.y);
		}

		const int & max(const int & a, const int & b)
		{
			return (a < b) ? b : a;
		}

		const int8 & maxIndex(const int & a, const int & b)
		{
			return (a < b) ? 0 : 1;
		}

		const int & min(const int & a, const int & b)
		{
			return !(b < a) ? a : b;
		}
	}

}
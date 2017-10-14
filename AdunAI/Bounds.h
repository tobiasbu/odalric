
#ifndef _OAI_BOUNDS_H_
#define _OAI_BOUNDS_H_

#include "Vector2.h"
#include "Types.h"
#include "Assert.h"
#include <BWAPI.h>

namespace Odarich {

	// AABB - bounding box
	struct Bounds {

	private:

		Vector2i min, max;


		//Vector2i size;
		//Vector2i center;
		//Rect rect;

	public:

		Bounds();
		Bounds(const Bounds & bounds);
		Bounds(const BWAPI::Unit & unit);
		Bounds(const BWAPI::UnitType & unit, const BWAPI::TilePosition & tile);
		Bounds(const Vector2i & pointA, const Vector2i & pointB);
		Bounds(const Vector2i & v0, const Vector2i & v1, const Vector2i & v2);

		/// Setters
		void set(const Vector2i & min, const Vector2i & max);
		void set(const int & xmin, const int & ymin, const int & xmax, const int & ymax);
		void setByType(const BWAPI::UnitType & type, const BWAPI::TilePosition & tile);

		void limit(const int & xmin, const int & ymin, const int & xmax, const int & ymax);
		void move(const int & x, const int & y);

		Bounds & extend(const Bounds & bounds);
		Bounds & extend(const Vector2i & point);
		Bounds & extend(BWAPI::Unit unit);

		void grow(const int & radius);
		void grow(const int & xradius, const int & yradius);

		/// Getters

		Vector2i getMin() const;
		Vector2i getMax() const;
		Vector2i getSize() const;
		Vector2i centroid() const;
		float area() const;
		Vector2i offset(const Vector2i & point) const;
		// return index of the axis (x = 0, y = 1, z = 2)
		int8 getMaxAxis() const;
		int8 getMinAxis() const;


		
		static Bounds merge(const Bounds & a, const Bounds & b);
		static Bounds merge(const Bounds & a, const BWAPI::Unit & unit);
		static Bounds merge(const Bounds & aabb, const Vector2i & point);

		// infinity bounds
		//const static Bounds infinity;

		bool intersects(const Bounds & bounds) const;
		bool intersects(const int & xmin, const int & ymin, const int & xmax, const int & ymax) const;
		bool intersects(const BWAPI::Unit & unit) const;
		bool contains(const Vector2i & point) const;



		/// Static Functions

		//static bool intersects(const Bounds & a, const Bounds & b);
		//static bool contains(const Bounds & bounds, const vec3 & point);


		const Bounds & operator=(const Bounds & rhs)
		{
			this->min = rhs.min;
			this->max = rhs.max;
			//this->size = rhs.size;
			return *this;
		}

		const Bounds & operator=(const BWAPI::Unit & rhs)
		{
			this->min.set(rhs->getLeft(), rhs->getTop());
			this->max.set(rhs->getRight(), rhs->getBottom());
			//this->size = rhs.size;
			return *this;
		}

		const Bounds & operator*(const int & rhs)
		{
			
			
			//this->size = rhs.size;
			return Bounds(this->min * rhs, this->max * rhs);
		}

		const Bounds & operator/(const int & rhs)
		{


			//this->size = rhs.size;
			return Bounds(this->min / rhs, this->max / rhs);
		}
		

		Vector2i & operator[](const size_t index) {

			OAI_ASSERT_MSG(index >= 0 && index < 2, "Can not access Bounds element.");

			return (index == 0) ? min : max;
			//(&min)[index];

		}

		const Vector2i & operator[](const size_t index) const {

			OAI_ASSERT_MSG(index >= 0 && index < 2, "Can not access Bounds element.");

			return (index == 0) ? min : max;
			//return (&min)[index];

		}


	};

}

#endif
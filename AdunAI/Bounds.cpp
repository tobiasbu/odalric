
#include "Bounds.h"
#include <cmath>
#include "Math.h"

namespace Odarich {



	Bounds::Bounds()
	{
		// swaped values!
		min = std::numeric_limits<int>::max();
		max = std::numeric_limits<int>::lowest();
	}

	Bounds::Bounds(const Bounds & bounds)
		:
		min(bounds.min),
		max(bounds.max)
	{

	}

	Bounds::Bounds(const BWAPI::Unit & unit)
	{
		min.x = unit->getLeft();
		min.y = unit->getTop();
		max.x = unit->getRight();
		max.y = unit->getBottom();
	}

	/*Bounds::Bounds(const Rect & rect) :
	min(rect.position),
	max(rect.position + rect.size),
	size(rect.size),
	rect(rect)
	{

	}*/

	Bounds::Bounds(const Vector2i & a, const Vector2i & b)
		:
		min(Math::min(a.x, b.x), Math::min(a.y, b.y)),
		max(Math::max(a.x, b.x), Math::max(a.y, b.y))
		
		/*rect(min, max - min)*/
	{
	}

	Bounds::Bounds(const Vector2i & v0, const Vector2i & v1, const Vector2i & v2)
	{
		min.x = Math::min(v0.x, Math::min(v1.x, v2.x));
		min.y = Math::min(v0.y, Math::min(v1.y, v2.y));
		

		max.x = Math::max(v0.x, Math::max(v1.x, v2.x));
		max.y = Math::max(v0.y, Math::max(v1.y, v2.y));
		//max(Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z)),

	}

	Bounds::Bounds(const BWAPI::UnitType & unit, const BWAPI::TilePosition & tile)
	{
		setByType(unit, tile);
	}

	void Bounds::set(const Vector2i & min, const Vector2i & max) {
		this->min = min;
		this->max = max;
	}
	void Bounds::set(const int & xmin, const int & ymin, const int & xmax, const int & ymax)
	{
		this->min.x = xmin;
		this->min.y = ymin;
		this->max.x = xmax;
		this->max.y = ymax;
	}

	void Bounds::setByType(const BWAPI::UnitType & type, const BWAPI::TilePosition & tile)
	{
		this->min.x = tile.x;
		this->min.y = tile.y;
		this->max.x = tile.x + type.tileWidth();
		this->max.y = tile.y + type.tileHeight();
	}

	void Bounds::limit(const int & xmin, const int & ymin, const int & xmax, const int & ymax)
	{
		if (min.x < xmin)
			min.x = xmin;

		if (min.y < ymin)
			min.y = ymin;

		if (max.x > xmax)
			max.x = xmax;

		if (max.y > ymax)
			max.y = ymax;
	}

	void Bounds::move(const int & x, const int & y)
	{
		min.x += x;
		min.y += y;
		max.x += x;
		max.y += y;
	}

	Bounds & Bounds::extend(const Vector2i & point)
	{
		if (point.x < min.x) min.x = point.x;
		if (point.y < min.y) min.y = point.y;
		if (point.x > max.x) max.x = point.x;
		if (point.y > max.y) max.y = point.y;

		return *this;
	}

	Bounds & Bounds::extend(BWAPI::Unit unit)
	{
		if (unit->getLeft() < min.x) min.x = unit->getLeft();
		if (unit->getTop() < min.y) min.y = unit->getTop();
		if (unit->getRight()  > max.x) max.x = unit->getRight();
		if (unit->getBottom() > max.y) max.y = unit->getBottom();



		/*min.x = Math::min(min.x, unit->getLeft());
		min.y = Math::min(min.y, unit->getTop());
		max.x = Math::max(max.x, unit->getRight());
		max.y = Math::max(max.y, unit->getBottom());*/

		return *this;
	}

	void Bounds::grow(const int & radius)
	{
		if (max.x > min.x)
		{
			min.x -= radius;
			max.x += radius;
		}
		else
		{
			min.x += radius;
			max.x -= radius;
		}

		if (max.y > min.y)
		{
			min.y -= radius;
			max.y += radius;
		}
		else
		{
			min.y += radius;
			max.y -= radius;
		}
	}

	void Bounds::grow(const int & xradius, const int & yradius)
	{
	
		if (max.x > min.x)
		{
			min.x -= xradius;
			max.x += xradius;
		}
		else
		{
			min.x += xradius;
			max.x -= xradius;
		}

		if (max.y > min.y)
		{
			min.y -= yradius;
			max.y += yradius;
		}
		else
		{
			min.y += yradius;
			max.y -= yradius;
		}
	}

	Bounds & Bounds::extend(const Bounds & bounds)
	{
		// min merge

		if (bounds.min.x < min.x)
			min.x = bounds.min.x;

		if (bounds.min.y < min.y)
			min.y = bounds.min.y;


		// max merge

		if (bounds.max.x > max.x)
			max.x = bounds.max.x;

		if (bounds.max.y > max.y)
			max.y = bounds.max.y;


		return *this;
	}

	Bounds Bounds::merge(const Bounds & a, const BWAPI::Unit & unit)
	{
		Bounds ret;
		Bounds b(unit);

		ret.min.x = Math::min(a.min.x, b.min.x);
		ret.min.y = Math::min(a.min.y, b.min.y);
		ret.max.x = Math::max(a.max.x, b.max.x);
		ret.max.y = Math::max(a.max.y, b.max.y);

		return ret;
	}

	Bounds Bounds::merge(const Bounds & b1, const Bounds & b2)
	{
		Bounds ret;

		ret.min.x = Math::min(b1.min.x, b2.min.x);
		ret.min.y = Math::min(b1.min.y, b2.min.y);
		ret.max.x = Math::max(b1.max.x, b2.max.x);
		ret.max.y = Math::max(b1.max.y, b2.max.y);

		return ret;

	}

	Bounds Bounds::merge(const Bounds & aabb, const Vector2i & point)
	{

		/*ret.min.x = Math::min(aabb.min.x, point.x);
		ret.min.y = Math::min(aabb.min.y, point.y);
		ret.min.z = Math::min(aabb.min.z, point.z);
		ret.max.x = Math::max(aabb.max.x, point.x);
		ret.max.y = Math::max(aabb.max.y, point.y);
		ret.max.z = Math::max(aabb.max.z, point.z);

		ret.size = ret.max - ret.min;*/

		return Bounds(
			Vector2i(Math::min(aabb.min.x, point.x), Math::min(aabb.min.y, point.y)),
			Vector2i(Math::max(aabb.max.x, point.x), Math::max(aabb.max.y, point.y))
		);
	}

	/// Getters

	Vector2i Bounds::getMin() const {

		return min;

	}

	Vector2i Bounds::getMax() const {

		return max;

	}

	Vector2i Bounds::getSize() const {

		return max - min;

	}

	float Bounds::area() const
	{
		Vector2i segments = max - min;//Vector2i::abs(min - max);
								  //return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
		return (segments.x * segments.y) * 2.0f;
	}

	Vector2i Bounds::offset(const Vector2i & point) const
	{
		Vector2i o = point - min;

		if (max.x > min.x)
			o.x /= max.x - min.x;

		if (max.y > min.y)
			o.y /= max.y - min.y;

		/*if (max.z > min.z)
			o.z /= max.z - min.z;*/

		return o;
	}

	int8 Bounds::getMaxAxis() const
	{
		int8 idx = 0;
		Vector2i size = getSize();

		if (size.y > size.x)
			idx = 1;

		

		return idx;
	}

	int8 Bounds::getMinAxis() const
	{

		int8 idx = 0;
		Vector2i size = getSize();

		if (size.y < size.x)
			idx = 1;

		

		return idx;
	}

	Vector2i Bounds::centroid() const {

		return (max + min) / 2;
		//(max + min) / 2.0f;

	}

	bool Bounds::intersects(const Bounds & bounds) const
	{
		return (max > bounds.min && min < bounds.max) || (min > bounds.max && max < bounds.min);
	}

	bool Bounds::intersects(const BWAPI::Unit & unit) const
	{
		Vector2i minu(unit->getLeft(), unit->getTop());
		Vector2i maxu(unit->getRight(), unit->getBottom());

		return (max > minu && min < maxu) || (min > maxu && max < minu);
	}

	bool Bounds::intersects(const int & xmin, const int & ymin, const int & xmax, const int & ymax) const
	{
	

		return (max.x >= xmin && max.y >= ymin &&
				min.x <= xmax && min.y <= ymax) ||

			   (min.x > xmax && min.y > ymax &&
				max.x <= xmin && max.y <= ymin);
	}

	bool Bounds::contains(const Vector2i & point) const
	{
		return point > min && point < max;
	}





}
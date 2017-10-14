
#include "Rect.h"
#include "Math.h"

namespace Odalric {

	Rect::Rect()
	:
	x(0), y(0), width(0), height(0)
	{
		x = std::numeric_limits<int>::lowest();
		y = std::numeric_limits<int>::lowest();
	}

	Rect::Rect(const int & x, const int & y, const int & width, const int & height)
	:
	x(x),
	y(y),
	width(width),
	height(height)
{

}

	Rect::Rect(const Rect & rect)
		:
		x(rect.x),
		y(rect.y),
		width(rect.width),
		height(rect.height)
	{

	}

	Rect::Rect(BWAPI::Unit unit)
	{
		width = Math::max(x + width, unit->getRight());
		height = Math::max(y + height, unit->getBottom());

		x = Math::min(x, unit->getLeft());
		y = Math::min(y, unit->getTop());

		width = width - x;
		height = height - y;
	}

	Rect & Rect::merge(const Rect & rect)
	{
		// max merge
		int xw = Math::max(x + width, rect.x + rect.width);
		int yh = Math::max(y + height, rect.y + rect.height);

		// min merge
		x = Math::min(x, rect.x);
		y = Math::min(y, rect.y);
		width = xw - x;
		height = yh - y;

		return *this;
	}

	bool Rect::contains(const int & x, const int &  y) const
	{
		if (width <= 0 && height <= 0)
			return false;

		return
			(x > this->x &&
				x < this->x + width &&
				y >= this->y &&
				y < this->y + height);
	}

	bool Rect::intersects(const Rect & other) const
	{
		if (width <= 0 || height <= 0 || other.width <= 0 || other.height <= 0)
			return false;


		return    !(x > other.x + other.width ||
			x + width < other.x ||
			y > other.y + other.height ||
			y + height < other.y);

	}

}
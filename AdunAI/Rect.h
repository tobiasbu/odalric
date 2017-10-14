
#ifndef _OAI_RECT_HPP_
#define _OAI_RECT_HPP_

#include <BWAPI.h>

namespace Odarich {

	struct Rect
	{
		int x, y;
		int width, height;

		Rect();
		Rect(const Rect & rect);
		Rect(BWAPI::Unit unit);
		Rect(const int & x, const int & y, const int & width, const int & height);
		Rect & Rect::merge(const Rect & rect);
		bool Rect::contains(const int & x, const int &  y) const;
		bool intersects(const Rect & other) const;

		const Rect & operator=(const Rect & rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->width = rhs.width;
			this->height = rhs.height;
			return *this;
		}

		const Rect & operator+=(const Rect & rhs)
		{
			return this->merge(rhs);
		}

	};

}

#endif
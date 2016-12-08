
#include <fightlib/entity/collision/CollisionRect.h>

namespace fl
{
	CollisionRect::~CollisionRect()
	{
		//
	}

	bool CollisionRect::isEmpty() const
	{
		return true;
	}

	fgl::RectangleD CollisionRect::getRect() const
	{
		return fgl::RectangleD();
	}

	bool CollisionRect::isSolid() const
	{
		return false;
	}

	fgl::PixelIterator CollisionRect::createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const
	{
		return fgl::PixelIterator(fgl::Vector2u(1, 1), fgl::RectangleU(0, 0, 1, 1), fgl::RectangleD(-0.5, -0.5, 1, 1), fgl::RectangleD(-0.5, -0.5, 1, 1), increment.x, increment.y);
	}

	bool CollisionRect::check(const fgl::PixelIterator& iterator) const
	{
		return false;
	}

	fgl::Vector2d CollisionRect::getPreferredIncrement() const
	{
		return fgl::Vector2d(1.0, 1.0);
	}
}

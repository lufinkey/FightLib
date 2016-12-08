
#pragma once

#include <GameLibrary/GameLibrary.h>

namespace fl
{
	class CollisionRect
	{
	public:
		virtual ~CollisionRect();

		virtual bool isEmpty() const;

		virtual fgl::RectangleD getRect() const;
		virtual bool isSolid() const;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const;
		virtual bool check(const fgl::PixelIterator& iterator) const;

		virtual fgl::Vector2d getPreferredIncrement() const;
	};
}


#include <fightlib/entity/collision/rects/BoxCollisionRect.hpp>

namespace fl
{
	BoxCollisionRect::BoxCollisionRect(const fgl::String& tag, const fgl::RectangleD& rect, const fgl::Vector2d& lastCenter, const fgl::Vector2d& resolution)
		: CollisionRect(tag),
		rect(rect),
		lastCenter(lastCenter),
		boundingRect(rect),
		resolution(resolution),
		usesTransform(false)
	{
		//
	}

	BoxCollisionRect::BoxCollisionRect(const fgl::String& tag, const fgl::RectangleD& rect, const fgl::Vector2d& lastCenter, double rotation, const fgl::Vector2d& origin, const fgl::Vector2d& resolution)
		: CollisionRect(tag),
		rect(rect),
		lastCenter(lastCenter),
		resolution(resolution),
		usesTransform(true)
	{
		srcTransform.rotate(rotation, origin);
		fgl::RectangleD relBoundingRect = srcTransform.transform(fgl::RectangleD(0,0,rect.width,rect.height));
		boundingRect = fgl::RectangleD(rect.x+relBoundingRect.x, rect.y+relBoundingRect.y, relBoundingRect.width, relBoundingRect.height);
	}

	fgl::RectangleD BoxCollisionRect::getRect() const
	{
		return boundingRect;
	}

	bool BoxCollisionRect::isFilled() const
	{
		if(usesTransform)
		{
			return false;
		}
		return true;
	}

	fgl::PixelIterator BoxCollisionRect::createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const
	{
		if(usesTransform)
		{
			return fgl::PixelIterator(fgl::Vector2u(1, 1), fgl::RectangleU(0, 0, 1, 1), boundingRect, loopRect, increment.x, increment.y, srcTransform, fgl::Vector2d((rect.width==0 ? 0 : 1/rect.width), (rect.height==0 ? 0 : 1/rect.height)));
		}
		else
		{
			return fgl::PixelIterator(fgl::Vector2u(1, 1), fgl::RectangleU(0, 0, 1, 1), boundingRect, loopRect, increment.x, increment.y);
		}
	}

	bool BoxCollisionRect::check(const fgl::PixelIterator& iterator) const
	{
		return (iterator.getCurrentPixelIndex()>=0);
	}

	fgl::Vector2d BoxCollisionRect::getVelocity() const
	{
		return getCenter()-lastCenter;
	}

	fgl::Vector2d BoxCollisionRect::getPreferredIncrement() const
	{
		return resolution;
	}

	void BoxCollisionRect::shift(const fgl::Vector2d& shiftAmount)
	{
		rect.x += shiftAmount.x;
		rect.y += shiftAmount.y;
		boundingRect.x += shiftAmount.x;
		boundingRect.y += shiftAmount.y;
	}
}

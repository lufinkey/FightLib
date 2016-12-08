
#include <fightlib/entity/collision/BoxCollisionRect.h>

namespace fl
{
	BoxCollisionRect::BoxCollisionRect(const fgl::RectangleD& rect, const fgl::Vector2d& resolution)
		: rect(rect),
		boundingRect(rect),
		resolution(resolution),
		usesTransform(false)
	{
		//
	}

	BoxCollisionRect::BoxCollisionRect(const fgl::RectangleD& rect, double rotation, const fgl::Vector2d& origin, const fgl::Vector2d& resolution)
		: rect(rect),
		resolution(resolution),
		usesTransform(true)
	{
		srcTransform.rotate(rotation, origin);
		fgl::RectangleD relBoundingRect = srcTransform.transform(fgl::RectangleD(0,0,rect.width,rect.height));
		boundingRect = fgl::RectangleD(rect.x+relBoundingRect.x, rect.y+relBoundingRect.y, relBoundingRect.width, relBoundingRect.height);
	}

	bool BoxCollisionRect::isEmpty() const
	{
		if(rect.width==0 || rect.height==0)
		{
			return true;
		}
		return false;
	}

	fgl::RectangleD BoxCollisionRect::getRect() const
	{
		return boundingRect;
	}

	bool BoxCollisionRect::isSolid() const
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

	fgl::Vector2d BoxCollisionRect::getPreferredIncrement() const
	{
		return resolution;
	}
}

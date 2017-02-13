
#include <fightlib/entity/collision/PixelCollisionRect.h>

namespace fl
{
	PixelCollisionRect::PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::Vector2d& velocity, const fgl::RectangleU srcRect, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical)
		: dstRect(dstRect),
		velocity(velocity),
		srcRect(srcRect),
		boundingRect(dstRect),
		image(image),
		mirroredHorizontal(mirroredHorizontal),
		mirroredVertical(mirroredVertical),
		usesTransform(false)
	{
		//
	}

	PixelCollisionRect::PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::Vector2d& velocity, const fgl::RectangleU srcRect, double rotation, const fgl::Vector2d& origin, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical)
		: dstRect(dstRect),
		velocity(velocity),
		srcRect(srcRect),
		image(image),
		mirroredHorizontal(mirroredHorizontal),
		mirroredVertical(mirroredVertical),
		usesTransform(true)
	{
		srcTransform.rotate(rotation, origin);
		fgl::RectangleD relBoundingRect = srcTransform.transform(fgl::RectangleD(0, 0, dstRect.width, dstRect.height));
		boundingRect = fgl::RectangleD(dstRect.x+relBoundingRect.x, dstRect.y+relBoundingRect.y, relBoundingRect.width, relBoundingRect.height);
	}

	fgl::RectangleD PixelCollisionRect::getRect() const
	{
		return boundingRect;
	}

	bool PixelCollisionRect::isFilled() const
	{
		return false;
	}

	fgl::PixelIterator PixelCollisionRect::createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const
	{
		if(usesTransform)
		{
			return fgl::PixelIterator(image->getDimensions(), srcRect, boundingRect, loopRect, increment.x, increment.y, srcTransform, fgl::Vector2d(dstRect.width/srcRect.width, dstRect.height/srcRect.height), mirroredHorizontal, mirroredVertical);
		}
		else
		{
			return fgl::PixelIterator(image->getDimensions(), srcRect, boundingRect, loopRect, increment.x, increment.y, mirroredHorizontal, mirroredVertical);
		}
	}

	bool PixelCollisionRect::check(const fgl::PixelIterator& iterator) const
	{
		double index = iterator.getCurrentPixelIndex();
		if(index >= 0)
		{
			return image->checkPixel((unsigned int)index);
		}
		return false;
	}

	fgl::Vector2d PixelCollisionRect::getVelocity() const
	{
		return velocity;
	}

	fgl::Vector2d PixelCollisionRect::getPreferredIncrement() const
	{
		return fgl::Vector2d(dstRect.width/srcRect.width, dstRect.height/srcRect.height);
	}
}

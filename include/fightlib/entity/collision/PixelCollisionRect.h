
#pragma once

#include "CollisionRect.h"

namespace fl
{
	class PixelCollisionRect : public CollisionRect
	{
	public:
		PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::RectangleU srcRect, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);
		PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::RectangleU srcRect, double rotation, const fgl::Vector2d& origin, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);

		virtual bool isEmpty() const override;

		virtual fgl::RectangleD getRect() const override;
		virtual bool isSolid() const override;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const override;
		virtual bool check(const fgl::PixelIterator& iterator) const override;

		virtual fgl::Vector2d getPreferredIncrement() const override;

	private:
		fgl::RectangleD dstRect;
		fgl::RectangleU srcRect;
		fgl::RectangleD boundingRect;
		fgl::TransformD srcTransform;
		fgl::TextureImage* image;

		bool mirroredHorizontal;
		bool mirroredVertical;

		bool usesTransform;
	};
}

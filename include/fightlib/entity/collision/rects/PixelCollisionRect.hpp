
#pragma once

#include "CollisionRect.hpp"

namespace fl
{
	class PixelCollisionRect : public CollisionRect
	{
	public:
		PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::Vector2d& velocity, const fgl::RectangleU srcRect, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);
		PixelCollisionRect(const fgl::RectangleD& dstRect, const fgl::Vector2d& velocity, const fgl::RectangleU srcRect, double rotation, const fgl::Vector2d& origin, fgl::TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);

		virtual fgl::RectangleD getRect() const override;
		virtual bool isFilled() const override;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const override;
		virtual bool check(const fgl::PixelIterator& iterator) const override;

		virtual fgl::Vector2d getVelocity() const override;
		virtual fgl::Vector2d getPreferredIncrement() const override;

		virtual void shift(const fgl::Vector2d& shiftAmount) override;

	private:
		fgl::RectangleD dstRect;
		fgl::Vector2d velocity;
		fgl::RectangleU srcRect;
		fgl::RectangleD boundingRect;
		fgl::TransformD srcTransform;
		fgl::TextureImage* image;

		bool mirroredHorizontal;
		bool mirroredVertical;

		bool usesTransform;
	};
}

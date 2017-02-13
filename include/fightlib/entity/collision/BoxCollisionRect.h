
#pragma once

#include "CollisionRect.h"

namespace fl
{
	class BoxCollisionRect : public CollisionRect
	{
	public:
		explicit BoxCollisionRect(const fgl::RectangleD& rect, const fgl::Vector2d& velocity, const fgl::Vector2d& resolution=fgl::Vector2d(1.0, 1.0));
		BoxCollisionRect(const fgl::RectangleD& rect, const fgl::Vector2d& velocity, double rotation, const fgl::Vector2d& origin, const fgl::Vector2d& resolution=fgl::Vector2d(1.0, 1.0));

		virtual fgl::RectangleD getRect() const override;
		virtual bool isFilled() const override;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const override;
		virtual bool check(const fgl::PixelIterator& iterator) const override;

		virtual fgl::Vector2d getVelocity() const override;
		virtual fgl::Vector2d getPreferredIncrement() const override;

	private:
		fgl::RectangleD rect;
		fgl::Vector2d velocity;
		fgl::RectangleD boundingRect;
		fgl::TransformD srcTransform;
		fgl::Vector2d resolution;
		bool usesTransform;
	};
}

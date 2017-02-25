
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class CollisionRect
	{
	public:
		virtual ~CollisionRect() {}

		virtual fgl::RectangleD getRect() const = 0;
		virtual bool isFilled() const = 0;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const = 0;
		virtual bool check(const fgl::PixelIterator& iterator) const = 0;

		virtual fgl::Vector2d getVelocity() const = 0;
		virtual fgl::Vector2d getPreferredIncrement() const = 0;

		//offset is rect2's offset to avoid rect1
		static fgl::Vector2d checkCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);

	private:
		static fgl::Vector2d checkFilledCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static fgl::Vector2d checkPixelOnFilledCollision(CollisionRect* pixelRect, CollisionRect* filledRect);
		static fgl::Vector2d checkPixelCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
	};
}

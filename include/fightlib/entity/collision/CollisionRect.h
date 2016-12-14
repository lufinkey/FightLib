
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
		virtual bool isFilled() const;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const;
		virtual bool check(const fgl::PixelIterator& iterator) const;

		virtual fgl::Vector2d getVelocity() const;
		virtual fgl::Vector2d getPreferredIncrement() const;

		//offset is rect2's offset to avoid rect1
		static fgl::Vector2d checkCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);

	private:
		static fgl::Vector2d checkFilledCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static fgl::Vector2d checkPixelOnFilledCollision(CollisionRect* pixelRect, CollisionRect* filledRect);
		static fgl::Vector2d checkPixelCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
	};
}

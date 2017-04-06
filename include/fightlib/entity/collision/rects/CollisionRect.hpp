
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class CollisionRect
	{
	public:
		CollisionRect(const fgl::String& tag);
		virtual ~CollisionRect();

		const fgl::String& getTag() const;
		fgl::Vector2d getCenter() const;
		virtual fgl::RectangleD getRect() const = 0;
		virtual fgl::RectangleD getPreviousRect() const = 0;
		virtual bool isFilled() const = 0;
		virtual fgl::PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const fgl::Vector2d& increment) const = 0;
		virtual bool check(const fgl::PixelIterator& iterator) const = 0;
		
		fgl::Vector2d getTotalVelocity() const;

		virtual fgl::Vector2d getPreferredIncrement() const = 0;

		virtual void shift(const fgl::Vector2d& shiftAmount) = 0;

		//offset is rect2's offset to avoid rect1
		static fgl::Vector2d getCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);

	private:
		static fgl::Vector2d getFilledCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static fgl::Vector2d getPixelOnFilledCollisionOffset(CollisionRect* pixelRect, CollisionRect* filledRect);
		static fgl::Vector2d getPixelCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);

		fgl::String tag;
	};
}


#pragma once

#include <fightlib/entity/draw/Sprite.hpp>
#include "rects/CollisionRect.hpp"

namespace fl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;

	class Collidable : public Sprite
	{
		friend class CollisionManager;
		friend class CollisionRectManager;
	public:
		Collidable(const fgl::Vector2d& position);
		
		virtual bool isStaticCollisionBody() const = 0;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const = 0;
		
		const fgl::Vector2d& getPreviousPosition() const;

	protected:
		virtual void shift(const fgl::Vector2d& offset) = 0;

		bool respondsToCollision(Collidable* collided, CollisionSide side) const;

		virtual void onCollision(Collidable* collided, CollisionSide side);
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side);
		virtual void onCollisionFinish(Collidable* collided, CollisionSide side);
		virtual void onFinishCollisionUpdates();
		
	private:
		fgl::Vector2d previousPosition;
	};
}

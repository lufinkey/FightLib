
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
		virtual bool isStaticCollisionBody() const = 0;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const = 0;

	protected:
		virtual void shift(const fgl::Vector2d& offset) = 0;

		virtual void onCollision(Collidable* collided, CollisionSide side) {}
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side) {}
		virtual void onCollisionFinish(Collidable* collided, CollisionSide side) {}
		virtual void onFinishCollisionUpdates() {}
	};
}

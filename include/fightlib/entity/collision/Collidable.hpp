
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
		
		virtual void update(const fgl::ApplicationData& appData) override;
		
		virtual bool isStaticCollisionBody() const = 0;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const = 0;
		
		const fgl::Vector2d& getPreviousPosition() const;
		
		void setVelocity(const fgl::Vector2d& velocity);
		const fgl::Vector2d& getVelocity() const;
		
		bool checkCollision(Collidable* collidable) const;

	protected:
		virtual void shift(const fgl::Vector2d& offset) = 0;

		virtual bool respondsToCollision(Collidable* collided, CollisionSide side) const;

		virtual void onCollision(Collidable* collided, CollisionSide side);
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side);
		virtual void onCollisionFinish(Collidable* collided, CollisionSide side);
		virtual void onFinishCollisionUpdates();
		
	private:
		fgl::Vector2d previousPosition;
		fgl::Vector2d velocity;
	};
}

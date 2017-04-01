
#pragma once

#include "Entity.hpp"

namespace fl
{
	class Item : public Entity
	{
	public:
		using Entity::Entity;

	protected:
		virtual bool respondsToCollision(Collidable* collided, CollisionSide side) const override;

		virtual void onCollision(Collidable* collided, CollisionSide side) override;
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side) override;
	};
}

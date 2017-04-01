
#pragma once

#include "Entity.hpp"

namespace fl
{
	class Item : public Entity
	{
	public:
		using Entity::Entity;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

	protected:
		virtual void onCollision(Collidable* collided, CollisionSide side) override;
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side) override;
	};
}

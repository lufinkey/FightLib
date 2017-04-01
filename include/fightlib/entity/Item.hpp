
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
	};
}

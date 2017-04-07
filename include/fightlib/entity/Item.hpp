
#pragma once

#include "Entity.hpp"

namespace fl
{
	class Item : public Entity
	{
	public:
		using Entity::Entity;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		virtual fgl::ArrayList<MetaPointType> getAnchorPoints() const = 0;
		virtual bool isPickedUpByTouching() const;

	protected:
		virtual void onCollision(Collidable* collided, CollisionSide side) override;
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side) override;
	};
}

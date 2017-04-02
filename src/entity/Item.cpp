
#include <fightlib/entity/Item.hpp>

namespace fl
{
	fgl::ArrayList<CollisionRect*> Item::getCollisionRects() const
	{
		if(getParentEntity()==nullptr)
		{
			return Entity::getCollisionRects();
		}
		return {};
	}

	void Item::onCollision(Collidable* collided, CollisionSide side)
	{
		if(side==fl::COLLISIONSIDE_BOTTOM)
		{
			fgl::Vector2d velocity = getVelocity();
			if(velocity.y > 0)
			{
				velocity.y = 0;
				setVelocity(velocity);
			}
		}
		Entity::onCollision(collided, side);
	}

	void Item::onCollisionUpdate(Collidable* collided, CollisionSide side)
	{
		if(side==fl::COLLISIONSIDE_BOTTOM)
		{
			fgl::Vector2d velocity = getVelocity();
			if(velocity.y > 0)
			{
				velocity.y = 0;
				setVelocity(velocity);
			}
		}
		Entity::onCollisionUpdate(collided, side);
	}
}

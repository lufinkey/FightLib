
#include <fightlib/entity/Item.hpp>

namespace fl
{
	Item::Item(const fgl::Vector2d& position, Orientation orientation)
		: Entity(position, orientation),
		parentCharacter(nullptr)
	{
		//
	}
	
	fgl::ArrayList<CollisionRect*> Item::getCollisionRects() const
	{
		if(getParentEntity()==nullptr)
		{
			return Entity::getCollisionRects();
		}
		return {};
	}
	
	bool Item::isPickedUpByTouching() const
	{
		return false;
	}
	
	Character* Item::getParentCharacter() const
	{
		return parentCharacter;
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
	
	void Item::onPickUp(Character* character)
	{
		//
	}
	
	void Item::onDiscard(Character* character)
	{
		//
	}
}

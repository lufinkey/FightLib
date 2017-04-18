
#include <fightlib/entity/Item.hpp>
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Item::Item(const fgl::Vector2d& position, Orientation orientation)
		: ActionEntity(position, orientation),
		parentCharacter(nullptr)
	{
		//
	}
	
	bool Item::getFlag(const fgl::String& flag) const
	{
		if(flag=="Item")
		{
			return true;
		}
		return ActionEntity::getFlag(flag);
	}
	
	void Item::draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const
	{
		//don't draw if the item is being held as a powerup and is not anchored to anything
		if(!(getParentCharacter()!=nullptr && getParentEntity()==nullptr && isPowerUp()))
		{
			ActionEntity::draw(appData, graphics);
		}
	}
	
	fgl::Vector2d Item::getDrawPosition(float* rotation) const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr && isPowerUp())
		{
			return getParentCharacter()->getPosition(rotation);
		}
		return ActionEntity::getDrawPosition(rotation);
	}
	
	fgl::Vector2d Item::getPosition(float* rotation) const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr && isPowerUp())
		{
			return getParentCharacter()->getPosition(rotation);
		}
		return ActionEntity::getPosition(rotation);
	}
	
	fgl::ArrayList<CollisionRect*> Item::getCollisionRects() const
	{
		if(getParentEntity()==nullptr)
		{
			return ActionEntity::getCollisionRects();
		}
		return {};
	}
	
	bool Item::isPickedUpByTouching() const
	{
		return false;
	}
	
	bool Item::isPowerUp() const
	{
		return false;
	}
	
	Character* Item::getParentCharacter() const
	{
		return parentCharacter;
	}
	
	bool Item::respondsToCollision(Collidable* collided, CollisionSide side) const
	{
		if(collided->getFlag("Character"))
		{
			return false;
		}
		return true;
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

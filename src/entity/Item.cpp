
#include <fightlib/entity/Item.hpp>
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Item::Item(const fgl::Vector2d& position, Orientation orientation)
		: ActionEntity(position, orientation),
		parentCharacter(nullptr),
		beingUsed(false)
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
	
	void Item::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			//don't draw if the item is held but is not equipped
			return;
		}
		ActionEntity::draw(appData, graphics);
	}
	
	fgl::Vector2d Item::getDrawPosition(float* rotation) const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			//return the parent character's position if the item is held but not equipped
			return getParentCharacter()->getPosition(rotation);
		}
		return ActionEntity::getDrawPosition(rotation);
	}
	
	fgl::Vector2d Item::getPosition(float* rotation) const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			//return the parent character's position if the item is held but not equipped
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

	bool Item::isEquippable() const
	{
		if(getAnchorPoints().size() > 0)
		{
			return true;
		}
		return false;
	}
	
	Character* Item::getParentCharacter() const
	{
		return parentCharacter;
	}
	
	void Item::startUse()
	{
		if(!beingUsed)
		{
			beingUsed = true;
			onStartUse();
		}
	}
	
	void Item::stopUse()
	{
		if(beingUsed)
		{
			beingUsed = false;
			onStopUse();
		}
	}
	
	bool Item::isBeingUsed() const
	{
		return beingUsed;
	}
	
	bool Item::respondsToCollision(Collidable* collided, CollisionSide side) const
	{
		if(collided->getFlag("Character"))
		{
			return false;
		}
		else if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			//don't allow collisions if the item is held but not equipped
			return false;
		}
		return ActionEntity::respondsToCollision(collided, side);
	}
	
	void Item::onPickUp()
	{
		//
	}
	
	void Item::onDiscard()
	{
		//
	}

	void Item::onEquip()
	{
		//
	}

	void Item::onUnequip()
	{
		//
	}
	
	void Item::onStartUse()
	{
		//
	}
	
	void Item::onStopUse()
	{
		//
	}
}

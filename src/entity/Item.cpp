
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

	void Item::update(fgl::ApplicationData appData)
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			setVelocity(fgl::Vector2d(0, 0));
		}
		ActionEntity::update(appData);
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
	
	fgl::ArrayList<fgl::PolygonD> Item::getCollisionPolygons() const
	{
		if(getParentCharacter()!=nullptr && getParentEntity()==nullptr)
		{
			//don't collide with anything if the item is held but is not equipped
			return {};
		}
		return ActionEntity::getCollisionPolygons();
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
	
	bool Item::respondsToCollision(Collidable* collided, CollisionData data) const
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
		return ActionEntity::respondsToCollision(collided, data);
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
}

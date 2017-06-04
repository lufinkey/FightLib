
#include <fightlib/entity/Character.hpp>
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	Character::Character(const fgl::Vector2d& position, Orientation orientation)
		: ActionEntity(position, orientation)
	{
		//
	}
	
	bool Character::getFlag(const fgl::String& flag) const
	{
		if(flag=="Character")
		{
			return true;
		}
		return ActionEntity::getFlag(flag);
	}

	void Character::update(fgl::ApplicationData appData)
	{
		if(getCurrentAction()==nullptr || getCurrentAction()->getFlag(ACTIONFLAG_ALLOWMOVEMENT))
		{
			updateMovement(appData);
		}
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
		//TODO if the character is standing on a moving platform, shift it by the platform's velocity
		ActionEntity::update(appData);
	}

	double Character::getMoveSpeed(double amount) const
	{
		return 200;
	}

	void Character::updateMovement(const fgl::ApplicationData& appData)
	{
		double moveSpeed = 0;
		double moveAmount = fgl::Math::abs(getDirection().x);
		if(moveAmount!=0)
		{
			moveSpeed = getMoveSpeed(moveAmount);
		}
		//TODO add gradual movement toward the move speed
		fgl::Vector2d velocity = getVelocity();
		double realSpeed = (direction.x >= 0) ? moveSpeed : -moveSpeed;
		if((realSpeed > 0 && realSpeed > velocity.x) || (realSpeed < 0 && realSpeed < velocity.x))
		{
			velocity.x = realSpeed;
			setVelocity(velocity);
		}

		if(getCurrentAction()==nullptr || getCurrentAction()->getFlag(ACTIONFLAG_ALLOWORIENTATIONCHANGE))
		{
			if(direction.x < 0)
			{
				setOrientation(ORIENTATION_LEFT);
			}
			else if(direction.x > 0)
			{
				setOrientation(ORIENTATION_RIGHT);
			}
		}
	}

	void Character::updateMoveAnimation()
	{
		double moveAmount = fgl::Math::abs(getDirection().x);
		fgl::String animName;
		if(moveAmount==0)
		{
			animName = getIdleAnimationName();
		}
		else
		{
			animName = getMoveAnimationName(moveAmount);
		}
		//TODO maybe throw some sort of exception if either function returns an empty string?
		if(animName.length() > 0 && animName!=getCurrentAnimationName())
		{
			//TODO add frame preservation?
			changeAnimation(animName);
		}
	}

	fgl::ArrayList<MetaPointType> Character::getItemAnchorPoints() const
	{
		return {};
	}
	
	fgl::ArrayList<MetaPointType> Character::getAvailableItemAnchorPoints() const
	{
		auto availablePoints = getItemAnchorPoints();
		for(auto& container : itemContainers)
		{
			availablePoints.removeFirstEqual(container.anchorPoint);
		}
		return availablePoints;
	}
	
	bool Character::pickUpItem(Item* item)
	{
		if(item->parentCharacter != nullptr)
		{
			throw fgl::IllegalArgumentException("item", "already being held by a character");
		}

		if(canPickUpItem(item))
		{
			//make sure no other characters can see the item as accessible
			auto stage = getStage();
			if(stage!=nullptr)
			{
				stage->removeAccessibleItem(item);
			}

			ItemContainer container;
			container.item = item;
			container.anchorPointIndex = 0;
			container.anchorPoint = 0;
			container.equipped = false;

			itemContainers.add(container);
			item->parentCharacter = this;

			item->onPickUp();
			onPickUpItem(item);

			return true;
		}
		return false;
	}
	
	void Character::discardItem(Item* item)
	{
		for(size_t i=0; i<itemContainers.size(); i++)
		{
			auto& container = itemContainers[i];
			if(container.item==item)
			{
				setItemEquipped(item, false);
				itemContainers.remove(i);
				removeAnchoredEntity(item);
				item->parentCharacter = nullptr;
				
				item->onDiscard();
				onDiscardItem(item);

				return;
			}
		}
	}
	
	bool Character::isCarryingItem(Item* item) const
	{
		for(auto& container : itemContainers)
		{
			if(container.item==item)
			{
				return true;
			}
		}
		return false;
	}

	fgl::ArrayList<Item*> Character::getEquippedItems() const
	{
		fgl::ArrayList<Item*> equippedItems;
		equippedItems.reserve(itemContainers.size());
		for(auto& container : itemContainers)
		{
			if(container.equipped)
			{
				equippedItems.add(container.item);
			}
		}
		return equippedItems;
	}

	fgl::ArrayList<Item*> Character::getUnequippedItems() const
	{
		fgl::ArrayList<Item*> unequippedItems;
		unequippedItems.reserve(itemContainers.size());
		for(auto& container : itemContainers)
		{
			if(!container.equipped)
			{
				unequippedItems.add(container.item);
			}
		}
		return unequippedItems;
	}

	fgl::ArrayList<Item*> Character::getItems() const
	{
		fgl::ArrayList<Item*> items;
		items.reserve(itemContainers.size());
		for(auto& container : itemContainers)
		{
			items.add(container.item);
		}
		return items;
	}

	bool Character::setItemEquipped(Item* item, bool equipped)
	{
		if(item->parentCharacter!=this)
		{
			throw fgl::IllegalArgumentException("item", "must be held by this Character in order to equip and unequip");
		}
		for(auto& container : itemContainers)
		{
			if(container.item==item)
			{
				if(equipped)
				{
					if(container.equipped)
					{
						return true;
					}

					//item is not equipped. equip it
					if(!canEquipItem(item))
					{
						//can't equip that item
						return false;
					}
					auto equipPoints = getMatchingEquipPoints(item);
					if(equipPoints.size() == 0)
					{
						//no available anchor points to equip item to
						return false;
					}

					auto equipPoint = equipPoints[0];
					container.anchorPoint = equipPoint.anchorPoint;
					container.anchorPointIndex = equipPoint.anchorPointIndex;
					container.equipped = true;
					anchorChildEntity(item, METAPOINT_HANDLE, 0, container.anchorPoint, container.anchorPointIndex);

					container.item->onEquip();
					onEquipItem(container.item);
				}
				else
				{
					if(!container.equipped)
					{
						return true;
					}

					//item is equipped. unequip it
					removeAnchoredEntity(item);
					container.equipped = false;

					container.item->onUnequip();
					onUnequipItem(container.item);
				}
				return true;
			}
		}
		throw fgl::IllegalStateException("inconsistant item->parentCharacter state");
	}

	bool Character::isItemEquipped(Item* item) const
	{
		for(auto& container : itemContainers)
		{
			if(container.item==item)
			{
				return container.equipped;
			}
		}
		return false;
	}

	bool Character::isItemEquippable(Item* item) const
	{
		if(item->parentCharacter!=this)
		{
			return false;
		}
		auto equipPoints = getMatchingEquipPoints(item);
		if(equipPoints.size()==0)
		{
			return false;
		}
		return true;
	}

	fgl::ArrayList<Character::EquipPoint> Character::getMatchingEquipPoints(Item* item) const
	{
		auto anchorPoints = item->getAnchorPoints().unique();
		if(anchorPoints.size()==0)
		{
			return {};
		}
		auto availableAnchorPoints = getAvailableItemAnchorPoints();
		fgl::ArrayList<EquipPoint> matchingEquipPoints;
		//find points shared between the item's supported points and the parent's available points
		for(auto& anchorPoint : anchorPoints)
		{
			//add all the matching anchor point indexes
			for(size_t i=0; i<availableAnchorPoints.size(); i++)
			{
				auto availablePoint = availableAnchorPoints[i];
				if(anchorPoint == availablePoint)
				{
					EquipPoint equipPoint;
					equipPoint.anchorPoint = anchorPoint;
					equipPoint.anchorPointIndex = i;
					matchingEquipPoints.add(equipPoint);
				}
			}
		}

		if(matchingEquipPoints.size()==0)
		{
			//no available anchor points for the item
			return {};
		}

		for(auto& container : itemContainers)
		{
			for(size_t i=0; i<matchingEquipPoints.size(); i++)
			{
				auto& equipPoint = matchingEquipPoints[i];
				if(container.equipped && equipPoint.anchorPoint==container.anchorPoint && equipPoint.anchorPointIndex==container.anchorPointIndex)
				{
					matchingEquipPoints.remove(i);
					break;
				}
			}
		}
		return matchingEquipPoints;
	}
	
	void Character::setDirection(const fgl::Vector2f& direction_arg)
	{
		direction = direction_arg;
	}

	const fgl::Vector2f& Character::getDirection() const
	{
		return direction;
	}
	
	void Character::onActionEnd(Action* action)
	{
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
	}

	bool Character::respondsToHitboxClash(Entity* clashedEntity) const
	{
		return false;
	}

	bool Character::canCollideWithEntityHitbox(Entity* collidedEntity) const
	{
		return true;
	}
	
	void Character::onFinishCollisionUpdates()
	{
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
		ActionEntity::onFinishCollisionUpdates();
	}
	
	bool Character::canPickUpItem(Item* item) const
	{
		return true;
	}
	
	void Character::onPickUpItem(Item* item)
	{
		//
	}
	
	void Character::onDiscardItem(Item* item)
	{
		//
	}

	bool Character::canEquipItem(Item* item) const
	{
		return true;
	}

	void Character::onEquipItem(Item* item)
	{
		//
	}

	void Character::onUnequipItem(Item* item)
	{
		//
	}
}

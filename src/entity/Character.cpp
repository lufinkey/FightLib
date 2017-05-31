
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
		for(auto& heldItem : heldItems)
		{
			size_t anchorPointIndex = availablePoints.indexOf(heldItem.anchorPoint);
			if(anchorPointIndex!=-1)
			{
				availablePoints.remove(anchorPointIndex);
			}
		}
		return availablePoints;
	}
	
	bool Character::pickUpItem(Item* item)
	{
		if(item->parentCharacter != nullptr)
		{
			throw fgl::IllegalArgumentException("item", "already being held by a character");
		}
		
		if(item->isPowerUp())
		{
			//powerup items don't get anchored to the character entity
			
			if(shouldPickUpItem(item))
			{
				//make sure no other characters can see the item as accessible
				auto stage = getStage();
				if(stage!=nullptr)
				{
					stage->removeAccessibleItem(item);
				}
				
				heldPowerups.add(item);
				item->parentCharacter = this;
				
				item->onPickUp(this);
				onPickUpItem(item);
				
				return true;
			}
		}
		else
		{
			//non-powerup items must get anchored to the character entity
			
			auto anchorPoints = item->getAnchorPoints();
			auto availableAnchorPoints = getAvailableItemAnchorPoints();
			fgl::ArrayList<MetaPointType> matchingAnchorPoints;
			//find points shared between the item's supported points and the parent's available points
			for(auto& anchorPoint : anchorPoints)
			{
				for(auto& availablePoint : availableAnchorPoints)
				{
					if(anchorPoint == availablePoint)
					{
						matchingAnchorPoints.add(anchorPoint);
						break;
					}
				}
			}
			
			if(matchingAnchorPoints.size()==0)
			{
				//no available anchor points for the item
				return false;
			}
			
			MetaPointType chosenPoint = matchingAnchorPoints[0];
			size_t chosenIndex = 0;
			
			//look for an available anchor point index
			bool chosenIndexTaken = false;
			do
			{
				chosenIndexTaken = false;
				for(auto& heldItem : heldItems)
				{
					if(heldItem.anchorPoint==chosenPoint && heldItem.anchorPointIndex==chosenIndex)
					{
						chosenIndexTaken = true;
						chosenIndex++;
						break;
					}
				}
			} while(chosenIndexTaken);
			
			if(shouldPickUpItem(item))
			{
				//make sure no other characters can see the item as accessible
				auto stage = getStage();
				if(stage!=nullptr)
				{
					stage->removeAccessibleItem(item);
				}
				
				//add to held items
				HeldItem heldItem;
				heldItem.item = item;
				heldItem.anchorPoint = chosenPoint;
				heldItem.anchorPointIndex = chosenIndex;
				heldItems.add(heldItem);
				
				//anchor the item
				anchorChildEntity(item, METAPOINT_HANDLE, 0, chosenPoint, chosenIndex);
				
				item->parentCharacter = this;
				
				item->onPickUp(this);
				onPickUpItem(item);
				
				return true;
			}
		}
		return false;
	}
	
	void Character::discardItem(Item* item)
	{
		for(size_t i=0; i<heldItems.size(); i++)
		{
			auto& heldItem = heldItems[i];
			if(heldItem.item==item)
			{
				heldItems.remove(i);
				removeAnchoredEntity(item);
				item->parentCharacter = nullptr;
				
				item->onDiscard(this);
				onDiscardItem(item);
				
				return;
			}
		}
		for(size_t i=0; i<heldPowerups.size(); i++)
		{
			auto powerupItem = heldPowerups[i];
			if(powerupItem==item)
			{
				heldPowerups.remove(i);
				item->parentCharacter = nullptr;
				
				item->onDiscard(this);
				onDiscardItem(item);
				return;
			}
		}
	}
	
	bool Character::isHoldingItem(Item* item) const
	{
		for(auto& heldItem : heldItems)
		{
			if(heldItem.item==item)
			{
				return true;
			}
		}
		if(heldPowerups.contains(item))
		{
			return true;
		}
		return false;
	}
	
	fgl::ArrayList<Item*> Character::getHeldItems() const
	{
		fgl::ArrayList<Item*> items;
		items.reserve(heldItems.size() + heldPowerups.size());
		
		for(auto& heldItem : heldItems)
		{
			items.add(heldItem.item);
		}
		for(auto& item : heldPowerups)
		{
			items.add(item);
		}
		
		return items;
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
	
	bool Character::shouldPickUpItem(Item* item)
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
}

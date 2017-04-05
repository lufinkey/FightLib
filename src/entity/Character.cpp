
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Character::Character(const fgl::Vector2d& position, Orientation orientation)
		: ActionEntity(position, orientation),
		fight(nullptr),
		groundCollidedCount(0)
	{
		//
	}

	void Character::update(const fgl::ApplicationData& appData)
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
		velocity.x = (direction.x >= 0) ? moveSpeed : -moveSpeed;
		setVelocity(velocity);

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
	
	void Character::setDirection(const fgl::Vector2f& direction_arg)
	{
		direction = direction_arg;
	}

	const fgl::Vector2f& Character::getDirection() const
	{
		return direction;
	}
	
	bool Character::isOnGround() const
	{
		if(groundCollidedCount > 0)
		{
			return true;
		}
		return false;
	}
	
	Fight* Character::getFight() const
	{
		return fight;
	}
	
	void Character::onActionEnd(Action* action)
	{
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
	}
	
	void Character::onCollision(fl::Collidable* collided, CollisionSide side)
	{
		if(side==COLLISIONSIDE_BOTTOM)
		{
			groundCollidedCount++;

			fgl::Vector2d velocity = getVelocity();
			if(velocity.y > 0)
			{
				velocity.y = 0;
				setVelocity(velocity);
			}
		}
		ActionEntity::onCollision(collided, side);
	}

	void Character::onCollisionUpdate(fl::Collidable* collided, CollisionSide side)
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
		ActionEntity::onCollisionUpdate(collided, side);
	}
	
	void Character::onCollisionFinish(fl::Collidable* collided, CollisionSide side)
	{
		if(side==COLLISIONSIDE_BOTTOM)
		{
			groundCollidedCount--;
		}
		ActionEntity::onCollisionFinish(collided, side);
	}
	
	void Character::onFinishCollisionUpdates()
	{
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
		ActionEntity::onFinishCollisionUpdates();
	}
}

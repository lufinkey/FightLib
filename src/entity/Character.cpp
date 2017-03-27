
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Character::Character(const fgl::Vector2d& position, Orientation orientation) : ActionEntity(position, orientation)
	{
		//
	}

	void Character::update(const fgl::ApplicationData& appData)
	{
		if(getCurrentAction()==nullptr)
		{
			updateMovement(appData);
			updateMoveAnimation(appData);
		}
		ActionEntity::update(appData);
	}

	fgl::String Character::getIdleAnimationName() const
	{
		return "";
	}

	double Character::getMoveSpeed(double amount) const
	{
		return 20;
	}

	fgl::String Character::getMoveAnimationName(double amount) const
	{
		return "";
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

		if(direction.x < 0)
		{
			setOrientation(ORIENTATION_LEFT);
		}
		else if(direction.x > 0)
		{
			setOrientation(ORIENTATION_RIGHT);
		}
	}

	void Character::updateMoveAnimation(const fgl::ApplicationData& appData)
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
		if(animName.length() > 0 && animName!=getCurrentAnimationName())
		{
			//TODO add frame preservation?
			changeAnimation(animName);
		}
	}
	//TODO I might need update the move animation when an action ends

	void Character::setDirection(const fgl::Vector2f& direction_arg)
	{
		direction = direction_arg;
	}

	const fgl::Vector2f& Character::getDirection() const
	{
		return direction;
	}
}

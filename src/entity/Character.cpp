
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Character::Character(const fgl::Vector2d& position, Orientation orientation) : ActionEntity(position, orientation)
	{
		//
	}

	void Character::update(const fgl::ApplicationData& appData)
	{
		if(getCurrentAction()==nullptr || getCurrentAction()->getFlag("AllowMovement"))
		{
			updateMovement(appData);
		}
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
		ActionEntity::update(appData);
	}

	double Character::getMoveSpeed(double amount) const
	{
		return 20;
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
	
	void Character::onActionEnd(Action* action)
	{
		if(getCurrentAction()==nullptr)
		{
			updateMoveAnimation();
		}
	}
}

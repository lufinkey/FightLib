
#include <fightlib/entity/Character.hpp>

namespace fl
{
	Character::Character(const fgl::Vector2d& position, Entity::Orientation orientation) : ActionEntity(position, orientation)
	{
		//
	}

	void Character::update(fgl::ApplicationData appData)
	{
		if(getCurrentAction()==nullptr)
		{
			updateMovement(appData);
			updateMoveAnimation(appData);
		}
		ActionEntity::update(appData);
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
	}

	void Character::updateMoveAnimation(const fgl::ApplicationData& appData)
	{
		fgl::String animName = getMoveAnimationName(fgl::Math::abs(getDirection().x));
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
}

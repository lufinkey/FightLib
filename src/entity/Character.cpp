
#include <fightlib/entity/Character.hpp>

namespace fl
{
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
		double moveSpeed = getMoveSpeed(fgl::Math::abs(getDirection().x));
		//TODO add gradual movement toward the move speed
		fgl::Vector2d velocity = getVelocity();
		velocity.x = (direction.x > 0) ? moveSpeed : -moveSpeed;
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

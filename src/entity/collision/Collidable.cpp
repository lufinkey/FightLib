
#include <fightlib/entity/collision/Collidable.hpp>

namespace fl
{
	Collidable::Collidable(const fgl::Vector2d& position)
		: previousPosition(position)
	{
		//
	}
	
	void Collidable::update(const fgl::ApplicationData& appData)
	{
		Sprite::update(appData);
		
		//offset for velocity
		shift(velocity*appData.getFrameSpeedMultiplier());
	}
	
	const fgl::Vector2d& Collidable::getPreviousPosition() const
	{
		return previousPosition;
	}
	
	void Collidable::setVelocity(const fgl::Vector2d& velocity_arg)
	{
		velocity = velocity_arg;
	}
	
	const fgl::Vector2d& Collidable::getVelocity() const
	{
		return velocity;
	}

	bool Collidable::respondsToCollision(Collidable* collided, CollisionSide side) const
	{
		return true;
	}
	
	void Collidable::onCollision(Collidable* collided, CollisionSide side)
	{
		//
	}
	
	void Collidable::onCollisionUpdate(Collidable* collided, CollisionSide side)
	{
		//
	}
	
	void Collidable::onCollisionFinish(Collidable* collided, CollisionSide side)
	{
		//
	}
	
	void Collidable::onFinishCollisionUpdates()
	{
		//
	}
}

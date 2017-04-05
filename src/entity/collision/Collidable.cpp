
#include <fightlib/entity/collision/Collidable.hpp>

namespace fl
{
	Collidable::Collidable(const fgl::Vector2d& position)
		: previousPosition(position)
	{
		//
	}
	
	const fgl::Vector2d& Collidable::getPreviousPosition() const
	{
		return previousPosition;
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


#include <fightlib/entity/collision/CollisionEvent.hpp>

namespace fl
{
	CollisionEvent::CollisionEvent(Collidable* collided, CollisionSide side)
		: collided(collided),
		side(side)
	{
		//
	}
	
	Collidable* CollisionEvent::getCollided() const
	{
		return collided;
	}
	
	CollisionSide CollisionEvent::getCollisionSide() const
	{
		return side;
	}
}

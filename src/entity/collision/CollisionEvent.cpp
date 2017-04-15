
#include <fightlib/entity/collision/CollisionEvent.hpp>

namespace fl
{
	const ActionEventType ACTIONEVENT_COLLISION = registerActionEventType();

	CollisionEvent::CollisionEvent(Collidable* collided, CollisionSide side)
		: collided(collided),
		side(side)
	{
		//
	}

	ActionEventType CollisionEvent::getEventType() const
	{
		return ACTIONEVENT_COLLISION;
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

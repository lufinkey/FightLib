
#include <fightlib/entity/collision/CollisionEvent.hpp>

namespace fl
{
	const EventType EVENT_COLLISION = registerEventType();

	CollisionEvent::CollisionEvent(Collidable* collided, CollisionSide side, CollisionState state)
		: collided(collided),
		side(side),
		state(state)
	{
		//
	}

	EventType CollisionEvent::getEventType() const
	{
		return EVENT_COLLISION;
	}
	
	Collidable* CollisionEvent::getCollided() const
	{
		return collided;
	}
	
	CollisionSide CollisionEvent::getCollisionSide() const
	{
		return side;
	}
	
	CollisionState CollisionEvent::getCollisionState() const
	{
		return state;
	}
}


#include <fightlib/entity/collision/CollisionEvent.hpp>

namespace fl
{
	const EventType EVENT_COLLISION = registerEventType();

	CollisionEvent::CollisionEvent(Collidable* collided, CollisionState state, const fgl::ArrayList<CollisionData>& collisions)
		: collided(collided),
		state(state),
		collisions(collisions)
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
	
	CollisionState CollisionEvent::getCollisionState() const
	{
		return state;
	}

	const fgl::ArrayList<CollisionData>& CollisionEvent::getCollisions() const
	{
		return collisions;
	}
}

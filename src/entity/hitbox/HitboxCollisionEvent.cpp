
#include <fightlib/entity/hitbox/HitboxCollisionEvent.hpp>

namespace fl
{
	const EventType EVENT_HITBOXCOLLISION = registerEventType();

	HitboxCollisionEvent::HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions)
		: hitEntity(hitEntity),
		hitboxCollisions(hitboxCollisions),
		previousHitboxCollisions(previousHitboxCollisions)
	{
		//
	}

	EventType HitboxCollisionEvent::getEventType() const
	{
		return EVENT_HITBOXCOLLISION;
	}
	
	Entity* HitboxCollisionEvent::getHitEntity() const
	{
		return hitEntity;
	}
	
	const fgl::ArrayList<HitboxCollision>& HitboxCollisionEvent::getHitboxCollisions() const
	{
		return hitboxCollisions;
	}

	const fgl::ArrayList<HitboxCollision>& HitboxCollisionEvent::getPreviousHitboxCollisions() const
	{
		return previousHitboxCollisions;
	}
}

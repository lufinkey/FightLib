
#include <fightlib/entity/hitbox/HitboxCollisionEvent.hpp>

namespace fl
{
	const fgl::EventType EVENT_HITBOXCOLLISION = fgl::registerEventType();

	HitboxCollisionEvent::HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions)
		: hitEntity(hitEntity),
		hitboxCollisions(hitboxCollisions),
		previousHitboxCollisions(previousHitboxCollisions)
	{
		//
	}

	fgl::EventType HitboxCollisionEvent::getEventType() const
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

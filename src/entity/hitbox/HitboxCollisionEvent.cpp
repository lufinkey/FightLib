
#include <fightlib/entity/hitbox/HitboxCollisionEvent.hpp>

namespace fl
{
	HitboxCollisionEvent::HitboxCollisionEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* collidedEntity, const TaggedBox& collidedBounds)
		: hitbox(hitbox),
		hitboxInfo(hitboxInfo),
		collidedEntity(collidedEntity),
		collidedBounds(collidedBounds)
	{
		//
	}
	
	const TaggedBox& HitboxCollisionEvent::getHitbox() const
	{
		return hitbox;
	}
	
	const HitboxInfo& HitboxCollisionEvent::getHitboxInfo() const
	{
		return hitboxInfo;
	}
	
	Entity* HitboxCollisionEvent::getCollidedEntity() const
	{
		return collidedEntity;
	}
	
	const TaggedBox& HitboxCollisionEvent::getCollidedBounds() const
	{
		return collidedBounds;
	}
}

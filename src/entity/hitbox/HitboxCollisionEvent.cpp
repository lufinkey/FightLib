
#include <fightlib/entity/hitbox/HitboxCollisionEvent.hpp>

namespace fl
{
	HitboxCollisionEvent::HitboxCollisionEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* collidedEntity, CollisionRect* collidedRect)
		: hitbox(hitbox),
		hitboxInfo(hitboxInfo),
		collidedEntity(collidedEntity),
		collidedRect(collidedRect)
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
	
	CollisionRect* HitboxCollisionEvent::getCollidedRect() const
	{
		return collidedRect;
	}
}

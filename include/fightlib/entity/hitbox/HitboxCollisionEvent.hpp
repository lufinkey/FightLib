
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include <fightlib/entity/collision/rects/CollisionRect.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class Entity;
	
	class HitboxCollisionEvent
	{
	public:
		HitboxCollisionEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* collidedEntity, CollisionRect* collidedRect);
		
		const TaggedBox& getHitbox() const;
		const HitboxInfo& getHitboxInfo() const;
		
		Entity* getCollidedEntity() const;
		CollisionRect* getCollidedRect() const;
		
	private:
		TaggedBox hitbox;
		HitboxInfo hitboxInfo;
		
		Entity* collidedEntity;
		CollisionRect* collidedRect;
	};
}

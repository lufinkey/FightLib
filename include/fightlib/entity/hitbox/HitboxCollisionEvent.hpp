
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class Entity;
	
	class HitboxCollisionEvent
	{
	public:
		HitboxCollisionEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* collidedEntity, const TaggedBox& collidedBounds);
		
		const TaggedBox& getHitbox() const;
		const HitboxInfo& getHitboxInfo() const;
		
		Entity* getCollidedEntity() const;
		const TaggedBox& getCollidedBounds() const;
		
	private:
		TaggedBox hitbox;
		HitboxInfo hitboxInfo;
		
		Entity* collidedEntity;
		TaggedBox collidedBounds;
	};
}

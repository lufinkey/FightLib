
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class Entity;
	
	class HitboxClashEvent
	{
	public:
		HitboxClashEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* clashedEntity, const TaggedBox& clashedHitbox, const HitboxInfo& clashedHitboxInfo);
		
		const TaggedBox& getHitbox() const;
		const HitboxInfo& getHitboxInfo() const;
		
		Entity* getClashedEntity() const;
		const TaggedBox& getClashedHitbox() const;
		const HitboxInfo& getClashedHitboxInfo() const;
		
	private:
		TaggedBox hitbox;
		HitboxInfo hitboxInfo;
		
		Entity* clashedEntity;
		TaggedBox clashedHitbox;
		HitboxInfo clashedHitboxInfo;
	};
}

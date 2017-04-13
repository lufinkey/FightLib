
#include <fightlib/entity/hitbox/HitboxClashEvent.hpp>

namespace fl
{
	HitboxClashEvent::HitboxClashEvent(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, Entity* clashedEntity, const TaggedBox& clashedHitbox, const HitboxInfo& clashedHitboxInfo)
		: hitbox(hitbox),
		hitboxInfo(hitboxInfo),
		clashedEntity(clashedEntity),
		clashedHitbox(clashedHitbox),
		clashedHitboxInfo(clashedHitboxInfo)
	{
		//
	}
	
	const TaggedBox& HitboxClashEvent::getHitbox() const
	{
		return hitbox;
	}
	
	const HitboxInfo& HitboxClashEvent::getHitboxInfo() const
	{
		return hitboxInfo;
	}
	
	Entity* HitboxClashEvent::getClashedEntity() const
	{
		return clashedEntity;
	}
	
	const TaggedBox& HitboxClashEvent::getClashedHitbox() const
	{
		return clashedHitbox;
	}
	
	const HitboxInfo& HitboxClashEvent::getClashedHitboxInfo() const
	{
		return clashedHitboxInfo;
	}
}

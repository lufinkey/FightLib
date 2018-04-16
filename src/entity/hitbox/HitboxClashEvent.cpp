
#include <fightlib/entity/hitbox/HitboxClashEvent.hpp>

namespace fl
{
	const fgl::EventType EVENT_HITBOXCLASH = fgl::registerEventType();

	HitboxClashEvent::HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes)
		: clashedEntity(clashedEntity),
		hitboxClashes(hitboxClashes),
		previousHitboxClashes(previousHitboxClashes)
	{
		//
	}

	fgl::EventType HitboxClashEvent::getEventType() const
	{
		return EVENT_HITBOXCLASH;
	}
	
	Entity* HitboxClashEvent::getClashedEntity() const
	{
		return clashedEntity;
	}
	
	const fgl::ArrayList<HitboxClash>& HitboxClashEvent::getHitboxClashes() const
	{
		return hitboxClashes;
	}
	
	const fgl::ArrayList<HitboxClash>& HitboxClashEvent::getPreviousHitboxClashes() const
	{
		return previousHitboxClashes;
	}
}

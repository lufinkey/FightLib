
#include <fightlib/entity/hitbox/HitboxClashEvent.hpp>

namespace fl
{
	const EventType EVENT_HITBOXCLASH = registerEventType();

	HitboxClashEvent::HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes)
		: clashedEntity(clashedEntity),
		hitboxClashes(hitboxClashes),
		previousHitboxClashes(previousHitboxClashes)
	{
		//
	}

	EventType HitboxClashEvent::getEventType() const
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

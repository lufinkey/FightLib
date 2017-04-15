
#include <fightlib/entity/hitbox/HitboxClashEvent.hpp>

namespace fl
{
	const ActionEventType ACTIONEVENT_HITBOXCLASH = registerActionEventType();

	HitboxClashEvent::HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes)
		: clashedEntity(clashedEntity),
		hitboxClashes(hitboxClashes),
		previousHitboxClashes(previousHitboxClashes)
	{
		//
	}

	ActionEventType HitboxClashEvent::getEventType() const
	{
		return ACTIONEVENT_HITBOXCLASH;
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

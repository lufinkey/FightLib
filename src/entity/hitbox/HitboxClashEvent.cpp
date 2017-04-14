
#include <fightlib/entity/hitbox/HitboxClashEvent.hpp>

namespace fl
{
	HitboxClashEvent::HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxPair>& clashedPairs, const fgl::ArrayList<HitboxPair>& previousClashedPairs)
		: clashedEntity(clashedEntity),
		clashedPairs(clashedPairs),
		previousClashedPairs(previousClashedPairs)
	{
		//
	}
	
	Entity* HitboxClashEvent::getClashedEntity() const
	{
		return clashedEntity;
	}
	
	const fgl::ArrayList<HitboxPair>& HitboxClashEvent::getClashedHitboxPairs() const
	{
		return clashedPairs;
	}
	
	const fgl::ArrayList<HitboxPair>& HitboxClashEvent::getPreviousClashedHitboxPairs() const
	{
		return previousClashedPairs;
	}
}

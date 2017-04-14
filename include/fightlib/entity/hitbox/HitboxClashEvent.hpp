
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxPair.hpp"

namespace fl
{
	class Entity;
	
	class HitboxClashEvent
	{
	public:
		HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxPair>& clashedPairs, const fgl::ArrayList<HitboxPair>& previousClashedPairs);
		
		Entity* getClashedEntity() const;
		
		const fgl::ArrayList<HitboxPair>& getClashedHitboxPairs() const;
		const fgl::ArrayList<HitboxPair>& getPreviousClashedHitboxPairs() const;
		
	private:
		Entity* clashedEntity;
		fgl::ArrayList<HitboxPair> clashedPairs;
		fgl::ArrayList<HitboxPair> previousClashedPairs;
	};
}

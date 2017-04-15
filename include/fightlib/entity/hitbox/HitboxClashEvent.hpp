
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxClash.hpp"

namespace fl
{
	class Entity;
	
	class HitboxClashEvent
	{
	public:
		HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes);
		
		Entity* getClashedEntity() const;
		
		const fgl::ArrayList<HitboxClash>& getHitboxClashes() const;
		const fgl::ArrayList<HitboxClash>& getPreviousHitboxClashes() const;
		
	private:
		Entity* clashedEntity;
		fgl::ArrayList<HitboxClash> hitboxClashes;
		fgl::ArrayList<HitboxClash> previousHitboxClashes;
	};
}

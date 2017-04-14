
#pragma once

#include "HitboxPair.hpp"

namespace fl
{
	class Entity;
	
	class HitboxClashPair
	{
	public:
		HitboxClashPair(Entity* entity1, Entity* entity2, const fgl::ArrayList<HitboxPair>& hitboxPairs);
		
		Entity* getFirstEntity() const;
		Entity* getSecondEntity() const;
		
		const fgl::ArrayList<HitboxPair>& getHitboxPairs() const;
		fgl::ArrayList<HitboxPair> getFlippedHitboxPairs() const;
		size_t getPriorityHitboxPairIndex() const;
		const HitboxPair& getPriorityHitboxPair() const;
		
	private:
		Entity* entity1;
		Entity* entity2;
		fgl::ArrayList<HitboxPair> hitboxPairs;
		size_t priorityPairIndex;
	};
}


#pragma once

#include "HitboxClash.hpp"

namespace fl
{
	class Entity;
	
	class HitboxClashPair
	{
	public:
		HitboxClashPair(Entity* entity1, Entity* entity2, const fgl::ArrayList<HitboxClash>& hitboxClashes);
		
		Entity* getFirstEntity() const;
		Entity* getSecondEntity() const;
		
		const fgl::ArrayList<HitboxClash>& getHitboxClashes() const;
		fgl::ArrayList<HitboxClash> getFlippedHitboxClashes() const;
		size_t getPriorityHitboxClashIndex() const;
		const HitboxClash& getPriorityHitboxClash() const;
		
	private:
		Entity* entity1;
		Entity* entity2;
		fgl::ArrayList<HitboxClash> hitboxClashes;
		size_t priorityClashIndex;
	};
}


#include <fightlib/entity/hitbox/HitboxClashPair.hpp>

namespace fl
{
	HitboxClashPair::HitboxClashPair(Entity* entity1, Entity* entity2, const fgl::ArrayList<HitboxPair>& hitboxPairs)
		: entity1(entity1),
		entity2(entity2),
		hitboxPairs(hitboxPairs),
		priorityPairIndex(-1)
	{
		if(hitboxPairs.size()==0)
		{
			throw fgl::IllegalArgumentException("hitboxPairs", "cannot be empty");
		}
		
		double topPriorityDiff = 0;
		double lastTopPriority = 0;
		bool secondIsHighest = false;
		for(size_t k=0; k<hitboxPairs.size(); k++)
		{
			auto& pair = hitboxPairs[k];
			float priorityDiff = fgl::Math::abs(pair.hitboxInfo1.getPriority() - pair.hitboxInfo2.getPriority());
			double topPriority = fgl::Math::max(pair.hitboxInfo1.getPriority(), pair.hitboxInfo2.getPriority());
			if(priorityPairIndex==-1 || priorityDiff > topPriorityDiff || (priorityDiff==topPriorityDiff && topPriority > lastTopPriority))
			{
				//set the new top priority hitbox collision
				priorityPairIndex = k;
				topPriorityDiff = priorityDiff;
				lastTopPriority = topPriority;
			}
		}
	}
	
	Entity* HitboxClashPair::getFirstEntity() const
	{
		return entity1;
	}
	
	Entity* HitboxClashPair::getSecondEntity() const
	{
		return entity2;
	}
	
	const fgl::ArrayList<HitboxPair>& HitboxClashPair::getHitboxPairs() const
	{
		return hitboxPairs;
	}
	
	fgl::ArrayList<HitboxPair> HitboxClashPair::getFlippedHitboxPairs() const
	{
		fgl::ArrayList<HitboxPair> flippedPairs;
		flippedPairs.reserve(hitboxPairs.size());
		for(auto& pair : hitboxPairs)
		{
			flippedPairs.add(pair.flipped());
		}
		return flippedPairs;
	}
	
	size_t HitboxClashPair::getPriorityHitboxPairIndex() const
	{
		return priorityPairIndex;
	}
	
	const HitboxPair& HitboxClashPair::getPriorityHitboxPair() const
	{
		return hitboxPairs[priorityPairIndex];
	}
}

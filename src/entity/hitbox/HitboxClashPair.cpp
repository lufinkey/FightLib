
#include <fightlib/entity/hitbox/HitboxClashPair.hpp>

namespace fl
{
	HitboxClashPair::HitboxClashPair(Entity* entity1, Entity* entity2, const fgl::ArrayList<HitboxClash>& hitboxClashes)
		: entity1(entity1),
		entity2(entity2),
		hitboxClashes(hitboxClashes),
		priorityClashIndex(-1)
	{
		if(hitboxClashes.size()==0)
		{
			throw fgl::IllegalArgumentException("hitboxClashes", "cannot be empty");
		}
		
		double topPriorityDiff = 0;
		double lastTopPriority = 0;
		for(size_t k=0; k<hitboxClashes.size(); k++)
		{
			auto& clash = hitboxClashes[k];
			float priorityDiff = fgl::Math::abs(clash.hitboxInfo1.getPriority() - clash.hitboxInfo2.getPriority());
			double topPriority = fgl::Math::max(clash.hitboxInfo1.getPriority(), clash.hitboxInfo2.getPriority());
			if(priorityClashIndex==-1 || priorityDiff > topPriorityDiff || (priorityDiff==topPriorityDiff && topPriority > lastTopPriority))
			{
				//set the new top priority hitbox collision
				priorityClashIndex = k;
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
	
	const fgl::ArrayList<HitboxClash>& HitboxClashPair::getHitboxClashes() const
	{
		return hitboxClashes;
	}
	
	fgl::ArrayList<HitboxClash> HitboxClashPair::getFlippedHitboxClashes() const
	{
		fgl::ArrayList<HitboxClash> flippedClashes;
		flippedClashes.reserve(hitboxClashes.size());
		for(auto& clash : hitboxClashes)
		{
			flippedClashes.add(clash.flipped());
		}
		return flippedClashes;
	}
	
	size_t HitboxClashPair::getPriorityHitboxClashIndex() const
	{
		return priorityClashIndex;
	}
	
	const HitboxClash& HitboxClashPair::getPriorityHitboxClash() const
	{
		return hitboxClashes[priorityClashIndex];
	}
}

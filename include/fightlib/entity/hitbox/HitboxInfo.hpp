
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class AngleRange
	{
	public:
		AngleRange(float min, float max);
		
		bool contains(float angle) const;
		
		float min;
		float max;
	};
	
	class HitboxInfo
	{
	public:
		HitboxInfo(AngleRange effectiveRange, float priority);
		
		AngleRange getEffectiveAngleRange() const;
		float getPriority() const;
		
	private:
		AngleRange effectiveAngleRange;
		float priority;
	};
	
	const HitboxInfo NULL_HITBOX_INFO = HitboxInfo(AngleRange(0, 0), 0);
}


#include <fightlib/entity/hitbox/HitboxInfo.hpp>

namespace fl
{
	AngleRange::AngleRange(float min, float max)
		: min(min), max(max)
	{
		if(min < 0 || min > 360)
		{
			throw fgl::IllegalArgumentException("min", "must be in the range [0 - 360]");
		}
		if(max < 0 || max > 360)
		{
			throw fgl::IllegalArgumentException("max", "must be in the range [0 - 360]");
		}
	}
	
	bool AngleRange::contains(float angle) const
	{
		if(min==max)
		{
			return false;
		}
		angle = fgl::Math::normalizeDegrees(angle);
		if(min < max)
		{
			if(angle >= min && angle < max)
			{
				return true;
			}
		}
		else
		{
			if(angle >= min)
			{
				return true;
			}
			else if(angle < max)
			{
				return true;
			}
		}
		return false;
	}
	
	HitboxInfo::HitboxInfo(AngleRange effectiveRange, float priority)
		: effectiveAngleRange(effectiveRange),
		priority(priority)
	{
		//
	}
	
	AngleRange HitboxInfo::getEffectiveAngleRange() const
	{
		return effectiveAngleRange;
	}
	
	float HitboxInfo::getPriority() const
	{
		return priority;
	}
}

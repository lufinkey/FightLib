
#include <fightlib/entity/hitbox/HitboxClash.hpp>

namespace fl
{
	HitboxClash::HitboxClash(const TaggedBox& hitbox1, const HitboxInfo& info1, const TaggedBox& hitbox2, const HitboxInfo& info2)
		: hitbox1(hitbox1),
		hitboxInfo1(info1),
		hitbox2(hitbox2),
		hitboxInfo2(info2)
	{
		//
	}
	
	HitboxClash HitboxClash::flipped() const
	{
		return HitboxClash(hitbox2, hitboxInfo2, hitbox1, hitboxInfo1);
	}
}

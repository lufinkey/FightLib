
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class HitboxPair
	{
	public:
		HitboxPair(const TaggedBox& hitbox1, const HitboxInfo& info1, const TaggedBox& hitbox2, const HitboxInfo& info2);
		
		HitboxPair flipped() const;
		
		TaggedBox hitbox1;
		HitboxInfo hitboxInfo1;
		TaggedBox hitbox2;
		HitboxInfo hitboxInfo2;
	};
}

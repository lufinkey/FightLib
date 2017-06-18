
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class HitboxCollision
	{
	public:
		HitboxCollision(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, size_t hitPolygonIndex);

		TaggedBox hitbox;
		HitboxInfo hitboxInfo;
		size_t hitPolygonIndex;
	};
}

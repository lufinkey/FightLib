
#pragma once

#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include <fightlib/entity/collision/rects/CollisionRect.hpp>
#include "HitboxInfo.hpp"

namespace fl
{
	class HitboxCollision
	{
	public:
		HitboxCollision(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, const fgl::String& hitCollisionRectTag);

		TaggedBox hitbox;
		HitboxInfo hitboxInfo;
		fgl::String hitCollisionRectTag;
	};
}

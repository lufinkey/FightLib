
#pragma once

#include <GameLibrary/GameLibrary.hpp>
#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
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

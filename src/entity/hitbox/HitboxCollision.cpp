
#include <fightlib/entity/hitbox/HitboxCollision.hpp>

namespace fl
{
	HitboxCollision::HitboxCollision(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, const fgl::String& hitCollisionRectTag)
		: hitbox(hitbox),
		hitboxInfo(hitboxInfo),
		hitCollisionRectTag(hitCollisionRectTag)
	{
		//
	}
}

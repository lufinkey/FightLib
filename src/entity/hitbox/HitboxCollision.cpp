
#include <fightlib/entity/hitbox/HitboxCollision.hpp>

namespace fl
{
	HitboxCollision::HitboxCollision(const TaggedBox& hitbox, const HitboxInfo& hitboxInfo, size_t hitPolygonIndex)
		: hitbox(hitbox),
		hitboxInfo(hitboxInfo),
		hitPolygonIndex(hitPolygonIndex)
	{
		//
	}
}

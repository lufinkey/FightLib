
#include <fightlib/entity/hitbox/HitboxCollisionPair.hpp>
#include <fightlib/entity/Entity.hpp>

namespace fl
{
	HitboxCollisionPair::HitboxCollisionPair(Entity* hittingEntity, Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions)
		: hittingEntity(hittingEntity),
		hitEntity(hitEntity),
		hitboxCollisions(hitboxCollisions)
	{
		//
	}

	Entity* HitboxCollisionPair::getHittingEntity() const
	{
		return hittingEntity;
	}

	Entity* HitboxCollisionPair::getHitEntity() const
	{
		return hitEntity;
	}

	const fgl::ArrayList<HitboxCollision>& HitboxCollisionPair::getHitboxCollisions() const
	{
		return hitboxCollisions;
	}
}

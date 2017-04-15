
#pragma once

#include "HitboxCollision.hpp"

namespace fl
{
	class Entity;

	class HitboxCollisionPair
	{
	public:
		HitboxCollisionPair(Entity* hittingEntity, Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions);

		Entity* getHittingEntity() const;
		Entity* getHitEntity() const;
		const fgl::ArrayList<HitboxCollision>& getHitboxCollisions() const;
		
	private:
		Entity* hittingEntity;
		Entity* hitEntity;
		fgl::ArrayList<HitboxCollision> hitboxCollisions;
	};
}


#pragma once

#include "HitboxCollision.hpp"

namespace fl
{
	class Entity;
	
	class HitboxCollisionEvent
	{
	public:
		HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions);
		
		Entity* getHitEntity() const;
		const fgl::ArrayList<HitboxCollision>& getHitboxCollisions() const;
		const fgl::ArrayList<HitboxCollision>& getPreviousHitboxCollisions() const;
		
	private:
		Entity* hitEntity;
		fgl::ArrayList<HitboxCollision> hitboxCollisions;
		fgl::ArrayList<HitboxCollision> previousHitboxCollisions;
	};
}

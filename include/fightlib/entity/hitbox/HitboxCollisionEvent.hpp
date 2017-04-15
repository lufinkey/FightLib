
#pragma once

#include <fightlib/base/Event.hpp>
#include "HitboxCollision.hpp"

namespace fl
{
	extern const EventType EVENT_HITBOXCOLLISION;

	class Entity;
	
	class HitboxCollisionEvent : public Event
	{
	public:
		HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions);

		virtual EventType getEventType() const override;
		
		Entity* getHitEntity() const;
		const fgl::ArrayList<HitboxCollision>& getHitboxCollisions() const;
		const fgl::ArrayList<HitboxCollision>& getPreviousHitboxCollisions() const;
		
	private:
		Entity* hitEntity;
		fgl::ArrayList<HitboxCollision> hitboxCollisions;
		fgl::ArrayList<HitboxCollision> previousHitboxCollisions;
	};
}

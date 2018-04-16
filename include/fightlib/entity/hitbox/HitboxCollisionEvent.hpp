
#pragma once

#include <GameLibrary/GameLibrary.hpp>
#include "HitboxCollision.hpp"

namespace fl
{
	extern const fgl::EventType EVENT_HITBOXCOLLISION;

	class Entity;
	
	class HitboxCollisionEvent : public fgl::Event
	{
	public:
		HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions);

		virtual fgl::EventType getEventType() const override;
		
		Entity* getHitEntity() const;
		const fgl::ArrayList<HitboxCollision>& getHitboxCollisions() const;
		const fgl::ArrayList<HitboxCollision>& getPreviousHitboxCollisions() const;
		
	private:
		Entity* hitEntity;
		fgl::ArrayList<HitboxCollision> hitboxCollisions;
		fgl::ArrayList<HitboxCollision> previousHitboxCollisions;
	};
}


#pragma once

#include <fightlib/entity/action/ActionEvent.hpp>
#include "HitboxCollision.hpp"

namespace fl
{
	extern const ActionEventType ACTIONEVENT_HITBOXCOLLISION;

	class Entity;
	
	class HitboxCollisionEvent : public ActionEvent
	{
	public:
		HitboxCollisionEvent(Entity* hitEntity, const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions);

		virtual ActionEventType getEventType() const override;
		
		Entity* getHitEntity() const;
		const fgl::ArrayList<HitboxCollision>& getHitboxCollisions() const;
		const fgl::ArrayList<HitboxCollision>& getPreviousHitboxCollisions() const;
		
	private:
		Entity* hitEntity;
		fgl::ArrayList<HitboxCollision> hitboxCollisions;
		fgl::ArrayList<HitboxCollision> previousHitboxCollisions;
	};
}

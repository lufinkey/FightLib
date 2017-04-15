
#pragma once

#include <fightlib/entity/action/ActionEvent.hpp>
#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxClash.hpp"

namespace fl
{
	extern const ActionEventType ACTIONEVENT_HITBOXCLASH;

	class Entity;
	
	class HitboxClashEvent : public ActionEvent
	{
	public:
		HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes);

		virtual ActionEventType getEventType() const override;
		
		Entity* getClashedEntity() const;
		
		const fgl::ArrayList<HitboxClash>& getHitboxClashes() const;
		const fgl::ArrayList<HitboxClash>& getPreviousHitboxClashes() const;
		
	private:
		Entity* clashedEntity;
		fgl::ArrayList<HitboxClash> hitboxClashes;
		fgl::ArrayList<HitboxClash> previousHitboxClashes;
	};
}

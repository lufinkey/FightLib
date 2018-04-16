
#pragma once

#include <GameLibrary/GameLibrary.hpp>
#include <fightlib/entity/draw/AnimationMetaPoint.hpp>
#include "HitboxClash.hpp"

namespace fl
{
	extern const fgl::EventType EVENT_HITBOXCLASH;

	class Entity;
	
	class HitboxClashEvent : public fgl::Event
	{
	public:
		HitboxClashEvent(Entity* clashedEntity, const fgl::ArrayList<HitboxClash>& hitboxClashes, const fgl::ArrayList<HitboxClash>& previousHitboxClashes);

		virtual fgl::EventType getEventType() const override;
		
		Entity* getClashedEntity() const;
		
		const fgl::ArrayList<HitboxClash>& getHitboxClashes() const;
		const fgl::ArrayList<HitboxClash>& getPreviousHitboxClashes() const;
		
	private:
		Entity* clashedEntity;
		fgl::ArrayList<HitboxClash> hitboxClashes;
		fgl::ArrayList<HitboxClash> previousHitboxClashes;
	};
}


#pragma once

#include "Action.hpp"
#include <fightlib/entity/hitbox/HitboxInfo.hpp>

namespace fl
{
	class AttackAction : public Action
	{
	public:
		virtual bool getFlag(const fgl::String& flag) const override;
		
		virtual HitboxInfo getHitboxInfo(size_t tag) const;
	};
}

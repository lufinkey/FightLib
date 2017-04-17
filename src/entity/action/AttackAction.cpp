
#include <fightlib/entity/action/AttackAction.hpp>

namespace fl
{
	bool AttackAction::getFlag(const fgl::String& flag) const
	{
		if(flag=="AttackAction")
		{
			return true;
		}
		return Action::getFlag(flag);
	}
	
	HitboxInfo AttackAction::getHitboxInfo(size_t tag) const
	{
		return NULL_HITBOX_INFO;
	}
}

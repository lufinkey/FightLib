
#pragma once

#include <fightlib/fightlib.hpp>

class PunchAttack : public fl::AttackAction
{
public:
	virtual void onPerform(fl::ActionParamsPtr params) override;
	
	virtual fl::HitboxInfo getHitboxInfo(size_t tag) const override;
	
	virtual void onEvent(fl::EventPtr event) override;
};


#pragma once

#include <fightlib/fightlib.hpp>

class JumpAction : public fl::Action
{
protected:
	virtual bool getFlag(const fgl::String& name) const override;
	
	virtual void onPerform(fl::ActionEntity* entity, fl::ActionParamsPtr params) override;
};

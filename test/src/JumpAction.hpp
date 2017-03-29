
#pragma once

#include <fightlib/fightlib.hpp>

class JumpAction : public fl::Action
{
public:
	virtual bool getFlag(const fgl::String& name) const override;

protected:
	virtual void onPerform(fl::ActionParamsPtr params) override;
	virtual void onEvent(fl::ActionEventPtr event) override;
};

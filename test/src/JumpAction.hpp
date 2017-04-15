
#pragma once

#include <fightlib/fightlib.hpp>

class JumpAction : public fl::Action
{
public:
	JumpAction();
	
	virtual bool getFlag(const fgl::String& name) const override;

protected:
	virtual void onPerform(fl::ActionParamsPtr params) override;
	virtual void onUpdate(const fgl::ApplicationData& appData) override;
	virtual void onEvent(fl::EventPtr event) override;
	
private:
	bool doubleJumped;
};


#pragma once

#include <fightlib/fightlib.hpp>

class PickUpItemAction : public fl::Action
{
protected:
	virtual void onPerform(fl::ActionParamsPtr params) override;
};


#pragma once

#include <fightlib/fightlib.hpp>

class HurtAction : public fl::Action
{
public:
	virtual void onPerform(fl::ActionParamsPtr params) override;
};

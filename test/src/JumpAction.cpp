
#include "JumpAction.hpp"

bool JumpAction::getFlag(const fgl::String& flag) const
{
	if(flag==fl::ACTIONFLAG_ALLOWMOVEMENT)
	{
		return true;
	}
	return false;
}

void JumpAction::onPerform(fl::ActionParamsPtr params)
{
	fl::Character* character = (fl::Character*)getEntity();
	auto velocity = character->getVelocity();
	velocity.y = -600;
	character->setVelocity(velocity);
	if(character->isOnGround())
	{
		character->changeAnimation("jump", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end();
			}
		});
	}
	else
	{
		character->changeAnimation("jump2", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end();
			}
		});
	}
}

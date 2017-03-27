
#include "JumpAction.hpp"

bool JumpAction::getFlag(const fgl::String& flag) const
{
	if(flag=="AllowMovement")
	{
		return true;
	}
	return false;
}

void JumpAction::onPerform(fl::ActionEntity* entity, fl::ActionParamsPtr params)
{
	auto velocity = entity->getVelocity();
	velocity.y = -600;
	entity->setVelocity(velocity);
	entity->changeAnimation("jump", [=](fl::AnimationEventType event){
		if(event==fl::ANIMATIONEVENT_FINISHED)
		{
			end(entity);
		}
	});
}

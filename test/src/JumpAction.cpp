
#include "JumpAction.hpp"

bool JumpAction::getFlag(const fgl::String& flag) const
{
	if(flag==fl::ACTIONFLAG_ALLOWMOVEMENT)
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
	fl::Character* character = (fl::Character*)entity;
	if(character->isOnGround())
	{
		entity->changeAnimation("jump", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end(entity);
			}
		});
	}
	else
	{
		entity->changeAnimation("jump2", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end(entity);
			}
		});
	}
}

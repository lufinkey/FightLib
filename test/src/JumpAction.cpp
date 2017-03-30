
#include "JumpAction.hpp"

JumpAction::JumpAction()
	: doubleJumped(true)
{
	//
}

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
	if(character->isOnGround())
	{
		auto velocity = character->getVelocity();
		velocity.y = -600;
		character->setVelocity(velocity);
		
		character->changeAnimation("jump", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end();
			}
		});
	}
	else if(!doubleJumped)
	{
		auto velocity = character->getVelocity();
		velocity.y = -600;
		character->setVelocity(velocity);
		
		doubleJumped = true;
		character->changeAnimation("jump2", [=](fl::AnimationEventType event){
			if(event==fl::ANIMATIONEVENT_FINISHED)
			{
				end();
			}
		});
	}
	else
	{
		end();
	}
}

void JumpAction::onUpdate(const fgl::ApplicationData& appData)
{
	fl::Character* character = (fl::Character*)getEntity();
	if(character->isOnGround())
	{
		doubleJumped = false;
	}
}

void JumpAction::onEvent(fl::ActionEventPtr event)
{
	if(isPerforming())
	{
		if(event->getEventType()==fl::ACTIONEVENT_INTERRUPT)
		{
			auto interruptEvent = std::static_pointer_cast<fl::ActionInterruptEvent>(event);
			if(interruptEvent->getInterruptingAction()==this)
			{
				if(!doubleJumped)
				{
					end();
				}
			}
		}
	}
}

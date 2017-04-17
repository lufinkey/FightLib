
#include "PunchAttack.hpp"

void PunchAttack::onPerform(fl::ActionParamsPtr params)
{
	getEntity()->setVelocity(fgl::Vector2d(0, getEntity()->getVelocity().y));
	getEntity()->changeAnimation("punch", [=](fl::AnimationEventType animEvent){
		if(animEvent==fl::ANIMATIONEVENT_FINISHED)
		{
			end();
		}
	});
}

fl::HitboxInfo PunchAttack::getHitboxInfo(size_t tag) const
{
	switch(getEntity()->getOrientation())
	{
		case fl::ORIENTATION_LEFT:
			return fl::HitboxInfo(fl::AngleRange(90, 270), 1.0);
			
		case fl::ORIENTATION_RIGHT:
			return fl::HitboxInfo(fl::AngleRange(270, 90), 1.0);
	}
	return AttackAction::getHitboxInfo(tag);
}

void PunchAttack::onEvent(fl::EventPtr event)
{
	if(event->getEventType()==fl::EVENT_HITBOXCOLLISION)
	{
		auto hitboxEvent = std::static_pointer_cast<fl::HitboxCollisionEvent>(event);
		if(hitboxEvent->getPreviousHitboxCollisions().size()==0)
		{
			static_cast<fl::ActionEntity*>(hitboxEvent->getHitEntity())->performAction("hurt");
		}
	}
}

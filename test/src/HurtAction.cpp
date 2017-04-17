
#include "HurtAction.hpp"

void HurtAction::onPerform(fl::ActionParamsPtr params)
{
	getEntity()->changeAnimation("hit-front", [=](fl::AnimationEventType animEvent){
		if(animEvent==fl::ANIMATIONEVENT_FINISHED)
		{
			end();
		}
	});
}

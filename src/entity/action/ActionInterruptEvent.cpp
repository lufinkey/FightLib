
#include <fightlib/entity/action/ActionInterruptEvent.hpp>

namespace fl
{
	const fgl::EventType EVENT_ACTIONINTERRUPT = fgl::registerEventType();
	
	ActionInterruptEvent::ActionInterruptEvent(Action* interruptingAction)
		: interruptingAction(interruptingAction)
	{
		//
	}

	fgl::EventType ActionInterruptEvent::getEventType() const
	{
		return EVENT_ACTIONINTERRUPT;
	}
	
	Action* ActionInterruptEvent::getInterruptingAction() const
	{
		return interruptingAction;
	}
}

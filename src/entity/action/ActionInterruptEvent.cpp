
#include <fightlib/entity/action/ActionInterruptEvent.hpp>

namespace fl
{
	const EventType EVENT_ACTIONINTERRUPT = registerEventType();
	
	ActionInterruptEvent::ActionInterruptEvent(Action* interruptingAction)
		: interruptingAction(interruptingAction)
	{
		//
	}

	EventType ActionInterruptEvent::getEventType() const
	{
		return EVENT_ACTIONINTERRUPT;
	}
	
	Action* ActionInterruptEvent::getInterruptingAction() const
	{
		return interruptingAction;
	}
}

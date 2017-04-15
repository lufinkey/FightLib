
#include <fightlib/entity/action/ActionInterruptEvent.hpp>

namespace fl
{
	const ActionEventType ACTIONEVENT_INTERRUPT = registerActionEventType();
	
	ActionInterruptEvent::ActionInterruptEvent(Action* interruptingAction)
		: interruptingAction(interruptingAction)
	{
		//
	}

	ActionEventType ActionInterruptEvent::getEventType() const
	{
		return ACTIONEVENT_INTERRUPT;
	}
	
	Action* ActionInterruptEvent::getInterruptingAction() const
	{
		return interruptingAction;
	}
}

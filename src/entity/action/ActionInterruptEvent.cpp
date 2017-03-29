
#include <fightlib/entity/action/ActionInterruptEvent.hpp>

namespace fl
{
	const ActionEventType ACTIONEVENT_INTERRUPT = registerActionEventType();
	
	ActionInterruptEvent::ActionInterruptEvent(Action* interruptingAction)
		: ActionEvent(ACTIONEVENT_INTERRUPT),
		interruptingAction(interruptingAction)
	{
		//
	}
	
	Action* ActionInterruptEvent::getInterruptingAction() const
	{
		return interruptingAction;
	}
}

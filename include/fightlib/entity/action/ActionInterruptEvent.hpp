
#pragma once

#include "ActionEvent.hpp"
#include "Action.hpp"

namespace fl
{
	extern const ActionEventType ACTIONEVENT_INTERRUPT;
	
	class ActionInterruptEvent : public ActionEvent
	{
	public:
		ActionInterruptEvent(Action* interruptingAction);
		
		Action* getInterruptingAction() const;
		
	private:
		Action* interruptingAction;
	};
}

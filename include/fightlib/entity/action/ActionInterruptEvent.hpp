
#pragma once

#include <fightlib/base/Event.hpp>
#include "Action.hpp"

namespace fl
{
	extern const EventType EVENT_ACTIONINTERRUPT;
	
	class ActionInterruptEvent : public Event
	{
	public:
		ActionInterruptEvent(Action* interruptingAction);

		virtual EventType getEventType() const override;
		
		Action* getInterruptingAction() const;
		
	private:
		Action* interruptingAction;
	};
}

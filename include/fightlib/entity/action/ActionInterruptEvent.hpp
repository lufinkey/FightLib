
#pragma once

#include <GameLibrary/GameLibrary.hpp>
#include "Action.hpp"

namespace fl
{
	extern const fgl::EventType EVENT_ACTIONINTERRUPT;
	
	class ActionInterruptEvent : public fgl::Event
	{
	public:
		ActionInterruptEvent(Action* interruptingAction);

		virtual fgl::EventType getEventType() const override;
		
		Action* getInterruptingAction() const;
		
	private:
		Action* interruptingAction;
	};
}

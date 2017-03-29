
#include <fightlib/entity/action/ActionEvent.hpp>

namespace fl
{
	ActionEvent::ActionEvent(ActionEventType eventType) : eventType(eventType)
	{
		//
	}
	
	ActionEvent::~ActionEvent()
	{
		//
	}
	
	ActionEventType ActionEvent::getEventType() const
	{
		return eventType;
	}
	
	ActionEventType registerActionEventType()
	{
		static ActionEventType nextType = 1;
		ActionEventType eventType = nextType;
		nextType++;
		return eventType;
	}
}

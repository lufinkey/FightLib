
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
	
	ActionEventType ActionEventType_nextType = 1;
	
	ActionEventType registerActionEventType()
	{
		ActionEventType eventType = ActionEventType_nextType;
		ActionEventType_nextType++;
		return eventType;
	}
}


#include <fightlib/entity/action/ActionEvent.hpp>

namespace fl
{
	ActionEvent::~ActionEvent()
	{
		//
	}
	
	ActionEventType registerActionEventType()
	{
		static ActionEventType nextType = 1;
		ActionEventType eventType = nextType;
		nextType++;
		return eventType;
	}
}

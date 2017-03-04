
#include <fightlib/entity/action/Action.hpp>
#include <fightlib/entity/ActionEntity.hpp>

namespace fl
{
	ActionParams::~ActionParams()
	{
		//
	}

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

	Action::~Action()
	{
		//
	}

	void Action::onPerform(ActionEntity* entity, ActionParamsPtr params)
	{
		//Open for implementation
	}

	void Action::onUpdate(ActionEntity* entity, fgl::ApplicationData appData)
	{
		//Open for implementation
	}

	void Action::onEnd(ActionEntity* entity)
	{
		//Open for implementation
	}

	void Action::onEvent(ActionEntity* entity, ActionEventPtr event)
	{
		//Open for implementation
	}

	void Action::end(ActionEntity* entity)
	{
		entity->endAction(this);
	}
}

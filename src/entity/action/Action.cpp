
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
	
	ActionEventType ActionEventType_nextType = 1;
	
	ActionEventType registerActionEventType()
	{
		ActionEventType eventType = ActionEventType_nextType;
		ActionEventType_nextType++;
		return eventType;
	}
	
	Action::Action()
		: entity(nullptr),
		performing(false)
	{
		//
	}

	Action::~Action()
	{
		//
	}
	
	ActionEntity* Action::getEntity() const
	{
		return entity;
	}
	
	bool Action::isPerforming() const
	{
		return performing;
	}

	bool Action::getFlag(const fgl::String& flag) const
	{
		return false;
	}

	void Action::onPerform(ActionParamsPtr params)
	{
		//Open for implementation
	}

	void Action::onUpdate(const fgl::ApplicationData& appData)
	{
		//Open for implementation
	}

	void Action::onEnd()
	{
		//Open for implementation
	}

	void Action::onEvent(ActionEventPtr event)
	{
		//Open for implementation
	}

	void Action::end()
	{
		entity->endAction(this);
	}
}

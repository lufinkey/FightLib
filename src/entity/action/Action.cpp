
#include <fightlib/entity/action/Action.hpp>
#include <fightlib/entity/ActionEntity.hpp>

namespace fl
{
	const fgl::String ACTIONFLAG_ALLOWMOVEMENT = "AllowMovement";
	const fgl::String ACTIONFLAG_ALLOWORIENTATIONCHANGE = "AllowOrientationChange";

	ActionParams::ActionParams(const fgl::BasicDictionary<fgl::String, bool>& flags)
		: flags(flags)
	{
		//
	}

	ActionParams::~ActionParams()
	{
		//
	}

	bool ActionParams::getFlag(const fgl::String& flag) const
	{
		return flags.get(flag, false);
	}

	ActionParamsPtr createActionFlags(const fgl::BasicDictionary<fgl::String, bool>& flags)
	{
		return ActionParamsPtr(new ActionParams(flags));
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

	void Action::onEvent(EventPtr event)
	{
		//Open for implementation
	}

	void Action::end()
	{
		entity->endAction(this);
	}
}

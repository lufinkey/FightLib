
#include <fightlib/entity/ActionEntity.hpp>

namespace fl
{
	ActionEntity::ActionEntity(const fgl::Vector2d& position, Entity::Orientation orientation)
		: Entity(position, orientation),
		currentAction(nullptr)
	{
		//
	}

	ActionEntity::~ActionEntity()
	{
		for(auto& actionPair : actions)
		{
			delete actionPair.second;
		}
	}

	bool ActionEntity::performAction(const fgl::String& name, ActionParamsPtr params)
	{
		if(currentAction!=nullptr)
		{
			return false;
		}
		Action* action = getAction(name);
		if(action==nullptr)
		{
			throw fgl::IllegalArgumentException("name", "does not match any actions");
		}
		currentAction = action;
		currentAction->onPerform(this, params);
		return true;
	}

	void ActionEntity::addAction(const fgl::String& name, Action* action)
	{
		actions.add(std::pair<fgl::String, Action*>(name, action));
	}

	void ActionEntity::endAction(Action* action)
	{
		if(action!=currentAction)
		{
			throw fgl::IllegalArgumentException("action", "is not currently being performed by this entity");
		}
		currentAction = nullptr;
		action->onEnd(this);
	}

	Action* ActionEntity::getAction(const fgl::String& name) const
	{
		for(auto& actionPair : actions)
		{
			if(actionPair.first==name)
			{
				return actionPair.second;
			}
		}
		return nullptr;
	}
}

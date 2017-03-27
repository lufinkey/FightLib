
#include <fightlib/entity/ActionEntity.hpp>

namespace fl
{
	ActionEntity::ActionEntity(const fgl::Vector2d& position, Orientation orientation)
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

	void ActionEntity::update(const fgl::ApplicationData& appData)
	{
		if(currentAction!=nullptr)
		{
			currentAction->onUpdate(this, appData);
		}
		Entity::update(appData);
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

	const fgl::String& ActionEntity::getActionName(Action* action) const
	{
		for(auto& actionPair : actions)
		{
			if(actionPair.second==action)
			{
				return actionPair.first;
			}
		}
		throw fgl::IllegalArgumentException("action", "does not belong to this entity");
	}

	Action* ActionEntity::getCurrentAction() const
	{
		return currentAction;
	}
}

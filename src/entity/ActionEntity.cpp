
#include <fightlib/entity/ActionEntity.hpp>
#include <fightlib/entity/action/ActionInterruptEvent.hpp>

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
		Entity::update(appData);
		for(auto actionPair : actions)
		{
			actionPair.second->onUpdate(appData);
		}
	}
	
	void ActionEntity::onActionEnd(Action* action)
	{
		//Open for implementation
	}

	bool ActionEntity::performAction(const fgl::String& name, ActionParamsPtr params)
	{
		Action* action = getAction(name);
		if(action==nullptr)
		{
			throw fgl::IllegalArgumentException("name", "does not match any actions");
		}
		if(currentAction!=nullptr)
		{
			//send an ActionInterruptEvent to give the current action a chance to end itself
			currentAction->onEvent(std::shared_ptr<ActionEvent>(new ActionInterruptEvent(action)));
			if(currentAction!=nullptr)
			{
				//the current action did not end itself
				return false;
			}
		}
		currentAction = action;
		currentAction->performing = true;
		currentAction->onPerform(params);
		return true;
	}

	void ActionEntity::addAction(const fgl::String& name, Action* action)
	{
		if(action==nullptr)
		{
			throw fgl::IllegalArgumentException("action", "cannot be null");
		}
		else if(action->entity != nullptr)
		{
			throw fgl::IllegalArgumentException("action", "cannot be added to multiple entities at the same time");
		}
		action->entity = this;
		actions.add(std::pair<fgl::String, Action*>(name, action));
	}

	void ActionEntity::endAction(Action* action)
	{
		if(action==nullptr)
		{
			throw fgl::IllegalArgumentException("action", "cannot be null");
		}
		else if(action!=currentAction)
		{
			throw fgl::IllegalArgumentException("action", "is not currently being performed by this entity");
		}
		currentAction->performing = false;
		currentAction = nullptr;
		action->onEnd();
		onActionEnd(action);
	}
	
	void ActionEntity::sendActionEvent(ActionEventPtr event, bool currentActionOnly)
	{
		if(currentActionOnly)
		{
			if(currentAction!=nullptr)
			{
				currentAction->onEvent(event);
			}
		}
		else
		{
			for(auto actionPair : actions)
			{
				actionPair.second->onEvent(event);
			}
		}
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

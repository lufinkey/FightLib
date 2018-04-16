
#include <fightlib/entity/ActionEntity.hpp>
#include <fightlib/entity/action/ActionInterruptEvent.hpp>
#include <fightlib/entity/action/AttackAction.hpp>

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
	
	bool ActionEntity::getFlag(const fgl::String& flag) const
	{
		if(flag=="ActionEntity")
		{
			return true;
		}
		return Entity::getFlag(flag);
	}

	void ActionEntity::update(fgl::ApplicationData appData)
	{
		Entity::update(appData);
		for(auto actionPair : actions)
		{
			actionPair.second->onUpdate(appData);
		}
	}
	
	HitboxInfo ActionEntity::getHitboxInfo(size_t tag) const
	{
		if(currentAction!=nullptr && currentAction->getFlag("AttackAction"))
		{
			return static_cast<AttackAction*>(currentAction)->getHitboxInfo(tag);
		}
		return Entity::getHitboxInfo(tag);
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
			currentAction->onEvent(fgl::EventPtr(new ActionInterruptEvent(action)));
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
			throw fgl::IllegalArgumentException("action", "cannot be added to multiple entities");
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
	
	void ActionEntity::sendActionEvent(fgl::EventPtr event, bool allActions)
	{
		if(allActions)
		{
			for(auto actionPair : actions)
			{
				actionPair.second->onEvent(event);
			}
		}
		else
		{
			if(currentAction!=nullptr)
			{
				currentAction->onEvent(event);
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
	
	void ActionEntity::onCollision(const fgl::CollisionEvent& collisionEvent)
	{
		Entity::onCollision(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new fgl::CollisionEvent(collisionEvent)));
		}
	}
	
	void ActionEntity::onCollisionUpdate(const fgl::CollisionEvent& collisionEvent)
	{
		Entity::onCollisionUpdate(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new fgl::CollisionEvent(collisionEvent)));
		}
	}
	
	void ActionEntity::onCollisionFinish(const fgl::CollisionEvent& collisionEvent)
	{
		Entity::onCollisionFinish(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new fgl::CollisionEvent(collisionEvent)));
		}
	}
	
	void ActionEntity::onHitboxClash(const HitboxClashEvent& clashEvent)
	{
		Entity::onHitboxClash(clashEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxClashEvent(clashEvent)));
		}
	}
	
	void ActionEntity::onHitboxClashUpdate(const HitboxClashEvent& clashEvent)
	{
		Entity::onHitboxClashUpdate(clashEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxClashEvent(clashEvent)));
		}
	}
	
	void ActionEntity::onHitboxClashFinish(const HitboxClashEvent& clashEvent)
	{
		Entity::onHitboxClashFinish(clashEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxClashEvent(clashEvent)));
		}
	}
	
	void ActionEntity::onHitboxCollision(const HitboxCollisionEvent& collisionEvent)
	{
		Entity::onHitboxCollision(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxCollisionEvent(collisionEvent)));
		}
	}
	
	void ActionEntity::onHitboxCollisionUpdate(const HitboxCollisionEvent& collisionEvent)
	{
		Entity::onHitboxCollisionUpdate(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxCollisionEvent(collisionEvent)));
		}
	}
	
	void ActionEntity::onHitboxCollisionFinish(const HitboxCollisionEvent& collisionEvent)
	{
		Entity::onHitboxCollisionFinish(collisionEvent);
		if(currentAction!=nullptr)
		{
			currentAction->onEvent(fgl::EventPtr(new HitboxCollisionEvent(collisionEvent)));
		}
	}
}

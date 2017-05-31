
#pragma once

#include "Entity.hpp"
#include "action/Action.hpp"

namespace fl
{
	class ActionEntity : public Entity
	{
		friend class Action;
	public:
		ActionEntity(const fgl::Vector2d& position, Orientation orientation);
		virtual ~ActionEntity();
		
		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;
		
		virtual HitboxInfo getHitboxInfo(size_t tag) const override;

		bool performAction(const fgl::String& name, ActionParamsPtr params = ActionParamsPtr(new ActionParams()));

	protected:
		void addAction(const fgl::String& name, Action* action);
		Action* getAction(const fgl::String& name) const;
		const fgl::String& getActionName(Action* action) const;
		Action* getCurrentAction() const;
		void sendActionEvent(EventPtr event, bool allActions = false);
		
		virtual void onActionEnd(Action* action);
		
		virtual void onCollision(const CollisionEvent& collisionEvent) override;
		virtual void onCollisionUpdate(const CollisionEvent& collisionEvent) override;
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent) override;
		
		virtual void onHitboxClash(const HitboxClashEvent& clashEvent) override;
		virtual void onHitboxClashUpdate(const HitboxClashEvent& clashEvent) override;
		virtual void onHitboxClashFinish(const HitboxClashEvent& clashEvent) override;
		
		virtual void onHitboxCollision(const HitboxCollisionEvent& collisionEvent) override;
		virtual void onHitboxCollisionUpdate(const HitboxCollisionEvent& collisionEvent) override;
		virtual void onHitboxCollisionFinish(const HitboxCollisionEvent& collisionEvent) override;

	private:
		void endAction(Action* action);
		
		Action* currentAction;
		fgl::ArrayList<std::pair<fgl::String, Action*>> actions;
	};
}

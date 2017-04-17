
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
		virtual void update(const fgl::ApplicationData& appData) override;
		
		virtual HitboxInfo getHitboxInfo(size_t tag) const override;

		bool performAction(const fgl::String& name, ActionParamsPtr params = ActionParamsPtr(new ActionParams()));

	protected:
		void addAction(const fgl::String& name, Action* action);
		Action* getAction(const fgl::String& name) const;
		const fgl::String& getActionName(Action* action) const;
		Action* getCurrentAction() const;
		void sendActionEvent(EventPtr event, bool allActions = false);
		
		virtual void onActionEnd(Action* action);

	private:
		void endAction(Action* action);
		
		Action* currentAction;
		fgl::ArrayList<std::pair<fgl::String, Action*>> actions;
	};
}

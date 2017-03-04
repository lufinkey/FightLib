
#pragma once

#include "Entity.hpp"
#include "action/Action.hpp"

namespace fl
{
	class ActionEntity : public Entity
	{
		friend class Action;
	public:
		ActionEntity(const fgl::Vector2d& position, Entity::Orientation orientation);
		virtual ~ActionEntity();

		virtual void update(fgl::ApplicationData appData) override;

		bool performAction(const fgl::String& name, ActionParamsPtr params = nullptr);

	protected:
		void addAction(const fgl::String& name, Action* action);

	private:
		void endAction(Action* action);

		Action* getAction(const fgl::String& name) const;

		Action* currentAction;
		fgl::ArrayList<std::pair<fgl::String, Action*>> actions;
	};
}

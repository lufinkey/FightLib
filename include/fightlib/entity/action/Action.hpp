
#pragma once

#include <fightlib/base/Event.hpp>

namespace fl
{
	class ActionEntity;

	class ActionParams
	{
	public:
		ActionParams(const fgl::BasicDictionary<fgl::String, bool>& flags = {});
		virtual ~ActionParams();

		virtual bool getFlag(const fgl::String& flag) const;

	private:
		fgl::BasicDictionary<fgl::String, bool> flags;
	};

	typedef std::shared_ptr<ActionParams> ActionParamsPtr;

	ActionParamsPtr createActionFlags(const fgl::BasicDictionary<fgl::String, bool>& flags);


	class Action
	{
		friend class ActionEntity;
	public:
		Action();
		virtual ~Action();

		ActionEntity* getEntity() const;
		bool isPerforming() const;

		virtual bool getFlag(const fgl::String& flag) const;

	protected:
		virtual void onPerform(ActionParamsPtr params = ActionParamsPtr(new ActionParams()));
		virtual void onUpdate(const fgl::ApplicationData& appData);
		virtual void onEnd();
		virtual void onEvent(EventPtr event);

		void end();

	private:
		ActionEntity* entity;
		bool performing;
	};

	extern const fgl::String ACTIONFLAG_ALLOWMOVEMENT;
	extern const fgl::String ACTIONFLAG_ALLOWORIENTATIONCHANGE;
}

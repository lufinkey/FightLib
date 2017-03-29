
#pragma once

#include "ActionEvent.hpp"

namespace fl
{
	class ActionEntity;
	
	class ActionParams
	{
	public:
		virtual ~ActionParams();
	};
	
	typedef std::shared_ptr<ActionParams> ActionParamsPtr;
	
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
		virtual void onPerform(ActionParamsPtr params = nullptr);
		virtual void onUpdate(const fgl::ApplicationData& appData);
		virtual void onEnd();
		virtual void onEvent(ActionEventPtr event);

		void end();
		
	private:
		ActionEntity* entity;
		bool performing;
	};
	
	extern const fgl::String ACTIONFLAG_ALLOWMOVEMENT;
	extern const fgl::String ACTIONFLAG_ALLOWORIENTATIONCHANGE;
}

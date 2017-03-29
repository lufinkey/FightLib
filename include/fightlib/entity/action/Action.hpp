
#pragma once

#include <memory>
#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class ActionEntity;


	
	class ActionParams
	{
	public:
		virtual ~ActionParams();
	};

	typedef std::shared_ptr<ActionParams> ActionParamsPtr;



	typedef fgl::Uint32 ActionEventType;
	ActionEventType registerActionEventType();

	class ActionEvent
	{
	public:
		virtual ~ActionEvent();
		ActionEventType getEventType() const;
	protected:
		ActionEvent(ActionEventType eventType);
	private:
		ActionEventType eventType;
	};

	typedef std::shared_ptr<ActionEvent> ActionEventPtr;



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
	
	static const fgl::String ACTIONFLAG_ALLOWMOVEMENT = "AllowMovement";
	static const fgl::String ACTIONFLAG_ALLOWORIENTATIONCHANGE = "AllowOrientationChange";
}

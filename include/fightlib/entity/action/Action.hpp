
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
		virtual ~Action();

	protected:
		virtual void onPerform(ActionEntity* entity, ActionParamsPtr params = nullptr);
		virtual void onEnd(ActionEntity* entity);
		virtual void onEvent(ActionEntity* entity, ActionEventPtr event);

		void end(ActionEntity* entity);
	};
}

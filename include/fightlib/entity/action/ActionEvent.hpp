
#pragma once

#include <memory>
#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
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
}

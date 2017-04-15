
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
		virtual ActionEventType getEventType() const = 0;
	};
	
	typedef std::shared_ptr<ActionEvent> ActionEventPtr;
}

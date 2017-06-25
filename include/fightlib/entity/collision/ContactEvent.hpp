
#pragma once

#include <fightlib/base/Event.hpp>

namespace fl
{
	typedef enum
	{
		CONTACTSTATE_NEW,
		CONTACTSTATE_UPDATED,
		CONTACTSTATE_FINISHED
	} ContactState;

	extern const EventType EVENT_CONTACT;

	class Collidable;

	class ContactEvent : public Event
	{
	public:
		ContactEvent(Collidable* contacted, ContactState state);

		virtual EventType getEventType() const override;

		Collidable* getContacted() const;
		ContactState getContactState() const;

	private:
		Collidable* contacted;
		ContactState state;
	};
}

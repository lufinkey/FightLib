
#pragma once

#include <fightlib/base/Event.hpp>

namespace fl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;

	extern const EventType EVENT_COLLISION;
	
	class Collidable;
	
	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(Collidable* collided, CollisionSide side);

		virtual EventType getEventType() const override;
		
		Collidable* getCollided() const;
		CollisionSide getCollisionSide() const;
		
	private:
		Collidable* collided;
		CollisionSide side;
	};
}

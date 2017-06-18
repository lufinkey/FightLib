
#pragma once

#include <fightlib/base/Event.hpp>
#include <fightlib/entity/collision/CollisionData.hpp>

namespace fl
{
	typedef enum
	{
		COLLISIONSTATE_STARTED,
		COLLISIONSTATE_UPDATED,
		COLLISIONSTATE_ENDED
	} CollisionState;

	extern const EventType EVENT_COLLISION;
	
	class Collidable;
	
	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(Collidable* collided, CollisionState state, const fgl::ArrayList<CollisionData>& collisions);

		virtual EventType getEventType() const override;
		
		Collidable* getCollided() const;
		CollisionState getCollisionState() const;
		const fgl::ArrayList<CollisionData>& getCollisions() const;
		
	private:
		Collidable* collided;
		CollisionState state;
		fgl::ArrayList<CollisionData> collisions;
	};
}

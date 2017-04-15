
#pragma once

#include <fightlib/entity/action/ActionEvent.hpp>

namespace fl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;

	extern const ActionEventType ACTIONEVENT_COLLISION;
	
	class Collidable;
	
	class CollisionEvent : public ActionEvent
	{
	public:
		CollisionEvent(Collidable* collided, CollisionSide side);

		virtual ActionEventType getEventType() const override;
		
		Collidable* getCollided() const;
		CollisionSide getCollisionSide() const;
		
	private:
		Collidable* collided;
		CollisionSide side;
	};
}

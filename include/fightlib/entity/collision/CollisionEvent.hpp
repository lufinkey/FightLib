
#pragma once

namespace fl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;
	
	class Collidable;
	
	class CollisionEvent
	{
	public:
		CollisionEvent(Collidable* collided, CollisionSide side);
		
		Collidable* getCollided() const;
		CollisionSide getCollisionSide() const;
		
	private:
		Collidable* collided;
		CollisionSide side;
	};
}

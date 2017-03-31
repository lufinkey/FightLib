
#pragma once

#include "Collidable.hpp"

namespace fl
{
	typedef enum : fgl::Uint8
	{
		COLLISIONMETHOD_NONE,
		COLLISIONMETHOD_FRAME,
		COLLISIONMETHOD_BOUNDS,
		COLLISIONMETHOD_PIXEL
	} CollisionMethod;
	
	class CollisionRectManager
	{
	public:
		CollisionRectManager();
		~CollisionRectManager();
		
		void setCollisionMethod(CollisionMethod method);
		CollisionMethod getCollisionMethod() const;
		
		const fgl::ArrayList<CollisionRect*>& getCollisionRects() const;
		
		void update(const fgl::ApplicationData& appData, Collidable* collidable);
		
	private:
		fgl::ArrayList<CollisionRect*> createCollisionRects(const fgl::ApplicationData& appData, Collidable* collidable) const;
		
		fgl::ArrayList<CollisionRect*> collisionRects;
		CollisionMethod collisionMethod;
	};
}

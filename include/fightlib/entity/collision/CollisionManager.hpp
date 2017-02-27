
#pragma once

#include "CollisionPair.hpp"
#include "rects/CollisionRect.hpp"

namespace fl
{
	class Entity;

	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;

	class CollisionManager
	{
	public:
		CollisionManager();

		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);

		void update(fgl::ApplicationData appData);

	private:
		CollisionSide getCollisionSide(const fgl::Vector2d& shiftAmount) const;

		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<CollisionPair> previousCollisions;

		fgl::ArrayList<CollisionPair> getCollisionPairs() const;
	};
}

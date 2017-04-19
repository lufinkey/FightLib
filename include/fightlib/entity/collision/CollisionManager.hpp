
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"

namespace fl
{
	class CollisionManager
	{
	public:
		CollisionManager();

		void addCollidable(Collidable* entity);
		void removeCollidable(Collidable* entity);

		void update(const fgl::ApplicationData& appData);

	private:
		CollisionSide getCollisionSide(const fgl::Vector2d& shiftAmount) const;

		fgl::ArrayList<Collidable*> collidables;
		fgl::ArrayList<CollisionPair> previousCollisions;

		fgl::ArrayList<CollisionPair> getCollisionPairs() const;
	};
}

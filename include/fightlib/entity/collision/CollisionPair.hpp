
#pragma once

#include "Collidable.hpp"

namespace fl
{
	class CollisionPair
	{
	public:
		CollisionPair(Collidable* collidable1, Collidable* collidable2, const CollisionData& data);

		void removeCollision(b2Fixture* fixture1, b2Fixture* fixture2);
		fgl::ArrayList<CollisionData> getReversedCollisions() const;

		Collidable* collidable1;
		Collidable* collidable2;

		fgl::ArrayList<CollisionData> collisions;
		fgl::ArrayList<CollisionData> previousCollisions;

		bool wasIgnored;
		bool ignored;
	};
}

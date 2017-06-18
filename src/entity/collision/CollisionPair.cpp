
#include <fightlib/entity/collision/CollisionPair.hpp>

namespace fl
{
	CollisionPair::CollisionPair(Collidable* collidable1, Collidable* collidable2, const CollisionData& data)
		: collidable1(collidable1),
		collidable2(collidable2),
		wasIgnored(false),
		ignored(false)
	{
		collisions.add(data);
	}

	void CollisionPair::removeCollision(b2Fixture* fixture1, b2Fixture* fixture2)
	{
		for(size_t collisions_size=collisions.size(), i=0; i<collisions_size; i++)
		{
			auto& collision = collisions[i];
			if(collision.getFixture1()==fixture1 && collision.getFixture2()==fixture2)
			{
				collisions.remove(i);
				return;
			}
		}
	}

	fgl::ArrayList<CollisionData> CollisionPair::getReversedCollisions() const
	{
		fgl::ArrayList<CollisionData> reversedCollisions;
		reversedCollisions.reserve(collisions.size());
		for(auto& collision : collisions)
		{
			reversedCollisions.add(collision.reversed());
		}
		return reversedCollisions;
	}
}


#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"

class b2World;
class b2Fixture;

namespace fl
{
	class Box2DCollisionHandler;

	class CollisionManager
	{
	public:
		CollisionManager();
		~CollisionManager();

		void setGravity(const fgl::Vector2d& gravity);
		fgl::Vector2d getGravity() const;

		void addCollidable(Collidable* collidable);
		void removeCollidable(Collidable* collidable);

		void update(const fgl::ApplicationData& appData);

		b2World* getWorld();
		const fgl::ArrayList<CollisionPair>& getCollisionPairs() const;

	private:
		b2World* world;
		fgl::ArrayList<Collidable*> collidables;
		Box2DCollisionHandler* collisionHandler;
	};
}

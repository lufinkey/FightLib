
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"

namespace box2d
{
	class World;
	class Fixture;
}

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

		box2d::World* getWorld();
		const fgl::ArrayList<CollisionPair>& getCollisionPairs() const;

	private:
		box2d::World* world;
		fgl::ArrayList<Collidable*> collidables;
		Box2DCollisionHandler* collisionHandler;
	};
}

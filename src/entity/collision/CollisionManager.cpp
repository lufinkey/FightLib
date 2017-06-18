
#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/collision/PhysicsConstants.hpp>
#include "Box2DCollisionHandler.hpp"
#include <Box2D/Box2D.h>

namespace fl
{
	CollisionManager::CollisionManager()
	{
		world = new b2World(b2Vec2(0, 0));
		collisionHandler = new Box2DCollisionHandler();
		world->SetContactListener(collisionHandler);
		world->SetContactFilter(collisionHandler);
	}

	CollisionManager::~CollisionManager()
	{
		world->SetContactListener(nullptr);
		world->SetContactFilter(nullptr);
		for(auto collidable : collidables)
		{
			collidable->deinitPhysicsBody();
			collidable->collisionManager = nullptr;
		}
		delete world;
		delete collisionHandler;
	}

	void CollisionManager::setGravity(const fgl::Vector2d& gravity)
	{
		world->SetGravity(b2Vec2((float32)(gravity.x * PIXELS_TO_METERS * PIXELS_TO_METERS), (float32)(gravity.y * PIXELS_TO_METERS * PIXELS_TO_METERS)));
	}

	fgl::Vector2d CollisionManager::getGravity() const
	{
		auto grav = world->GetGravity();
		return fgl::Vector2d((double)grav.x * METERS_TO_PIXELS * METERS_TO_PIXELS, (double)grav.y * METERS_TO_PIXELS * METERS_TO_PIXELS);
	}

	void CollisionManager::addCollidable(Collidable* collidable)
	{
		if(collidable->collisionManager!=nullptr)
		{
			throw fgl::IllegalArgumentException("collidable", "already added to a CollisionManager");
		}
		collidable->collisionManager = this;
		collidables.add(collidable);
		collidable->initPhysicsBody();
	}

	void CollisionManager::removeCollidable(Collidable* collidable)
	{
		if(collidable->collisionManager!=this)
		{
			//not added to this CollisionManager
			return;
		}
		//TODO end any collisions that this collidable is engaged in
		collidable->deinitPhysicsBody();
		collidables.removeFirstEqual(collidable);
	}

	void CollisionManager::update(const fgl::ApplicationData& appData)
	{
		//begin collision updates
		for(auto& collidable : collidables)
		{
			collidable->onBeginCollisionUpdates();
		}

		//step the world
		world->Step((float32)appData.getFrameSpeedMultiplier(), 3, 3);

		//get collision callbacks
		fgl::ArrayList<std::function<void()>> onCollisionCalls;
		onCollisionCalls.reserve(collisionHandler->collisionPairs.size());
		fgl::ArrayList<std::function<void()>> onCollisionFinishCalls;
		onCollisionFinishCalls.reserve(collisionHandler->collisionPairs.size());
		for(auto& pair : collisionHandler->collisionPairs)
		{
			if(!pair.ignored && pair.previousCollisions.size()==0 && pair.collisions.size() > 0)
			{
				onCollisionCalls.add([=]{
					pair.collidable1->onCollision(CollisionEvent(pair.collidable2, COLLISIONSTATE_STARTED, pair.collisions));
					pair.collidable2->onCollision(CollisionEvent(pair.collidable1, COLLISIONSTATE_STARTED, pair.getReversedCollisions()));
				});
			}
			else if((!pair.ignored && pair.collisions.size()==0 && pair.previousCollisions.size() > 0) || (pair.ignored && !pair.wasIgnored && pair.previousCollisions.size()>0))
			{
				onCollisionFinishCalls.add([=]{
					pair.collidable1->onCollisionFinish(CollisionEvent(pair.collidable2, COLLISIONSTATE_ENDED, pair.collisions));
					pair.collidable2->onCollisionFinish(CollisionEvent(pair.collidable1, COLLISIONSTATE_ENDED, pair.getReversedCollisions()));
				});
			}
			else if(!pair.ignored)
			{
				onCollisionCalls.add([=]{
					pair.collidable1->onCollisionUpdate(CollisionEvent(pair.collidable2, COLLISIONSTATE_UPDATED, pair.collisions));
					pair.collidable2->onCollisionUpdate(CollisionEvent(pair.collidable1, COLLISIONSTATE_UPDATED, pair.getReversedCollisions()));
				});
			}
		}

		//remove finished collisions
		collisionHandler->collisionPairs.removeWhere([](const CollisionPair& pair) -> bool {
			if(pair.collisions.size()==0)
			{
				return true;
			}
			return false;
		});

		//update the CollisionPairs
		for(auto& pair : collisionHandler->collisionPairs)
		{
			pair.previousCollisions = pair.collisions;
		}

		//call the collision callbacks
		for(auto& onCollisionFinish : onCollisionFinishCalls)
		{
			onCollisionFinish();
		}
		for(auto& onCollision : onCollisionCalls)
		{
			onCollision();
		}

		//finish collision updates
		for(auto& collidable : collidables)
		{
			collidable->onFinishCollisionUpdates();
		}
	}

	b2World* CollisionManager::getWorld()
	{
		return world;
	}

	const fgl::ArrayList<CollisionPair>& CollisionManager::getCollisionPairs() const
	{
		return collisionHandler->collisionPairs;
	}
}

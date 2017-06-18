
#include "Box2DCollisionHandler.hpp"
#include <fightlib/entity/collision/Collidable.hpp>

namespace fl
{
	Box2DCollisionHandler::Box2DCollisionHandler()
	{
		//
	}

	float Box2DCollisionHandler::getContactAngle(const box2d::Contact& contact)
	{
		auto localNormal = contact.GetManifold().GetLocalNormal();
		switch(contact.GetManifold().GetType())
		{
			case box2d::Manifold::Type::e_circles:
			//TODO get the angle from fixtureA's center to fixtureB's center
			throw fgl::NotImplementedException("I haven't implemented box2d e_circles");
			break;

			case box2d::Manifold::Type::e_faceA:
			return fgl::Math::normalizeDegrees(fgl::Math::radtodeg(fgl::Math::atan2((double)localNormal.x, (double)localNormal.y))+180.0);

			case box2d::Manifold::Type::e_faceB:
			return fgl::Math::radtodeg(fgl::Math::atan2((double)localNormal.x, (double)localNormal.y));

			default:
			throw std::logic_error("what the fuck");
		}
	}

	bool Box2DCollisionHandler::ShouldCollide(const box2d::Fixture* fixture1, const box2d::Fixture* fixture2)
	{
		auto collidable1 = (Collidable*)fixture1->GetBody()->GetUserData();
		auto collidable2 = (Collidable*)fixture2->GetBody()->GetUserData();

		//if the bodies weren't added using CollisionManager, then ignore their collisions
		if(collidable1==nullptr || collidable2==nullptr)
		{
			return false;
		}

		//prevent static bodies from colliding with each other
		if(collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody())
		{
			return false;
		}
		return true;
	}

	void Box2DCollisionHandler::BeginContact(box2d::Contact& contact)
	{
		auto fixture1 = contact.GetFixtureA();
		auto fixture2 = contact.GetFixtureB();

		auto collidable1 = (Collidable*)fixture1->GetBody()->GetUserData();
		auto collidable2 = (Collidable*)fixture2->GetBody()->GetUserData();

		//if the bodies weren't added using CollisionManager, then we don't care about this collision
		if(collidable1==nullptr || collidable2==nullptr)
		{
			return;
		}

		//find an existing instance of the collision between the two Collidables
		for(auto& pair : collisionPairs)
		{
			if(pair.collidable1==collidable1 && pair.collidable2==collidable2)
			{
				auto data = CollisionData(getContactAngle(contact), fixture1, fixture2);
				pair.collisions.add(data);
				if(!pair.ignored)
				{
					if(!pair.collidable1->respondsToCollision(pair.collidable2, data))
					{
						pair.ignored = true;
					}
					else if(!pair.collidable2->respondsToCollision(pair.collidable1, data.reversed()))
					{
						pair.ignored = true;
					}
				}
				return;
			}
			else if(pair.collidable1==collidable2 && pair.collidable2==collidable1)
			{
				auto data = CollisionData(getContactAngle(contact), fixture1, fixture2).reversed();
				pair.collisions.add(data);
				if(!pair.ignored)
				{
					if(!pair.collidable1->respondsToCollision(pair.collidable2, data))
					{
						pair.ignored = true;
					}
					else if(!pair.collidable2->respondsToCollision(pair.collidable1, data.reversed()))
					{
						pair.ignored = true;
					}
				}
				return;
			}
		}

		//no existing collision exists, so make one
		auto data = CollisionData(getContactAngle(contact), fixture1, fixture2);
		auto pair = CollisionPair(collidable1, collidable2, data);
		if(!pair.collidable1->respondsToCollision(pair.collidable2, data))
		{
			pair.ignored = true;
		}
		else if(!pair.collidable2->respondsToCollision(pair.collidable1, data.reversed()))
		{
			pair.ignored = true;
		}
		collisionPairs.add(pair);
	}

	void Box2DCollisionHandler::EndContact(box2d::Contact& contact)
	{
		auto fixture1 = contact.GetFixtureA();
		auto fixture2 = contact.GetFixtureB();

		auto collidable1 = (Collidable*)fixture1->GetBody()->GetUserData();
		auto collidable2 = (Collidable*)fixture2->GetBody()->GetUserData();

		//if the bodies weren't added using CollisionManager, then we don't care about this collision
		if(collidable1==nullptr || collidable2==nullptr)
		{
			return;
		}

		for(auto& pair : collisionPairs)
		{
			if(pair.collidable1==collidable1 && pair.collidable2==collidable2)
			{
				pair.removeCollision(fixture1, fixture2);
				return;
			}
			else if(pair.collidable1==collidable2 && pair.collidable2==collidable1)
			{
				pair.removeCollision(fixture2, fixture1);
				return;
			}
		}
	}

	void Box2DCollisionHandler::PreSolve(box2d::Contact& contact, const box2d::Manifold& oldManifold)
	{
		auto fixture1 = contact.GetFixtureA();
		auto fixture2 = contact.GetFixtureB();

		auto collidable1 = (Collidable*)fixture1->GetBody()->GetUserData();
		auto collidable2 = (Collidable*)fixture2->GetBody()->GetUserData();

		//if the bodies weren't added using CollisionManager, then we don't care about this collision
		if(collidable1==nullptr || collidable2==nullptr)
		{
			return;
		}

		//check if the collision was ignored and disable the contact if it was
		auto data = CollisionData(getContactAngle(contact), fixture1, fixture2);
		auto revData = data.reversed();
		for(auto& pair : collisionPairs)
		{
			if(pair.collidable1==collidable1 && pair.collidable2==collidable2)
			{
				if(pair.ignored)
				{
					contact.SetEnabled(false);
				}
				else if(!collidable1->respondsToCollision(collidable2, data)
					|| !collidable2->respondsToCollision(collidable1, revData))
				{
					pair.ignored = true;
					contact.SetEnabled(false);
				}
				return;
			}
			else if(pair.collidable1==collidable2 && pair.collidable2==collidable1)
			{
				if(pair.ignored)
				{
					contact.SetEnabled(false);
				}
				else if(!collidable1->respondsToCollision(collidable2, data)
					|| !collidable2->respondsToCollision(collidable1, revData))
				{
					pair.ignored = true;
					contact.SetEnabled(false);
				}
				return;
			}
		}
	}

	void Box2DCollisionHandler::PostSolve(box2d::Contact& contact, const box2d::ContactImpulsesList& impulses, iteration_type solved)
	{
		//nuthin' to do here
	}
}

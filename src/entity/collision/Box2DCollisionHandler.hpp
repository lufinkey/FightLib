
#pragma once

#include <fightlib/entity/collision/CollisionPair.hpp>
#include <Box2D/Box2D.hpp>

namespace fl
{
	class Box2DCollisionHandler : public box2d::ContactListener, public box2d::ContactFilter
	{
		friend class CollisionManager;
	public:
		Box2DCollisionHandler();

		virtual bool ShouldCollide(const box2d::Fixture* fixture1, const box2d::Fixture* fixture2) override;
		virtual void BeginContact(box2d::Contact& contact) override;
		virtual void EndContact(box2d::Contact& contact) override;
		virtual void PreSolve(box2d::Contact& contact, const box2d::Manifold& oldManifold) override;
		virtual void PostSolve(box2d::Contact& contact, const box2d::ContactImpulsesList& impulses, iteration_type solved) override;

	private:
		static float getContactAngle(const box2d::Contact& contact);

		fgl::ArrayList<CollisionPair> collisionPairs;
	};
}

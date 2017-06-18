
#pragma once

#include <fightlib/entity/collision/CollisionPair.hpp>
#include <Box2D/Box2D.h>

namespace fl
{
	class Box2DCollisionHandler : public b2ContactListener, public b2ContactFilter
	{
		friend class CollisionManager;
	public:
		Box2DCollisionHandler();

		virtual bool ShouldCollide(b2Fixture* fixture1, b2Fixture* fixture2) override;
		virtual void BeginContact(b2Contact* contact) override;
		virtual void EndContact(b2Contact* contact) override;
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	private:
		static float getContactAngle(b2Contact* contact);

		fgl::ArrayList<CollisionPair> collisionPairs;
	};
}

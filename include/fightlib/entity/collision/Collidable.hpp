
#pragma once

#include <fightlib/entity/draw/Animatable.hpp>
#include "PhysicalState.hpp"
#include "CollisionEvent.hpp"

namespace box2d
{
	class Body;
	class World;
}

namespace fl
{
	class CollisionManager;

	typedef enum : fgl::Uint8
	{
		COLLISIONMETHOD_PIXEL,
		COLLISIONMETHOD_BOUNDS,
		COLLISIONMETHOD_FRAME
	} CollisionMethod;

	class Collidable : public Animatable
	{
		friend class CollisionManager;
		friend class Box2DCollisionHandler;
	public:
		Collidable(const fgl::Vector2d& position);
		virtual ~Collidable();

		virtual bool isStaticCollisionBody() const = 0;
		virtual bool isRotationFixed() const;
		virtual double getFriction(size_t polygonIndex) const;

		virtual fgl::Vector2d getPosition() const override;
		void setPosition(const fgl::Vector2d& position) override;

		virtual double getRotation() const override;
		virtual void setRotation(double degrees) override;

		fgl::Vector2d getVelocity() const;
		void setVelocity(const fgl::Vector2d& velocity);

		double getRotationalVelocity() const;
		void setRotationalVelocity(double degreesPerSecond);
		
		void applyForce(const fgl::Vector2d& force);
		void applyForce(const fgl::Vector2d& force, const fgl::Vector2d& point);

		fgl::ArrayList<fgl::PolygonD> getTransformedCollisionPolygons() const;

		box2d::Body* getPhysicsBody();
		CollisionManager* getCollisionManager() const;

	protected:
		virtual fgl::Vector2d getDrawPosition() const override;
		virtual double getDrawRotation() const override;
		virtual fgl::Vector2d getDrawScale() const override;

		virtual void onBeginCollisionUpdates();
		virtual bool respondsToCollision(Collidable* collided, CollisionData data) const;
		virtual void onCollision(const CollisionEvent& collisionEvent);
		virtual void onCollisionUpdate(const CollisionEvent& collisionEvent);
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent);
		virtual void onFinishCollisionUpdates();

		virtual fgl::ArrayList<fgl::PolygonD> getCollisionPolygons() const;
		void setNeedsCollisionPolygonsUpdate();
		bool needsCollisionPolygonsUpdate() const;
		void updateCollisionPolygons();

		void setCollisionMethod(CollisionMethod collisionMethod);
		CollisionMethod getCollisionMethod() const;

	private:
		void initPhysicsBody();
		void deinitPhysicsBody();
		
		CollisionManager* collisionManager;
		union
		{
			//when added to a CollisionManager
			box2d::Body* physicsBody;
			//when NOT added to a CollisionManager
			PhysicalState* physicalState;
		};
		CollisionMethod collisionMethod;
		bool needsPolygonsUpdate;
	};
}


#pragma once

#include <fightlib/entity/draw/Sprite.hpp>
#include "rects/CollisionRect.hpp"
#include "ContactEvent.hpp"
#include "CollisionEvent.hpp"

namespace fl
{
	class Collidable : public Sprite
	{
		friend class CollisionManager;
		friend class CollisionRectManager;
	public:
		Collidable(const fgl::Vector2d& position);

		virtual void update(fgl::ApplicationData appData) override;

		virtual double getMass() const;
		virtual bool isStaticCollisionBody() const = 0;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const = 0;

		const fgl::Vector2d& getPreviousPosition() const;

		void setVelocity(const fgl::Vector2d& velocity);
		const fgl::Vector2d& getVelocity() const;

		const fgl::Vector2d& getDisplacement() const;

		void applyForce(const fgl::Vector2d& force);

		bool checkCollision(Collidable* collidable) const;

		virtual bool respondsToCollision(Collidable* collided, CollisionSide side) const;

	protected:
		virtual void onBeginCollisionUpdates();

		virtual void onContact(const ContactEvent& contactEvent);
		virtual void onContactUpdate(const ContactEvent& contactEvent);
		virtual void onContactFinish(const ContactEvent& contactEvent);

		virtual void onCollision(const CollisionEvent& collisionEvent);
		virtual void onCollisionUpdate(const CollisionEvent& collisionEvent);
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent);

		virtual void onFinishCollisionUpdates();

	private:
		fgl::Vector2d previousPosition;
		fgl::Vector2d velocity;
		fgl::Vector2d displacement;
	};
}

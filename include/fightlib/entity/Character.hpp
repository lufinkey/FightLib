
#pragma once

#include "ActionEntity.hpp"

namespace fl
{
	class Character : public ActionEntity
	{
	public:
		Character(const fgl::Vector2d& position, Orientation orientation);

		virtual void update(const fgl::ApplicationData& appData) override;

		virtual fgl::String getIdleAnimationName() const = 0;
		virtual fgl::String getMoveAnimationName(double amount) const = 0;
		virtual double getMoveSpeed(double amount) const;

		void setDirection(const fgl::Vector2f& direction);
		const fgl::Vector2f& getDirection() const;
		
		bool isOnGround() const;

	protected:
		virtual void updateMovement(const fgl::ApplicationData& appData);
		virtual void updateMoveAnimation();
		
		virtual void onActionEnd(Action* action) override;
		
		virtual void onCollision(Collidable* collided, CollisionSide side) override;
		virtual void onCollisionFinish(Collidable* collided, CollisionSide side) override;
		virtual void onFinishCollisionUpdates() override;

	private:
		fgl::Vector2f direction;
		size_t topCollidedCount;
	};
}

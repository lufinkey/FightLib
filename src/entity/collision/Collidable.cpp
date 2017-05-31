
#include <fightlib/entity/collision/Collidable.hpp>

namespace fl
{
	Collidable::Collidable(const fgl::Vector2d& position)
		: Sprite(position),
		previousPosition(position)
	{
		//
	}
	
	void Collidable::update(fgl::ApplicationData appData)
	{
		Sprite::update(appData);
		
		//offset for velocity
		shift(velocity*appData.getFrameSpeedMultiplier());
	}

	double Collidable::getMass() const
	{
		return 1.0;
	}
	
	const fgl::Vector2d& Collidable::getPreviousPosition() const
	{
		return previousPosition;
	}
	
	void Collidable::setVelocity(const fgl::Vector2d& velocity_arg)
	{
		velocity = velocity_arg;
	}
	
	const fgl::Vector2d& Collidable::getVelocity() const
	{
		return velocity;
	}
	
	void Collidable::applyForce(const fgl::Vector2d& force)
	{
		auto acceleration = force/getMass();
		velocity += acceleration;
	}

	bool Collidable::respondsToCollision(Collidable* collided, CollisionSide side) const
	{
		return true;
	}
	
	bool Collidable::checkCollision(Collidable* collidable) const
	{
		auto collisionRects1 = getCollisionRects();
		auto collisionRects2 = collidable->getCollisionRects();
		for(size_t i=0; i<collisionRects1.size(); i++)
		{
			for(size_t j=0; j<collisionRects2.size(); j++)
			{
				if(CollisionRect::checkCollision(collisionRects1[i], collisionRects2[j]))
				{
					return true;
				}
			}
		}
		return false;
	}
	
	void Collidable::onCollision(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onCollisionUpdate(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onCollisionFinish(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onFinishCollisionUpdates()
	{
		//
	}
}

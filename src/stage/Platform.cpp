
#include <fightlib/stage/Platform.hpp>
#include <fightlib/entity/Entity.hpp>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: Collidable(position),
		scale(1.0f)
	{
		setCollisionMethod(COLLISIONMETHOD_BOUNDS);
	}
	
	bool Platform::getFlag(const fgl::String& flag) const
	{
		if(flag=="Platform")
		{
			return true;
		}
		return Collidable::getFlag(flag);
	}
	
	void Platform::update(fgl::ApplicationData appData)
	{
		Collidable::update(appData);
		
		collisionRectManager.update(appData, this);
	}

	fgl::Vector2d Platform::getFriction(const fgl::ApplicationData& appData, Entity* entity, CollisionSide side) const
	{
		if(side==COLLISIONSIDE_TOP)
		{
			auto velocity = entity->getVelocity();
			if(fgl::Math::abs(velocity.x) < 100)
			{
				return fgl::Vector2d(-velocity.x, 0);
			}
			return fgl::Vector2d(-velocity.x*(12*appData.getFrameSpeedMultiplier()), 0);
		}
		return fgl::Vector2d(0, 0);
	}
	
	float Platform::getScale() const
	{
		return scale;
	}
	
	void Platform::setScale(float scale_arg)
	{
		scale = scale_arg;
	}
	
	float Platform::getDrawScale() const
	{
		return scale;
	}
	
	bool Platform::isStaticCollisionBody() const
	{
		return true;
	}
	
	fgl::ArrayList<CollisionRect*> Platform::getCollisionRects() const
	{
		return collisionRectManager.getCollisionRects();
	}
	
	void Platform::setCollisionMethod(CollisionMethod collisionMethod)
	{
		collisionRectManager.setCollisionMethod(collisionMethod);
	}
	
	CollisionMethod Platform::getCollisionMethod() const
	{
		return collisionRectManager.getCollisionMethod();
	}
}

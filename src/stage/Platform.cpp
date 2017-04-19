
#include <fightlib/stage/Platform.hpp>
#include <fightlib/entity/Entity.hpp>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: Collidable(position),
		position(position),
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
	
	void Platform::update(const fgl::ApplicationData& appData)
	{
		Collidable::update(appData);
		
		collisionRectManager.update(appData, this);
	}
	
	fgl::Vector2d Platform::getPosition(float* rotation) const
	{
		if(rotation!=nullptr)
		{
			*rotation = 0;
		}
		return position;
	}

	void Platform::shift(const fgl::Vector2d& offset)
	{
		position += offset;
	}

	fgl::Vector2d Platform::getFriction(Entity* entity, CollisionSide side) const
	{
		if(side==COLLISIONSIDE_TOP)
		{
			auto velocity = entity->getVelocity();
			return fgl::Vector2d(-velocity.x*12, 0);
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

	fgl::Vector2d Platform::getDrawPosition(float* rotation) const
	{
		if(rotation!=nullptr)
		{
			*rotation = 0;
		}
		return position;
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

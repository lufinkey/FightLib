
#include <fightlib/stage/Platform.hpp>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: Collidable(position),
		position(position),
		scale(1.0f)
	{
		setCollisionMethod(COLLISIONMETHOD_BOUNDS);
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
	
	void Platform::shift(const fgl::Vector2d& offset)
	{
		position += offset;
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

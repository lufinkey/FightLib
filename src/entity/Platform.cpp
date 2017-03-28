
#include <fightlib/entity/Platform.hpp>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: position(position)
	{
		setCollisionMethod(COLLISIONMETHOD_PIXEL);
	}
	
	void Platform::update(const fgl::ApplicationData& appData)
	{
		Collidable::update(appData);
		
		collisionRectManager.update(appData, this, fgl::Vector2d(0,0));
	}
	
	fgl::Vector2d Platform::getPosition(float* rotation) const
	{
		if(rotation!=nullptr)
		{
			*rotation = 0;
		}
		return position;
	}
	
	fgl::Vector2d Platform::getDrawPosition(float* rotation) const
	{
		if(rotation!=nullptr)
		{
			*rotation = 0;
		}
		return position;
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

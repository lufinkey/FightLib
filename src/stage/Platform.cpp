
#include <fightlib/stage/Platform.hpp>
#include <fightlib/entity/Entity.hpp>
#include <Box2D/Box2D.h>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: Collidable(position)
	{
		//
	}
	
	bool Platform::getFlag(const fgl::String& flag) const
	{
		if(flag=="Platform")
		{
			return true;
		}
		return Collidable::getFlag(flag);
	}
	
	bool Platform::isStaticCollisionBody() const
	{
		return true;
	}
}


#include <fightlib/stage/Platform.hpp>
#include <fightlib/entity/Entity.hpp>

namespace fl
{
	Platform::Platform(const fgl::Vector2d& position)
		: StageObject(position)
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

	fgl::Vector2d Platform::getFriction(const fgl::ApplicationData& appData, Entity* entity, CollisionSide side) const
	{
		if(side==COLLISIONSIDE_TOP)
		{
			auto velocity = entity->getVelocity();
			if(fgl::Math::abs(velocity.x) < 100)
			{
				return fgl::Vector2d(-velocity.x, 0);
			}
			return fgl::Vector2d(-velocity.x*(appData.getFrameSpeedMultiplier()/0.2), 0);
		}
		return fgl::Vector2d(0, 0);
	}

	bool Platform::isStaticCollisionBody() const
	{
		return true;
	}

	bool Platform::respondsToGravity() const
	{
		return false;
	}

	bool Platform::respondsToAirResistance() const
	{
		return false;
	}
}

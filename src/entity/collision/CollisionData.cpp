
#include <fightlib/entity/collision/CollisionData.hpp>
#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	CollisionSide CollisionSide_fromAngle(float angle)
	{
		angle = fgl::Math::normalizeDegrees(angle);
		if(angle > 315.0 || angle < 45.0)
		{
			return COLLISIONSIDE_TOP;
		}
		else if(angle >= 45.0 && angle <= 135.0)
		{
			return COLLISIONSIDE_RIGHT;
		}
		else if(angle > 135.0 && angle < 225.0)
		{
			return COLLISIONSIDE_BOTTOM;
		}
		else if(angle >= 225.0 && angle <= 315.0)
		{
			return COLLISIONSIDE_RIGHT;
		}
		throw fgl::IllegalArgumentException("angle", "I don't know what you did, but you fucked up");
	}

	CollisionSide CollisionSide_getOpposite(CollisionSide side)
	{
		switch(side)
		{
			case COLLISIONSIDE_LEFT:
			return COLLISIONSIDE_RIGHT;

			case COLLISIONSIDE_RIGHT:
			return COLLISIONSIDE_LEFT;

			case COLLISIONSIDE_TOP:
			return COLLISIONSIDE_BOTTOM;

			case COLLISIONSIDE_BOTTOM:
			return COLLISIONSIDE_TOP;
		}
		throw fgl::IllegalArgumentException("side", "invalid CollisionSide enum value");
	}

	CollisionData::CollisionData(float angle, const b2Fixture* fixture1, const b2Fixture* fixture2)
		: angle(angle),
		side(CollisionSide_fromAngle(angle)),
		fixture1(fixture1),
		fixture2(fixture2)
	{
		//
	}

	float CollisionData::getAngle() const
	{
		return angle;
	}

	CollisionSide CollisionData::getSide() const
	{
		return side;
	}

	const b2Fixture* CollisionData::getFixture1() const
	{
		return fixture1;
	}

	const b2Fixture* CollisionData::getFixture2() const
	{
		return fixture2;
	}

	CollisionData CollisionData::reversed() const
	{
		return CollisionData(fgl::Math::normalizeDegrees(angle+180.0), fixture2, fixture1);
	}
}

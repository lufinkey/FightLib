
#pragma once

class b2Fixture;

namespace fl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;

	CollisionSide CollisionSide_fromAngle(float angle);
	CollisionSide CollisionSide_getOpposite(CollisionSide side);

	class CollisionData
	{
	public:
		CollisionData(float angle, const b2Fixture* fixture1, const b2Fixture* fixture2);

		float getAngle() const;
		CollisionSide getSide() const;
		const b2Fixture* getFixture1() const;
		const b2Fixture* getFixture2() const;

		CollisionData reversed() const;

	private:
		float angle;
		CollisionSide side;
		const b2Fixture* fixture1;
		const b2Fixture* fixture2;
	};
}

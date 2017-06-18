
#pragma once

namespace box2d
{
	class Fixture;
}

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
		CollisionData(float angle, const box2d::Fixture* fixture1, const box2d::Fixture* fixture2);

		float getAngle() const;
		CollisionSide getSide() const;
		const box2d::Fixture* getFixture1() const;
		const box2d::Fixture* getFixture2() const;

		CollisionData reversed() const;

	private:
		float angle;
		CollisionSide side;
		const box2d::Fixture* fixture1;
		const box2d::Fixture* fixture2;
	};
}

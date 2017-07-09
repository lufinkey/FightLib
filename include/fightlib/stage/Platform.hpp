
#pragma once

#include <fightlib/entity/StageObject.hpp>

namespace fl
{
	class Entity;

	class Platform : public StageObject
	{
	public:
		Platform(const fgl::Vector2d& position);

		virtual bool getFlag(const fgl::String& flag) const override;

		virtual fgl::Vector2d getFriction(const fgl::ApplicationData& appData, Entity* entity, CollisionSide side) const;

		virtual bool isStaticCollisionBody() const override;
		virtual bool respondsToGravity() const override;
		virtual bool respondsToAirResistance() const override;
	};
}

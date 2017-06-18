
#pragma once

#include <fightlib/entity/collision/Collidable.hpp>

namespace fl
{
	class Entity;

	class Platform : public Collidable
	{
	public:
		Platform(const fgl::Vector2d& position);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		
		virtual bool isStaticCollisionBody() const override;
	};
}

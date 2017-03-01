
#pragma once

#include <fightlib/fightlib.hpp>

class Player : public fl::Entity
{
public:
	Player(const fgl::Vector2d& position, fl::Entity::Orientation orientation);

	virtual void update(fgl::ApplicationData appData) override;

protected:
	virtual void onCollision(fl::Entity* entity, fl::CollisionSide side) override;
};

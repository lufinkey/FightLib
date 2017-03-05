
#pragma once

#include <fightlib/fightlib.hpp>

class Player : public fl::Character
{
public:
	Player(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Entity::Orientation orientation);

	virtual void update(fgl::ApplicationData appData) override;

	virtual fgl::String getIdleAnimationName() const override;

protected:
	virtual void onCollision(fl::Entity* entity, fl::CollisionSide side) override;
};

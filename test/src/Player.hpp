
#pragma once

#include <fightlib/fightlib.hpp>

class Player : public fl::Character
{
public:
	Player(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation);

	virtual void update(const fgl::ApplicationData& appData) override;

	virtual fgl::String getIdleAnimationName() const override;
	virtual fgl::String getMoveAnimationName(double amount) const override;

protected:
	virtual void onCollision(fl::Collidable* collided, fl::CollisionSide side) override;
	virtual void onCollisionFinish(fl::Collidable* collided, fl::CollisionSide side) override;
};

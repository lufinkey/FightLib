
#pragma once

#include <fightlib/fightlib.hpp>

class MovingPlatform : public fl::Platform
{
public:
	MovingPlatform(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position);
	
	virtual void update(fgl::ApplicationData appData) override;
	
protected:
	virtual bool respondsToCollision(fl::Collidable* collided, fl::CollisionSide side) const override;
	
	virtual void onCollision(const fl::CollisionEvent& collisionEvent) override;
};


#pragma once

#include <fightlib/fightlib.hpp>

class MovingPlatform : public fl::Platform
{
public:
	MovingPlatform(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position);
	
	virtual void update(fgl::ApplicationData appData) override;
	
protected:
	virtual bool respondsToCollision(fgl::Collidable* collided, fgl::CollisionSide side) const override;
	
	virtual void onCollision(const fgl::CollisionEvent& collisionEvent) override;
};

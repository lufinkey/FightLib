
#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position) : Platform(position)
{
	loadAnimation("assets/animations/platform/block.plist", assetManager);
	changeAnimation("block");
	setVelocity(fgl::Vector2d(100, 0));
}

void MovingPlatform::update(const fgl::ApplicationData& appData)
{
	auto position = getPosition();
	if(position.x >= 600)
	{
		setVelocity(fgl::Vector2d(-100, 0));
	}
	else if(position.x <= 0)
	{
		setVelocity(fgl::Vector2d(100, 0));
	}
	Platform::update(appData);
}

bool MovingPlatform::respondsToCollision(fl::Collidable* collided, fl::CollisionSide side) const
{
	if(side==fl::COLLISIONSIDE_TOP)
	{
		return true;
	}
	return false;
}

void MovingPlatform::onCollision(const fl::CollisionEvent &collisionEvent)
{
	Platform::onCollision(collisionEvent);
}

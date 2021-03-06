
#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position) : Platform(position)
{
	loadAnimation("assets/animations/platform/block.plist", assetManager);
	changeAnimation("block");
	setVelocity(fgl::Vector2d(100, 0));
}

void MovingPlatform::update(fgl::ApplicationData appData)
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

bool MovingPlatform::respondsToCollision(fgl::Collidable* collided, fgl::CollisionSide side, fgl::CollisionRectPair rectPair) const
{
	if(side==fgl::COLLISIONSIDE_TOP)
	{
		return true;
	}
	return false;
}

void MovingPlatform::onCollision(const fgl::CollisionEvent &collisionEvent)
{
	Platform::onCollision(collisionEvent);
}

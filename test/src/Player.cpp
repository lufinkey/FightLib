
#include "Player.hpp"
#include "JumpAction.hpp"

Player::Player(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation)
	: Character(position, orientation)
{
	setScale(3.0);
	setCollisionMethod(fl::COLLISIONMETHOD_BOUNDS);

	loadAnimation("assets/animations/idle.plist", assetManager);
	loadAnimation("assets/animations/walk.plist", assetManager);
	loadAnimation("assets/animations/jump.plist", assetManager);
	loadAnimation("assets/animations/punch.plist", assetManager);
	changeAnimation("idle");
	
	addAction("jump", new JumpAction());
}

void Player::update(const fgl::ApplicationData& appData)
{
	fgl::Vector2f direction = getDirection();
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::LEFTARROW))
	{
		direction.x = -1;
	}
	else if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::RIGHTARROW))
	{
		direction.x = 1;
	}
	else
	{
		direction.x = 0;
	}
	setDirection(direction);

	fgl::Vector2d velocity = getVelocity();
	velocity.y += (1800*appData.getFrameSpeedMultiplier());
	setVelocity(velocity);
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::UPARROW) && !fgl::Keyboard::wasKeyPressed(fgl::Keyboard::UPARROW))
	{
		performAction("jump");
	}

	Character::update(appData);
}

fgl::String Player::getIdleAnimationName() const
{
	return "idle";
}

fgl::String Player::getMoveAnimationName(double amount) const
{
	return "walk";
}

void Player::onCollision(fl::Collidable* entity, fl::CollisionSide side)
{
	if(side==fl::COLLISIONSIDE_TOP)
	{
		fgl::Vector2d velocity = getVelocity();
		velocity.y = 0;
		setVelocity(velocity);
	}
}

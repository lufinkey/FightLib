
#include "Player.hpp"

Player::Player(const fgl::Vector2d& position, fl::Entity::Orientation orientation)
	: Character(position, orientation)
{
	//
}

void Player::update(fgl::ApplicationData appData)
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
	velocity.y += (100*appData.getFrameSpeedMultiplier());
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::UPARROW) && !fgl::Keyboard::wasKeyPressed(fgl::Keyboard::UPARROW))
	{
		velocity.y = -100;
	}
	setVelocity(velocity);

	Character::update(appData);
}

void Player::onCollision(fl::Entity* entity, fl::CollisionSide side)
{
	if(side==fl::COLLISIONSIDE_TOP)
	{
		fgl::Vector2d velocity = getVelocity();
		velocity.y = 0;
		setVelocity(velocity);
	}
}

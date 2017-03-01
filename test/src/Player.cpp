
#include "Player.hpp"

Player::Player(const fgl::Vector2d& position, fl::Entity::Orientation orientation)
	: Entity(position, orientation)
{
	//
}

void Player::update(fgl::ApplicationData appData)
{
	fgl::Vector2d velocity = getVelocity();
	velocity.y += (100*appData.getFrameSpeedMultiplier());

	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::LEFTARROW))
	{
		velocity.x = -20;
	}
	else if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::RIGHTARROW))
	{
		velocity.x = 20;
	}
	else
	{
		velocity.x = 0;
	}

	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::UPARROW) && !fgl::Keyboard::wasKeyPressed(fgl::Keyboard::UPARROW))
	{
		velocity.y = -100;
	}

	setVelocity(velocity);

	Entity::update(appData);
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

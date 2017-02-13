
#include "FightGame.h"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	character = new fl::Entity(300, 200, fl::Entity::ORIENTATION_LEFT);
	character->setScale(3.0);
	fgl::String animationError;
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");
	character->setCollisionMethod(fl::Entity::COLLISIONMETHOD_PIXEL);

	sword = new fl::Entity(200, 200, fl::Entity::ORIENTATION_LEFT);
	sword->setScale(2.0);
	fgl::String swordAnimationError;
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->changeAnimation("sword");
	sword->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);
}

void FightGame::update(fgl::ApplicationData appData)
{
	double speed = 40;
	fgl::Vector2d velocity;
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::UPARROW))
	{
		velocity.y -= speed;
	}
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::DOWNARROW))
	{
		velocity.y += speed;
	}
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::LEFTARROW))
	{
		velocity.x -= speed;
	}
	if(fgl::Keyboard::isKeyPressed(fgl::Keyboard::RIGHTARROW))
	{
		velocity.x += speed;
	}
	character->setVelocity(velocity);

	character->update(appData);
	sword->update(appData);

	fl::CollisionRect* characterCollisionRect = character->createCollisionRect();
	fl::CollisionRect* swordCollisionRect = sword->createCollisionRect();
	fgl::Vector2d offset = fl::CollisionRect::checkCollision(characterCollisionRect, swordCollisionRect);
	if(offset.x!=0 || offset.y!=0)
	{
		character->x -= offset.x;
		character->y -= offset.y;
	}
	delete characterCollisionRect;
	delete swordCollisionRect;
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	character->draw(appData, graphics);
	sword->draw(appData, graphics);
}

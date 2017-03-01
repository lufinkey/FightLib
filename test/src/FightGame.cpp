
#include "FightGame.hpp"
#include "Player.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	fgl::FileTools::setCurrentWorkingDirectory(getResourceDirectory());
	
	character = new Player(fgl::Vector2d(300, 200), fl::Entity::ORIENTATION_LEFT);
	character->setScale(3.0);
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");
	character->setCollisionMethod(fl::Entity::COLLISIONMETHOD_BOUNDS);

	block1 = new fl::Entity(fgl::Vector2d(300, 300), fl::Entity::ORIENTATION_LEFT);
	block1->loadAnimation("assets/animations/block.plist", assetManager);
	block1->changeAnimation("block");
	block1->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);
	block1->setStaticCollisionBody(true);

	block2 = new fl::Entity(fgl::Vector2d(350, 300), fl::Entity::ORIENTATION_LEFT);
	block2->loadAnimation("assets/animations/block.plist", assetManager);
	block2->changeAnimation("block");
	block2->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);
	block2->setStaticCollisionBody(true);

	block3 = new fl::Entity(fgl::Vector2d(250, 300), fl::Entity::ORIENTATION_LEFT);
	block3->loadAnimation("assets/animations/block.plist", assetManager);
	block3->changeAnimation("block");
	block3->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);
	block3->setStaticCollisionBody(true);

	block4 = new fl::Entity(fgl::Vector2d(200, 250), fl::Entity::ORIENTATION_LEFT);
	block4->loadAnimation("assets/animations/block.plist", assetManager);
	block4->changeAnimation("block");
	block4->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);
	block4->setStaticCollisionBody(true);

	collisionManager.addEntity(character);
	collisionManager.addEntity(block1);
	collisionManager.addEntity(block2);
	collisionManager.addEntity(block3);
	collisionManager.addEntity(block4);
}

void FightGame::update(fgl::ApplicationData appData)
{
	character->update(appData);
	block1->update(appData);
	block2->update(appData);
	block3->update(appData);
	block4->update(appData);

	collisionManager.update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	character->draw(appData, graphics);
	block1->draw(appData, graphics);
	block2->draw(appData, graphics);
	block3->draw(appData, graphics);
	block4->draw(appData, graphics);
}


#include "FightGame.hpp"
#include "Player.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	animationAssetManager = new fl::AnimationAssetManager(assetManager);

	character = new Player(animationAssetManager, fgl::Vector2d(300, 200), fl::ORIENTATION_LEFT);

	sword = new fl::Entity(fgl::Vector2d(0, 0), fl::ORIENTATION_LEFT);
	sword->loadAnimation("assets/animations/sword.plist", animationAssetManager);
	sword->setScale(2.0);
	sword->changeAnimation("sword");
	
	character->anchorChildEntity(sword, fl::AnimationMetaPoint::POINTTYPE_HANDLE, 0, fl::AnimationMetaPoint::POINTTYPE_LEFTHAND, 0);

	block1 = new fl::Platform(fgl::Vector2d(300, 300));
	block1->loadAnimation("assets/animations/block.plist", animationAssetManager);
	block1->changeAnimation("block");
	block1->setCollisionMethod(fl::COLLISIONMETHOD_FRAME);

	block2 = new fl::Platform(fgl::Vector2d(350, 300));
	block2->loadAnimation("assets/animations/block.plist", animationAssetManager);
	block2->changeAnimation("block");
	block2->setCollisionMethod(fl::COLLISIONMETHOD_FRAME);

	block3 = new fl::Platform(fgl::Vector2d(250, 300));
	block3->loadAnimation("assets/animations/block.plist", animationAssetManager);
	block3->changeAnimation("block");
	block3->setCollisionMethod(fl::COLLISIONMETHOD_FRAME);

	block4 = new fl::Platform(fgl::Vector2d(200, 250));
	block4->loadAnimation("assets/animations/block.plist", animationAssetManager);
	block4->changeAnimation("block");
	block4->setCollisionMethod(fl::COLLISIONMETHOD_FRAME);

	collisionManager.addCollidable(character);
	collisionManager.addCollidable(block1);
	collisionManager.addCollidable(block2);
	collisionManager.addCollidable(block3);
	collisionManager.addCollidable(block4);
}

void FightGame::update(fgl::ApplicationData appData)
{
	character->update(appData);
	sword->update(appData);
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


#include "FightGame.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	character = new fl::Entity(fgl::Vector2d(300, 200), fl::Entity::ORIENTATION_LEFT);
	character->setScale(3.0);
	fgl::String animationError;
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");
	character->setCollisionMethod(fl::Entity::COLLISIONMETHOD_PIXEL);

	sword = new fl::Entity(fgl::Vector2d(200, 200), fl::Entity::ORIENTATION_RIGHT);
	sword->setScale(2.0);
	fgl::String swordAnimationError;
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->changeAnimation("sword");
	sword->setCollisionMethod(fl::Entity::COLLISIONMETHOD_FRAME);

	//character->anchorChildEntity(sword, fl::AnimationMetaPoint::POINTTYPE_HANDLE, 0, fl::AnimationMetaPoint::POINTTYPE_LEFTHAND, 0);
}

void FightGame::update(fgl::ApplicationData appData)
{
	character->update(appData);
	sword->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	character->draw(appData, graphics);
	sword->draw(appData, graphics);
}

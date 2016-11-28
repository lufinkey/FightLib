
#include "FightGame.h"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	character = new fl::Entity(300, 200);
	fgl::String animationError;
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");

	sword = new fl::Entity(200, 200);
	fgl::String swordAnimationError;
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->changeAnimation("sword");
	
	character->anchorChildEntity(sword, fl::AnimationMetaPoint::POINTTYPE_HANDLE, 0, fl::AnimationMetaPoint::POINTTYPE_LEFTHAND, 0);
}

void FightGame::update(fgl::ApplicationData appData)
{
	character->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	character->draw(appData, graphics);
	fgl::Vector2d position = sword->getPosition();
	graphics.setColor(fgl::Color::RED);
	graphics.fillRect(position.x-10, position.y-10, 20, 20);
}

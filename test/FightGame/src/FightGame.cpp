
#include "FightGame.h"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	character = new fl::Entity(300, 200);
	character->setScale(3.0);
	fgl::String animationError;
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");

	sword = new fl::Entity(200, 200);
	sword->setScale(2.0);
	fgl::String swordAnimationError;
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->changeAnimation("sword");
	
	character->anchorChildEntity(sword, fl::AnimationMetaPoint::POINTTYPE_HANDLE, 0, fl::AnimationMetaPoint::POINTTYPE_RIGHTHAND, 0);
}

void FightGame::update(fgl::ApplicationData appData)
{
	character->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	character->draw(appData, graphics);
	float rotation = 0;
	fgl::Vector2d position = sword->getPosition(&rotation);
	graphics.setColor(fgl::Color::RED);
	graphics.rotate(rotation, position);
	graphics.fillRect(position.x-2, position.y-2, 4, 4);
}

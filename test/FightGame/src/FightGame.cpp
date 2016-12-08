
#include "FightGame.h"

static bool colliding = false;

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	character = new fl::Entity(300, 200, fl::Entity::ORIENTATION_RIGHT);
	character->setScale(3.0);
	fgl::String animationError;
	character->loadAnimation("assets/animations/punch.plist", assetManager);
	character->changeAnimation("punch");

	sword = new fl::Entity(200, 200, fl::Entity::ORIENTATION_RIGHT);
	sword->setScale(2.0);
	fgl::String swordAnimationError;
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->changeAnimation("sword");
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

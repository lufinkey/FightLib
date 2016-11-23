
#include "FightGame.h"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	entity = new fl::Entity(300, 200);
	fgl::String animationError;
	entity->loadAnimation("assets/animations/punch.plist", assetManager, &animationError);
	fgl::Console::writeErrorLine(animationError);
	entity->changeAnimation("punch", [](fl::AnimationEventType eventType){
		switch(eventType)
		{
			case fl::ANIMATIONEVENT_CHANGED:
			fgl::Console::writeLine("changed");
			break;

			case fl::ANIMATIONEVENT_FINISHED:
			fgl::Console::writeLine("finished");
			break;

			case fl::ANIMATIONEVENT_FRAMECHANGED:
			fgl::Console::writeLine("frame changed");
			break;
		}
	});
}

void FightGame::update(fgl::ApplicationData appData)
{
	entity->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	entity->draw(appData, graphics);
}

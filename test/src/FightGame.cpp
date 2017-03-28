
#include "FightGame.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	animationAssetManager = new fl::AnimationAssetManager(assetManager);

	testStage = new TestStage(animationAssetManager);
}

void FightGame::update(fgl::ApplicationData appData)
{
	testStage->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	testStage->draw(appData, graphics);
}

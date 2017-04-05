
#include "FightGame.hpp"
#include "TestStage.hpp"
#include "Player.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	setFPS(60);
	
	animationAssetManager = new fl::AnimationAssetManager(assetManager);
	
	auto player = new Player(animationAssetManager, fgl::Vector2d(300, 200), fl::ORIENTATION_LEFT);
	auto controller = new fl::KeyboardCharacterController(player);
	controller->setKeyDownAction(fgl::Keyboard::UPARROW, "jump");
	
	fl::FightParams params;
	params.setStage(new TestStage(animationAssetManager));
	params.addCharacter(player);
	params.addCharacterController(controller);
	
	fight = new fl::Fight(params);
}

void FightGame::update(fgl::ApplicationData appData)
{
	fight->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	fight->draw(appData, graphics);
}

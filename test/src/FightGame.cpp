
#include "FightGame.hpp"
#include "TestStage.hpp"
#include "Player.hpp"

void FightGame::loadContent(fgl::AssetManager* assetManager)
{
	setFPS(60);

	fgl::Console::writeLine(assetManager->getRootDirectory());
	animationAssetManager = new fl::AnimationAssetManager(assetManager->getWindow(), assetManager->getRootDirectory());

	auto player = new Player(animationAssetManager, fgl::Vector2d(300, 200), fl::ORIENTATION_LEFT);
	auto controller = new fl::KeyboardCharacterController(player);
	controller->setKeyDownAction(fgl::Keyboard::UPARROW, "jump");
	controller->setKeyDownAction(fgl::Keyboard::P, "pickUp");
	controller->setKeyDownAction(fgl::Keyboard::O, "punch");

	auto stage = new TestStage(animationAssetManager);
	auto camera = new fl::FollowerCamera();
	camera->setFocus(player);

	fl::FightParams params;
	params.setStage(stage);
	params.setCamera(camera);
	params.addCharacter(player);
	params.addCharacterController(controller);

	fight = new fl::Fight(params);
}

void FightGame::unloadContent(fgl::AssetManager* assetManager)
{
	delete fight;
	delete animationAssetManager;
}

void FightGame::update(fgl::ApplicationData appData)
{
	fight->update(appData);
}

void FightGame::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	fight->draw(appData, graphics);
}

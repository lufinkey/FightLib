
#include "TestStage.hpp"
#include "MovingPlatform.hpp"
#include "Player.hpp"
#include "Sword.hpp"
#include "Sandbag.hpp"

TestStage::TestStage(fl::AnimationAssetManager* assetManager)
{
	sword = new Sword(assetManager, fgl::Vector2d(218, 0), fl::ORIENTATION_LEFT);

	sandbag = new Sandbag(assetManager, fgl::Vector2d(350, 0), fl::ORIENTATION_LEFT);
	
	//character->anchorChildEntity(sword, fl::METAPOINT_HANDLE, 0, fl::METAPOINT_LEFTHAND, 0);
	
	auto block1 = new fl::Platform(fgl::Vector2d(200, 170));
	block1->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block1->changeAnimation("block");
	
	auto block2 = new fl::Platform(fgl::Vector2d(200, 120));
	block2->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block2->changeAnimation("block");

	auto block3 = new fl::Platform(fgl::Vector2d(200, 220));
	block3->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block3->changeAnimation("block");
	
	auto block4 = new fl::Platform(fgl::Vector2d(250, 300));
	block4->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block4->changeAnimation("block");
	
	auto block5 = new fl::Platform(fgl::Vector2d(300, 300));
	block5->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block5->changeAnimation("block");
	
	auto block6 = new fl::Platform(fgl::Vector2d(350, 300));
	block6->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block6->changeAnimation("block");
	
	auto block7 = new fl::Platform(fgl::Vector2d(400, 300));
	block7->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block7->changeAnimation("block");
	
	movingBlock = new MovingPlatform(assetManager, fgl::Vector2d(400, 150));
	
	addPlatform(block1);
	addPlatform(block2);
	addPlatform(block3);
	addPlatform(block4);
	addPlatform(block5);
	addPlatform(block6);
	addPlatform(block7);
	addPlatform(movingBlock);
	addItem(sword);
	addCharacter(sandbag);
}

TestStage::~TestStage()
{
	//
}

void TestStage::update(fgl::ApplicationData appData)
{
	for(auto character : getCharacters())
	{
		auto window = appData.getWindow();
		auto winSize = window->getSize();
		auto position = character->getPosition();
		auto newPosition = position;
		if(newPosition.x < -40)
		{
			newPosition.x = (double)winSize.x+40;
		}
		else if(newPosition.x > (winSize.x+40))
		{
			newPosition.x = -40;
		}
		if(newPosition.y < -40)
		{
			newPosition.y = (double)winSize.y+40;
		}
		else if(newPosition.y > (winSize.y+40))
		{
			newPosition.y = -40;
		}
		character->setPosition(newPosition);
	}
	Stage::update(appData);
}

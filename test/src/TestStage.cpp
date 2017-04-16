
#include "TestStage.hpp"
#include "Player.hpp"
#include "Sword.hpp"

TestStage::TestStage(fl::AnimationAssetManager* assetManager)
{
	sword = new Sword(assetManager, fgl::Vector2d(218, 0), fl::ORIENTATION_LEFT);
	
	//character->anchorChildEntity(sword, fl::METAPOINT_HANDLE, 0, fl::METAPOINT_LEFTHAND, 0);
	
	auto block1 = new fl::Platform(fgl::Vector2d(200, 220));
	block1->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block1->changeAnimation("block");
	
	auto block2 = new fl::Platform(fgl::Vector2d(200, 170));
	block2->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block2->changeAnimation("block");
	
	auto block3 = new fl::Platform(fgl::Vector2d(250, 300));
	block3->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block3->changeAnimation("block");
	
	auto block4 = new fl::Platform(fgl::Vector2d(300, 300));
	block4->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block4->changeAnimation("block");
	
	auto block5 = new fl::Platform(fgl::Vector2d(350, 300));
	block5->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block5->changeAnimation("block");
	
	auto block6 = new fl::Platform(fgl::Vector2d(400, 300));
	block6->loadAnimation("assets/animations/platform/block.plist", assetManager);
	block6->changeAnimation("block");
	
	movingBlock = new fl::Platform(fgl::Vector2d(400, 250));
	movingBlock->loadAnimation("assets/animations/platform/block.plist", assetManager);
	movingBlock->changeAnimation("block");
	movingBlock->setVelocity(fgl::Vector2d(100, 0));
	
	addPlatform(block1);
	addPlatform(block2);
	addPlatform(block3);
	addPlatform(block4);
	addPlatform(block5);
	addPlatform(block6);
	addPlatform(movingBlock);
	addItem(sword);
}

TestStage::~TestStage()
{
	//
}

void TestStage::update(fgl::ApplicationData appData)
{
	fgl::Vector2d position = movingBlock->getPosition();
	if(position.x >= 600)
	{
		movingBlock->setVelocity(fgl::Vector2d(-100, 0));
	}
	else if(position.x <= 0)
	{
		movingBlock->setVelocity(fgl::Vector2d(100, 0));
	}
	Stage::update(appData);
}

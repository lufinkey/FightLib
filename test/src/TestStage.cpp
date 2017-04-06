
#include "TestStage.hpp"
#include "Player.hpp"
#include "Sword.hpp"

TestStage::TestStage(fl::AnimationAssetManager* assetManager)
{
	sword = new Sword(fgl::Vector2d(218, 0), fl::ORIENTATION_LEFT);
	sword->loadAnimation("assets/animations/sword.plist", assetManager);
	sword->setScale(2.0);
	sword->changeAnimation("sword");
	
	//character->anchorChildEntity(sword, fl::AnimationMetaPoint::POINTTYPE_HANDLE, 0, fl::AnimationMetaPoint::POINTTYPE_LEFTHAND, 0);
	
	block1 = new fl::Platform(fgl::Vector2d(300, 300));
	block1->loadAnimation("assets/animations/block.plist", assetManager);
	block1->changeAnimation("block");
	
	block2 = new fl::Platform(fgl::Vector2d(200, 170));
	block2->loadAnimation("assets/animations/block.plist", assetManager);
	block2->changeAnimation("block");
	
	block3 = new fl::Platform(fgl::Vector2d(250, 300));
	block3->loadAnimation("assets/animations/block.plist", assetManager);
	block3->changeAnimation("block");
	
	block4 = new fl::Platform(fgl::Vector2d(200, 220));
	block4->loadAnimation("assets/animations/block.plist", assetManager);
	block4->changeAnimation("block");
	
	addPlatform(block1);
	addPlatform(block2);
	addPlatform(block3);
	addPlatform(block4);
	//addEntity(sword);
}

TestStage::~TestStage()
{
	//
}

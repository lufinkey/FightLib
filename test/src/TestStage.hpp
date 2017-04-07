
#pragma once

#include <fightlib/fightlib.hpp>

class TestStage : public fl::Stage
{
public:
	TestStage(fl::AnimationAssetManager* assetManager);
	virtual ~TestStage();
	
	virtual void update(fgl::ApplicationData appData) override;
	
private:
	fl::Item* sword;
	
	fl::Platform* movingBlock;
};

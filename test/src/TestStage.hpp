
#include <fightlib/fightlib.hpp>

class TestStage : public fl::Stage
{
public:
	TestStage(fl::AnimationAssetManager* assetManager);
	virtual ~TestStage();
	
private:
	fl::Character* character;
	fl::Entity* sword;
	
	fl::Platform* block1;
	fl::Platform* block2;
	fl::Platform* block3;
	fl::Platform* block4;
};

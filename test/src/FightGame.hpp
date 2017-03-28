
#pragma once

#include <fightlib/fightlib.hpp>

class FightGame : public fgl::Application
{
public:
	virtual void loadContent(fgl::AssetManager* assetManager) override;
	virtual void update(fgl::ApplicationData appData) override;
	virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

private:
	fl::AnimationAssetManager* animationAssetManager;

	fl::Character* character;
	fl::Entity* sword;

	fl::Platform* block1;
	fl::Platform* block2;
	fl::Platform* block3;
	fl::Platform* block4;

	fl::CollisionManager collisionManager;
};

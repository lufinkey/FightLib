
#pragma once

#include <fightlib/fightlib.hpp>

class FightGame : public fgl::Application
{
public:
	virtual void loadContent(fgl::AssetManager* assetManager) override;
	virtual void unloadContent(fgl::AssetManager* assetManager) override;
	virtual void update(fgl::ApplicationData appData) override;
	virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

private:
	fl::AnimationAssetManager* animationAssetManager;

	fl::Fight* fight;
};


#pragma once

#include <GameLibrary/GameLibrary.h>
#include <fightlib/fightlib.h>

class FightGame : public fgl::Application
{
public:
	virtual void loadContent(fgl::AssetManager* assetManager) override;
	virtual void update(fgl::ApplicationData appData) override;
	virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

private:
	fl::Entity* entity;
};

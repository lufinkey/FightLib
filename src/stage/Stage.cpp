
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	Stage::Stage()
	{
		//
	}
	
	Stage::~Stage()
	{
		//
	}
	
	void Stage::update(fgl::ApplicationData appData)
	{
		appData.additionalData["stage"] = this;
		drawManager.update(appData);
		collisionManager.update(appData);
	}
	
	void Stage::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		appData.additionalData["stage"] = this;
		drawManager.draw(appData, graphics);
	}
	
	void Stage::addPlatform(Platform* platform)
	{
		collisionManager.addCollidable(platform);
		drawManager.addDrawable(platform);
	}
	
	void Stage::removePlatform(Platform* platform)
	{
		collisionManager.removeCollidable(platform);
		drawManager.removeDrawable(platform);
	}
	
	void Stage::addEntity(Entity* entity)
	{
		collisionManager.addCollidable(entity);
		drawManager.addDrawable(entity);
	}
	
	void Stage::removeEntity(Entity* entity)
	{
		collisionManager.removeCollidable(entity);
		drawManager.removeDrawable(entity);
	}
}

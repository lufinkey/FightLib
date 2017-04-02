
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
		for(auto entity : entities)
		{
			if(entity->getParentEntity()==nullptr)
			{
				double gravity = getGravity(entity);
				auto velocity = entity->getVelocity();
				velocity.y += gravity*appData.getFrameSpeedMultiplier();
				entity->setVelocity(velocity);
			}
		}
		drawManager.update(appData);
		collisionManager.update(appData);
	}
	
	void Stage::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		appData.additionalData["stage"] = this;
		drawManager.draw(appData, graphics);
	}
	
	double Stage::getGravity(Entity* entity) const
	{
		return 1800;
	}
	
	void Stage::addPlatform(Platform* platform, double zLayer)
	{
		platforms.add(platform);
		collisionManager.addCollidable(platform);
		drawManager.addDrawable(platform, zLayer);
	}
	
	void Stage::removePlatform(Platform* platform)
	{
		size_t index = platforms.indexOf(platform);
		if(index!=-1)
		{
			platforms.remove(index);
		}
		collisionManager.removeCollidable(platform);
		drawManager.removeDrawable(platform);
	}
	
	void Stage::addEntity(Entity* entity, double zLayer)
	{
		entities.add(entity);
		collisionManager.addCollidable(entity);
		drawManager.addDrawable(entity, zLayer);
	}
	
	void Stage::removeEntity(Entity* entity)
	{
		size_t index = entities.indexOf(entity);
		if(index!=-1)
		{
			entities.remove(index);
		}
		collisionManager.removeCollidable(entity);
		drawManager.removeDrawable(entity);
	}

	void Stage::addItem(Item* item, double zLayer)
	{
		items.add(item);
		entities.add(item);
		collisionManager.addCollidable(item);
		drawManager.addDrawable(item, zLayer);
	}

	void Stage::removeItem(Item* item)
	{
		size_t itemIndex = items.indexOf(item);
		if(itemIndex!=-1)
		{
			items.remove(itemIndex);
		}
		size_t entityIndex = entities.indexOf(item);
		if(entityIndex!=-1)
		{
			entities.remove(entityIndex);
		}
		collisionManager.removeCollidable(item);
		drawManager.removeDrawable(item);
	}

	const fgl::ArrayList<Item*>& Stage::getItems() const
	{
		return items;
	}
}

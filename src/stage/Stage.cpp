
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	Stage::Stage()
	{
		//
	}
	
	Stage::~Stage()
	{
		for(auto entity : entities)
		{
			delete entity;
		}
		for(auto platform : platforms)
		{
			delete platform;
		}
	}
	
	void Stage::update(fgl::ApplicationData appData)
	{
		appData.additionalData["stage"] = this;
		for(auto entity : entities)
		{
			if(entity->getParentEntity()==nullptr && entity->respondsToGravity())
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

	const fgl::ArrayList<Platform*>& Stage::getPlatforms() const
	{
		return platforms;
	}
	
	void Stage::addEntity(Entity* entity, double zLayer)
	{
		if(entity->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("entity", "cannot be added to multiple Stage objects");
		}
		entities.add(entity);
		collisionManager.addCollidable(entity);
		drawManager.addDrawable(entity, zLayer);
	}
	
	void Stage::removeEntity(Entity* entity)
	{
		if(entity->stage!=nullptr && entity->stage!=this)
		{
			throw fgl::IllegalArgumentException("entity", "belongs to a different Stage");
		}
		entity->stage = nullptr;
		size_t index = entities.indexOf(entity);
		if(index!=-1)
		{
			entities.remove(index);
		}
		collisionManager.removeCollidable(entity);
		drawManager.removeDrawable(entity);
	}

	const fgl::ArrayList<Entity*>& Stage::getEntities() const
	{
		return entities;
	}

	void Stage::addItem(Item* item, double zLayer)
	{
		if(item->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("item", "cannot be added to multiple Stage objects");
		}
		addEntity(item, zLayer);
		items.add(item);
	}

	void Stage::removeItem(Item* item)
	{
		if(item->stage!=nullptr && item->stage!=this)
		{
			throw fgl::IllegalArgumentException("item", "belongs to a different Stage");
		}
		removeEntity(item);
		size_t itemIndex = items.indexOf(item);
		if(itemIndex!=-1)
		{
			items.remove(itemIndex);
		}
	}

	const fgl::ArrayList<Item*>& Stage::getItems() const
	{
		return items;
	}

	void Stage::addCharacter(Character* character, double zLayer)
	{
		if(character->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("item", "cannot be added to multiple Stage objects");
		}
		addEntity(character, zLayer);
		characters.add(character);
	}

	void Stage::removeCharacter(Character* character)
	{
		if(character->stage!=nullptr && character->stage!=this)
		{
			throw fgl::IllegalArgumentException("character", "belongs to a different Stage");
		}
		removeEntity(character);
		size_t characterIndex = characters.indexOf(character);
		if(characterIndex!=-1)
		{
			characters.remove(characterIndex);
		}
	}

	const fgl::ArrayList<Character*>& Stage::getCharacters() const
	{
		return characters;
	}
}

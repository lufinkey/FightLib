
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	Stage::Stage() : fight(nullptr), parentStage(nullptr)
	{
		//
	}

	Stage::~Stage()
	{
		for(auto section : sections)
		{
			unloadSection(section);
			delete section;
		}
		for(auto controller : controllers)
		{
			delete controller;
		}
		for(auto drawable : drawManager.getDrawables())
		{
			delete drawable;
		}
	}

	bool Stage::getFlag(const fgl::String& flag) const
	{
		if(flag=="Stage")
		{
			return true;
		}
		return Drawable::getFlag(flag);
	}

	void Stage::update(fgl::ApplicationData appData)
	{
		appData.additionalData["stage"] = this;

		//update gravity
		for(auto entity : entities)
		{
			if(entity->getParentEntity()==nullptr && entity->respondsToGravity())
			{
				auto velocity = entity->getVelocity();

				velocity += getGravity(entity)*entity->getGravityScale()*appData.getFrameSpeedMultiplier();
				if(!entity->isOnGround())
				{
					velocity += getAirResistance(entity)*appData.getFrameSpeedMultiplier();
				}

				entity->setVelocity(velocity);
			}
		}

		//update the stage controllers
		for(auto controller : controllers)
		{
			controller->update(appData);
		}

		//update all these damn managers
		drawManager.update(appData);
		hitboxCollisionManager.update(appData);
		collisionManager.update(appData);

		//update the list of items that characters are able to pick up
		fgl::BasicDictionary<Character*, fgl::ArrayList<Item*>> accessibleItems;
		for(auto character : characters)
		{
			for(auto item : items)
			{
				if(item->getParentCharacter()==nullptr && character->checkCollision(item))
				{
					fgl::ArrayList<Item*>& itemList = accessibleItems[character];
					itemList.add(item);
				}
			}
		}
		characterAccessibleItems = accessibleItems;
	}

	void Stage::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		appData.additionalData["stage"] = this;
		drawManager.draw(appData, graphics);
	}

	fgl::Vector2d Stage::getGravity(Entity* entity) const
	{
		return fgl::Vector2d(0, 1800);
	}

	fgl::Vector2d Stage::getAirResistance(Entity* entity) const
	{
		auto velocity = entity->getVelocity();
		return fgl::Vector2d(-velocity.x*2, 0);
	}

	Fight* Stage::getFight() const
	{
		return fight;
	}

	Stage* Stage::getParentStage() const
	{
		return parentStage;
	}

	fgl::ArrayList<Item*> Stage::getAccessibleItems(fl::Character* character) const
	{
		return characterAccessibleItems.get(character, {});
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

	void Stage::addDrawable(Drawable* drawable, double zLayer)
	{
		drawManager.addDrawable(drawable, zLayer);
	}

	void Stage::removeDrawable(Drawable* drawable)
	{
		drawManager.removeDrawable(drawable);
	}

	void Stage::addEntity(Entity* entity, double zLayer)
	{
		if(entity->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("entity", "cannot be added to multiple Stage objects");
		}
		entity->stage = this;
		entities.add(entity);
		collisionManager.addCollidable(entity);
		hitboxCollisionManager.addEntity(entity);
		drawManager.addDrawable(entity, zLayer);
		entity->onAddToStage(this);
	}

	void Stage::removeEntity(Entity* entity)
	{
		if(entity->stage!=this)
		{
			return;
		}
		entity->stage = nullptr;
		size_t index = entities.indexOf(entity);
		if(index!=-1)
		{
			entities.remove(index);
		}
		collisionManager.removeCollidable(entity);
		hitboxCollisionManager.removeEntity(entity);
		drawManager.removeDrawable(entity);
		entity->onRemoveFromStage(this);
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
		if(item->stage!=this)
		{
			return;
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
			throw fgl::IllegalArgumentException("character", "cannot be added to multiple Stage objects");
		}
		addEntity(character, zLayer);
		characters.add(character);
	}

	void Stage::removeCharacter(Character* character)
	{
		if(character->stage!=this)
		{
			return;
		}
		removeEntity(character);
		size_t characterIndex = characters.indexOf(character);
		if(characterIndex!=-1)
		{
			characters.remove(characterIndex);
		}
	}

	void Stage::addSection(StageSection* section)
	{
		if(section->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("section", "already added to a Stage");
		}
		sections.add(section);
		section->stage = this;
		loadSection(section);
	}

	void Stage::removeSection(StageSection* section)
	{
		if(section->stage!=this)
		{
			return;
		}
		unloadSection(section);
		sections.removeFirstEqual(section);
		section->stage = nullptr;
	}

	void Stage::addSubStage(Stage* stage, double zLayer)
	{
		if(stage->parentStage!=nullptr)
		{
			throw fgl::IllegalArgumentException("stage", "already added to a Stage");
		}
		subStages.add(stage);
		drawManager.addDrawable(stage);
		stage->parentStage = this;
	}

	void Stage::removeSubStage(Stage* stage)
	{
		if(stage->parentStage!=this)
		{
			return;
		}
		subStages.removeFirstEqual(stage);
		drawManager.removeDrawable(stage);
		stage->parentStage = nullptr;
	}

	void Stage::addController(StageController* controller)
	{
		if(controller->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("controller", "already added to a Stage");
		}
		controllers.add(controller);
		controller->stage = this;
		controller->onAddToStage(this);
	}

	void Stage::removeController(StageController* controller)
	{
		if(controller->stage!=this)
		{
			return;
		}
		controllers.removeFirstEqual(controller);
		controller->stage = nullptr;
		controller->onRemoveFromStage(this);
	}

	void Stage::loadSection(StageSection* section)
	{
		if(section->loaded)
		{
			return;
		}
		//TODO saving and loading section states
		section->onLoad({});
		section->loaded = true;
	}

	void Stage::unloadSection(StageSection* section)
	{
		if(!section->loaded)
		{
			return;
		}
		//TODO saving and loading section states
		section->onUnload();
		section->loaded = false;
	}

	const fgl::ArrayList<Character*>& Stage::getCharacters() const
	{
		return characters;
	}

	void Stage::removeAccessibleItem(Item* item)
	{
		for(auto& listPair : characterAccessibleItems)
		{
			size_t itemIndex = listPair.second.indexOf(item);
			if(itemIndex!=-1)
			{
				listPair.second.remove(itemIndex);
			}
		}
	}
}

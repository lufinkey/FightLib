
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
		for(auto object : objects)
		{
			auto velocity = object->getVelocity();

			if(object->respondsToGravity())
			{
				velocity += getGravity(object)*appData.getFrameSpeedMultiplier();
			}
			if(object->respondsToAirResistance())
			{
				velocity += getAirResistance(object)*appData.getFrameSpeedMultiplier();
			}

			object->setVelocity(velocity);
		}

		//update the stage controllers
		for(auto controller : controllers)
		{
			controller->update(appData);
		}

		//update all these damn managers
		hitboxCollisionManager.update(appData);
		drawManager.update(appData);
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
		
		//draw StageControllers
		for(auto& controller : controllers)
		{
			controller->draw(appData, graphics);
		}
	}

	fgl::Vector2d Stage::getGravity(StageObject* object) const
	{
		return fgl::Vector2d(0, 1800);
	}

	fgl::Vector2d Stage::getAirResistance(StageObject* object) const
	{
		auto velocity = object->getVelocity();
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

	fgl::CollisionManager* Stage::getCollisionManager()
	{
		return &collisionManager;
	}

	const fgl::CollisionManager* Stage::getCollisionManager() const
	{
		return &collisionManager;
	}

	HitboxCollisionManager* Stage::getHitboxCollisionManager()
	{
		return &hitboxCollisionManager;
	}

	const HitboxCollisionManager* Stage::getHitboxCollisionManager() const
	{
		return &hitboxCollisionManager;
	}

	DrawManager* Stage::getDrawManager()
	{
		return &drawManager;
	}

	const DrawManager* Stage::getDrawManager() const
	{
		return &drawManager;
	}

	fgl::ArrayList<Item*> Stage::getAccessibleItems(fl::Character* character) const
	{
		return characterAccessibleItems.get(character, {});
	}

	void Stage::addObject(StageObject* object, double zLayer)
	{
		if(object->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("object", "already added to a stage");
		}
		objects.add(object);
		if(object->getFlag("Platform"))
		{
			platforms.add(static_cast<Platform*>(object));
		}
		if(object->getFlag("Entity"))
		{
			entities.add(static_cast<Entity*>(object));
			hitboxCollisionManager.addEntity(static_cast<Entity*>(object));
		}
		if(object->getFlag("Item"))
		{
			items.add(static_cast<Item*>(object));
		}
		if(object->getFlag("Character"))
		{
			characters.add(static_cast<Character*>(object));
		}
		
		collisionManager.addCollidable(static_cast<fgl::Collidable*>(object));
		drawManager.addDrawable(object, zLayer);
		object->stage = this;
		object->onAddToStage(this);
		onAddObject(object);
	}

	void Stage::removeObject(StageObject* object)
	{
		if(object->stage!=this)
		{
			return;
		}
		objects.removeFirstEqual(object);
		platforms.removeFirstEqual(static_cast<Platform*>(object));
		entities.removeFirstEqual(static_cast<Entity*>(object));
		items.removeFirstEqual(static_cast<Item*>(object));
		characters.removeFirstEqual(static_cast<Character*>(object));
		
		collisionManager.removeCollidable(static_cast<fgl::Collidable*>(object));
		hitboxCollisionManager.removeEntity(static_cast<Entity*>(object));
		drawManager.removeDrawable(static_cast<fl::Drawable*>(object));
		object->stage = nullptr;
		object->onRemoveFromStage(this);
		onRemoveObject(object);
	}

	const fgl::ArrayList<StageObject*>& Stage::getObjects() const
	{
		return objects;
	}

	const fgl::ArrayList<Platform*>& Stage::getPlatforms() const
	{
		return platforms;
	}

	const fgl::ArrayList<Entity*>& Stage::getEntities() const
	{
		return entities;
	}

	const fgl::ArrayList<Item*>& Stage::getItems() const
	{
		return items;
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
	
	void Stage::onAddObject(StageObject* object)
	{
		//
	}
	
	void Stage::onRemoveObject(StageObject* object)
	{
		//
	}
}

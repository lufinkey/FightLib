
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	Stage::Stage() : fight(nullptr)
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
		
		//update gravity
		for(auto entity : entities)
		{
			if(entity->getParentEntity()==nullptr && entity->respondsToGravity())
			{
				auto gravity = getGravity(entity);
				auto velocity = entity->getVelocity();
				velocity += gravity*appData.getFrameSpeedMultiplier();
				entity->setVelocity(velocity);
			}
		}
		
		drawManager.update(appData);
		
		if(fight != nullptr)
		{
			//update entity hitboxes
			for(size_t i=0; i<entities.size(); i++)
			{
				auto entity1 = entities[i];
				auto hitboxes1 = entity1->getMetaPointBoxes(METAPOINT_HITBOX);
				auto collisionRects1 = entity1->getCollisionRects();
				hitboxes1 = hitboxes1.filter([](const TaggedBox& box){
					if(box.tag==-1)
					{
						return false;
					}
					return true;
				});
				
				for(size_t j=(i+1); j<entities.size(); j++)
				{
					auto entity2 = entities[j];
					auto hitboxes2 = entity2->getMetaPointBoxes(METAPOINT_HITBOX);
					auto collisionRects2 = entity2->getCollisionRects();
					
					hitboxes2 = hitboxes2.filter([](const TaggedBox& box){
						if(box.tag==-1)
						{
							return false;
						}
						return true;
					});
					
					//check for hitboxes hitting each other
					typedef std::pair<size_t, size_t> TagPair;
					fgl::ArrayList<TagPair> collidedHitboxes;
					for(auto& hitbox1 : hitboxes1)
					{
						for(auto& hitbox2 : hitboxes2)
						{
							if(hitbox1.rect.intersects(hitbox2.rect))
							{
								collidedHitboxes.add(TagPair(hitbox1.tag, hitbox2.tag));
							}
						}
					}
					
					//TODO have separate virtual functions for when hitbox hits hitbox and when hitbox hits hurtbox
					if(collidedHitboxes.size() > 0)
					{
						//TODO figure out which hitbox has a higher priority and do hurt thing
					}
					else
					{
						//See if any hitboxes hit any hurtboxes
					}
				}
			}
		}
		
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
	
	Fight* Stage::getFight() const
	{
		return fight;
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
	
	void Stage::addEntity(Entity* entity, double zLayer)
	{
		if(entity->stage!=nullptr)
		{
			throw fgl::IllegalArgumentException("entity", "cannot be added to multiple Stage objects");
		}
		entity->stage = this;
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

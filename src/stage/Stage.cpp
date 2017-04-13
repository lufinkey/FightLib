
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
		
		//update entity hitboxes
		for(size_t i=0; i<entities.size(); i++)
		{
			auto entity1 = entities[i];
			auto hitboxes1 = entity1->getMetaPointBoxes(METAPOINT_HITBOX).filter([](const TaggedBox& box){
				//filter out untagged hitboxes
				if(box.tag==-1)
				{
					return false;
				}
				return true;
			});
			auto collisionRects1 = entity1->getCollisionRects();
			
			for(size_t j=(i+1); j<entities.size(); j++)
			{
				auto entity2 = entities[j];
				auto hitboxes2 = entity2->getMetaPointBoxes(METAPOINT_HITBOX).filter([](const TaggedBox& box){
					//filter out untagged hitboxes
					if(box.tag==-1)
					{
						return false;
					}
					return true;
				});
				auto collisionRects2 = entity2->getCollisionRects();
				
				//check for hitboxes hitting each other
				//find the highest priority hitting hitboxes
				size_t topIndex1 = -1;
				size_t topIndex2 = -1;
				float topPriorityDiff = 0;
				float lastTopPriority = 0;
				bool foundHitboxCollision = false;
				size_t hitboxIndex1 = 0;
				for(auto& hitbox1 : hitboxes1)
				{
					size_t hitboxIndex2 = 0;
					for(auto& hitbox2 : hitboxes2)
					{
						if(hitbox1.rect.intersects(hitbox2.rect))
						{
							fgl::Vector2d boxDiff = hitbox2.rect.getCenter() - hitbox1.rect.getCenter();
							float angle1 = fgl::Math::radtodeg(fgl::Math::atan2(-boxDiff.y, boxDiff.x));
							float angle2 = fgl::Math::normalizeDegrees(angle1+180);

							auto info1 = entity1->getHitboxInfo(hitbox1.tag);
							auto info2 = entity2->getHitboxInfo(hitbox2.tag);

							//make sure the angle they're hitting at is within their ranges
							if(info1.getEffectiveAngleRange().contains(angle1) && info2.getEffectiveAngleRange().contains(angle2))
							{
								float priorityDiff = fgl::Math::abs(info1.getPriority()-info2.getPriority());
								double topPriority = fgl::Math::max(info1.getPriority(), info2.getPriority());
								if(!foundHitboxCollision || priorityDiff > topPriorityDiff || (priorityDiff==topPriorityDiff && topPriority > lastTopPriority))
								{
									topIndex1 = hitboxIndex1;
									topIndex2 = hitboxIndex2;
									topPriorityDiff = priorityDiff;
									lastTopPriority = topPriority;
									foundHitboxCollision = true;
								}
							}
						}
						hitboxIndex2++;
					}
					hitboxIndex1++;
				}
				
				if(foundHitboxCollision)
				{
					//handle two hitboxes colliding
					auto& hitbox1 = hitboxes1[topIndex1];
					auto& hitbox2 = hitboxes2[topIndex2];
					
					auto info1 = entity1->getHitboxInfo(hitbox1.tag);
					auto info2 = entity2->getHitboxInfo(hitbox2.tag);
					
					if(info1.getPriority() > info2.getPriority())
					{
						auto clashEvent = HitboxClashEvent(hitbox1, info1, entity2, hitbox2, info2);
						entity1->onHitboxClash(clashEvent);
					}
					else if(info2.getPriority() > info1.getPriority())
					{
						auto clashEvent = HitboxClashEvent(hitbox2, info2, entity1, hitbox1, info1);
						entity2->onHitboxClash(clashEvent);
					}
					else
					{
						auto clashEvent1 = HitboxClashEvent(hitbox1, info1, entity2, hitbox2, info2);
						auto clashEvent2 = HitboxClashEvent(hitbox2, info2, entity1, hitbox1, info1);
						//randomly choose which entity gets the event first
						double randomFirst = fgl::Math::random();
						if(randomFirst < 0.5)
						{
							entity1->onHitboxClash(clashEvent1);
							entity2->onHitboxClash(clashEvent2);
						}
						else
						{
							entity2->onHitboxClash(clashEvent2);
							entity1->onHitboxClash(clashEvent1);
						}
					}
				}
				else
				{
					//check if any hitboxes hit any bounds boxes
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

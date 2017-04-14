
#include <fightlib/entity/hitbox/HitboxCollisionManager.hpp>

namespace fl
{
	HitboxCollisionManager::HitboxCollisionManager()
	{
		//
	}
	
	void HitboxCollisionManager::addEntity(Entity* entity)
	{
		entities.add(entity);
	}
	
	void HitboxCollisionManager::removeEntity(Entity* entity)
	{
		size_t index = entities.indexOf(entity);
		if(index!=(size_t)-1)
		{
			entities.remove(index);
		}
	}
	
	void HitboxCollisionManager::update(const fgl::ApplicationData& appData)
	{
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
									//set the new top priority hitbox collision
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
					
					auto clashEvent1 = HitboxClashEvent(hitbox1, info1, entity2, hitbox2, info2);
					auto clashEvent2 = HitboxClashEvent(hitbox2, info2, entity1, hitbox1, info1);
					if(info1.getPriority() > info2.getPriority())
					{
						entity1->onHitboxClash(clashEvent1);
						entity2->onHitboxClash(clashEvent2);
					}
					else if(info2.getPriority() > info1.getPriority())
					{
						entity2->onHitboxClash(clashEvent2);
						entity1->onHitboxClash(clashEvent1);
					}
					else //if(info1.getPriority() == info2.getPriority())
					{
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
					//check if any hitboxes hit any collision rects
					//TODO should I use hurtboxes instead of the collision rects?
					
				}
			}
		}
	}
}

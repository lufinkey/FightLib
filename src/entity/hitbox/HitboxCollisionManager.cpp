
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
		fgl::ArrayList<HitboxClashPair> clashPairs;
		
		fgl::ArrayList<std::function<void()>> onHitboxClashCalls;
		fgl::ArrayList<std::function<void()>> onHitboxClashFinishCalls;
		
		//update entity hitboxes
		for(auto& entityPair : getEntityPairs())
		{
			auto entity1 = entityPair.entity1;
			auto& hitboxes1 = entityPair.hitboxes1;
			auto collisionRects1 = entity1->getCollisionRects();
			
			auto entity2 = entityPair.entity2;
			auto& hitboxes2 = entityPair.hitboxes2;
			auto collisionRects2 = entity2->getCollisionRects();
			
			//check for hitboxes hitting each other
			fgl::ArrayList<HitboxPair> hitboxPairs;
			if(entity1->respondsToHitboxClash(entity2) && entity2->respondsToHitboxClash(entity1))
			{
				for(auto& hitbox1 : hitboxes1)
				{
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
								hitboxPairs.add(HitboxPair(hitbox1, info1, hitbox2, info2));
							}
						}
					}
				}
			}
			
			//find previous clashed hitbox pairs
			fgl::ArrayList<HitboxPair> prevHitboxPairs1;
			fgl::ArrayList<HitboxPair> prevHitboxPairs2;
			for(auto& prevClashPair : previousClashPairs)
			{
				if(prevClashPair.getFirstEntity()==entity1 && prevClashPair.getSecondEntity()==entity2)
				{
					prevHitboxPairs1 = prevClashPair.getHitboxPairs();
					prevHitboxPairs2 = prevClashPair.getFlippedHitboxPairs();
					break;
				}
				else if(prevClashPair.getFirstEntity()==entity2 && prevClashPair.getSecondEntity()==entity1)
				{
					prevHitboxPairs1 = prevClashPair.getFlippedHitboxPairs();
					prevHitboxPairs2 = prevClashPair.getHitboxPairs();
					break;
				}
			}
			
			if(hitboxPairs.size() > 0)
			{
				//handle hitboxes clashing
				auto clashPair = HitboxClashPair(entity1, entity2, hitboxPairs);
				auto& priorityPair = clashPair.getPriorityHitboxPair();
				
				//send hitbox clash events
				auto clashEvent1 = HitboxClashEvent(entity2, clashPair.getHitboxPairs(), prevHitboxPairs1);
				auto clashEvent2 = HitboxClashEvent(entity1, clashPair.getFlippedHitboxPairs(), prevHitboxPairs2);
				if(priorityPair.hitboxInfo1.getPriority() > priorityPair.hitboxInfo2.getPriority())
				{
					//entity1 gets the event first, since it has the higher priority
					if(prevHitboxPairs1.size() > 0)
					{
						onHitboxClashCalls.add([=]{
							entity1->onHitboxClashUpdate(clashEvent1);
							entity2->onHitboxClashUpdate(clashEvent2);
						});
					}
					else
					{
						onHitboxClashCalls.add([=]{
							entity1->onHitboxClash(clashEvent1);
							entity2->onHitboxClash(clashEvent2);
						});
					}
				}
				else if(priorityPair.hitboxInfo2.getPriority() > priorityPair.hitboxInfo1.getPriority())
				{
					//entity2 gets the event first, since it has the higher priority
					if(prevHitboxPairs1.size() > 0)
					{
						onHitboxClashCalls.add([=]{
							entity2->onHitboxClashUpdate(clashEvent2);
							entity1->onHitboxClashUpdate(clashEvent1);
						});
					}
					else
					{
						onHitboxClashCalls.add([=]{
							entity2->onHitboxClash(clashEvent2);
							entity1->onHitboxClash(clashEvent1);
						});
					}
				}
				else //if(priorityPair.hitboxInfo1.getPriority() == priorityPair.hitboxInfo2.getPriority())
				{
					//randomly choose which entity gets the event first
					double randomFirst = fgl::Math::random();
					if(randomFirst < 0.5)
					{
						//entity1 gets the event first
						if(prevHitboxPairs1.size() > 0)
						{
							onHitboxClashCalls.add([=]{
								entity1->onHitboxClashUpdate(clashEvent1);
								entity2->onHitboxClashUpdate(clashEvent2);
							});
						}
						else
						{
							onHitboxClashCalls.add([=]{
								entity1->onHitboxClash(clashEvent1);
								entity2->onHitboxClash(clashEvent2);
							});
						}
					}
					else
					{
						//entity2 gets the event first
						if(prevHitboxPairs1.size() > 0)
						{
							onHitboxClashCalls.add([=]{
								entity2->onHitboxClashUpdate(clashEvent2);
								entity1->onHitboxClashUpdate(clashEvent1);
							});
						}
						else
						{
							onHitboxClashCalls.add([=]{
								entity2->onHitboxClash(clashEvent2);
								entity1->onHitboxClash(clashEvent1);
							});
						}
					}
				}
				
				clashPairs.add(clashPair);
			}
			else
			{
				//add finish events if hitboxes were clashing in the last frame, since they aren't clashing anymore
				if(prevHitboxPairs1.size() > 0)
				{
					//fuck the order who fucking cares I'll just call them in the order that they are
					//It's a finish event anyway so I don't give a shit about the order
					auto clashEvent1 = HitboxClashEvent(entity2, {}, prevHitboxPairs1);
					auto clashEvent2 = HitboxClashEvent(entity1, {}, prevHitboxPairs2);
					
					onHitboxClashFinishCalls.add([=]{
						entity1->onHitboxClashFinish(clashEvent1);
						entity2->onHitboxClashFinish(clashEvent2);
					});
				}
				
				//check if any hitboxes hit any collision rects
				//TODO should I use hurtboxes instead of the collision rects?
			}
		}
		
		previousClashPairs = clashPairs;
		
		//call onHitboxClashFinish events
		for(auto& onHitboxClashFinish : onHitboxClashFinishCalls)
		{
			onHitboxClashFinish();
		}
		
		//call onHitboxClash events
		for(auto& onHitboxClash : onHitboxClashCalls)
		{
			onHitboxClash();
		}
		
		//tell entities that hitbox updates have finished
		for(auto entity : entities)
		{
			entity->onFinishHitboxUpdates();
		}
	}
	
	fgl::ArrayList<HitboxCollisionManager::EntityPair> HitboxCollisionManager::getEntityPairs() const
	{
		size_t pair_count = 0;
		size_t entities_size=entities.size();
		for(size_t i=0; i<entities_size; i++)
		{
			for(size_t j=(i+1); j<entities_size; j++)
			{
				pair_count++;
			}
		}
		fgl::ArrayList<EntityPair> pairs;
		pairs.reserve(pair_count);
		for(size_t i=0; i<entities.size(); i++)
		{
			auto entity1 = entities[i];
			fgl::ArrayList<TaggedBox> hitboxes1;
			if(entity1->usesHitboxes())
			{
				hitboxes1 = entity1->getMetaPointBoxes(METAPOINT_HITBOX).filter([](const TaggedBox& box){
					//filter out untagged hitboxes
					if(box.tag==-1)
					{
						return false;
					}
					return true;
				});
			}
			
			for(size_t j=(i+1); j<entities.size(); j++)
			{
				auto entity2 = entities[j];
				fgl::ArrayList<TaggedBox> hitboxes2;
				if(entity2->usesHitboxes())
				{
					hitboxes2 = entity2->getMetaPointBoxes(METAPOINT_HITBOX).filter([](const TaggedBox& box){
						//filter out untagged hitboxes
						if(box.tag==-1)
						{
							return false;
						}
						return true;
					});
				}
				
				EntityPair pair;
				pair.entity1 = entity1;
				pair.hitboxes1 = hitboxes1;
				pair.entity2 = entity2;
				pair.hitboxes2 = hitboxes2;
				
				pairs.add(pair);
			}
		}
		return pairs;
	}
}

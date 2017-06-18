
#include <fightlib/entity/hitbox/HitboxCollisionManager.hpp>
#include <fightlib/entity/hitbox/HitboxCollisionPair.hpp>

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

	void HitboxCollisionManager::addHitboxCollisionEvents(Entity* hittingEntity,
		Entity* hitEntity,
		const fgl::ArrayList<HitboxCollision>& hitboxCollisions,
		const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions,
		fgl::ArrayList<std::function<void()>>* onHitboxCollisionCalls,
		fgl::ArrayList<std::function<void()>>* onHitboxCollisionFinishCalls)
	{
		auto collisionEvent = HitboxCollisionEvent(hitEntity, hitboxCollisions, previousHitboxCollisions);
		if(hitboxCollisions.size() > 0)
		{
			if(previousHitboxCollisions.size() > 0)
			{
				onHitboxCollisionCalls->add([=]{
					hittingEntity->onHitboxCollisionUpdate(collisionEvent);
				});
			}
			else
			{
				onHitboxCollisionCalls->add([=] {
					hittingEntity->onHitboxCollision(collisionEvent);
				});
			}
		}
		else if(previousHitboxCollisions.size() > 0)
		{
			onHitboxCollisionFinishCalls->add([=] {
				hittingEntity->onHitboxCollisionFinish(collisionEvent);
			});
		}
	}
	
	void HitboxCollisionManager::update(const fgl::ApplicationData& appData)
	{
		fgl::ArrayList<HitboxClashPair> clashPairs;
		fgl::ArrayList<HitboxCollisionPair> collisionPairs;
		
		fgl::ArrayList<std::function<void()>> onHitboxClashCalls;
		fgl::ArrayList<std::function<void()>> onHitboxClashFinishCalls;
		fgl::ArrayList<std::function<void()>> onHitboxCollisionCalls;
		fgl::ArrayList<std::function<void()>> onHitboxCollisionFinishCalls;
		
		//update entity hitboxes
		for(auto& entityPair : getEntityPairs())
		{
			auto entity1 = entityPair.entity1;
			auto& hitboxes1 = entityPair.hitboxes1;
			auto polygons1 = entity1->getTransformedCollisionPolygons();
			
			auto entity2 = entityPair.entity2;
			auto& hitboxes2 = entityPair.hitboxes2;
			auto polygons2 = entity2->getTransformedCollisionPolygons();
			
			//check for hitboxes hitting each other
			fgl::ArrayList<HitboxClash> hitboxClashes;
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
								hitboxClashes.add(HitboxClash(hitbox1, info1, hitbox2, info2));
							}
						}
					}
				}
			}
			
			//find previous clashed hitboxes
			fgl::ArrayList<HitboxClash> prevHitboxClashes1;
			fgl::ArrayList<HitboxClash> prevHitboxClashes2;
			for(auto& prevClashPair : previousClashPairs)
			{
				if(prevClashPair.getFirstEntity()==entity1 && prevClashPair.getSecondEntity()==entity2)
				{
					prevHitboxClashes1 = prevClashPair.getHitboxClashes();
					prevHitboxClashes2 = prevClashPair.getFlippedHitboxClashes();
					break;
				}
				else if(prevClashPair.getFirstEntity()==entity2 && prevClashPair.getSecondEntity()==entity1)
				{
					prevHitboxClashes1 = prevClashPair.getFlippedHitboxClashes();
					prevHitboxClashes2 = prevClashPair.getHitboxClashes();
					break;
				}
			}
			
			if(hitboxClashes.size() > 0)
			{
				//handle hitboxes clashing
				auto clashPair = HitboxClashPair(entity1, entity2, hitboxClashes);
				auto& priorityClash = clashPair.getPriorityHitboxClash();
				
				//send hitbox clash events
				auto clashEvent1 = HitboxClashEvent(entity2, clashPair.getHitboxClashes(), prevHitboxClashes1);
				auto clashEvent2 = HitboxClashEvent(entity1, clashPair.getFlippedHitboxClashes(), prevHitboxClashes2);
				if(priorityClash.hitboxInfo1.getPriority() > priorityClash.hitboxInfo2.getPriority())
				{
					//entity1 gets the event first, since it has the higher priority
					if(prevHitboxClashes1.size() > 0)
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
				else if(priorityClash.hitboxInfo2.getPriority() > priorityClash.hitboxInfo1.getPriority())
				{
					//entity2 gets the event first, since it has the higher priority
					if(prevHitboxClashes2.size() > 0)
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
						if(prevHitboxClashes1.size() > 0)
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
						if(prevHitboxClashes2.size() > 0)
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
				if(prevHitboxClashes1.size() > 0)
				{
					//fuck the order who fucking cares I'll just call them in the order that they are
					//It's a finish event anyway so I don't give a shit about the order
					auto clashEvent1 = HitboxClashEvent(entity2, {}, prevHitboxClashes1);
					auto clashEvent2 = HitboxClashEvent(entity1, {}, prevHitboxClashes2);
					
					onHitboxClashFinishCalls.add([=]{
						entity1->onHitboxClashFinish(clashEvent1);
						entity2->onHitboxClashFinish(clashEvent2);
					});
				}
				
				//check if any hitboxes hit any collision rects
				//TODO should I use hurtboxes instead of the collision rects?

				//find collisions of entity1's hitboxes with entity2
				fgl::ArrayList<HitboxCollision> hitboxCollisions1;
				double priority1 = 0;
				if(entity2->canCollideWithEntityHitbox(entity1))
				{
					for(auto& hitbox1 : hitboxes1)
					{
						for(size_t i=0; i<polygons2.size(); i++)
						{
							auto& polygon = polygons2[i];
							if(polygon.intersects(hitbox1.rect.toPolygon()))
							{
								auto info = entity1->getHitboxInfo(hitbox1.tag);
								priority1 += info.getPriority();
								hitboxCollisions1.add(HitboxCollision(hitbox1, info, i));
							}
						}
					}
				}
				//find collisions of entity2's hitboxes with entity1
				fgl::ArrayList<HitboxCollision> hitboxCollisions2;
				double priority2 = 0;
				if(entity1->canCollideWithEntityHitbox(entity2))
				{
					for(auto& hitbox2 : hitboxes2)
					{
						for(size_t i=0; i<polygons1.size(); i++)
						{
							auto& polygon = polygons1[i];
							if(polygon.intersects(hitbox2.rect.toPolygon()))
							{
								auto info = entity2->getHitboxInfo(hitbox2.tag);
								priority2 += info.getPriority();
								hitboxCollisions2.add(HitboxCollision(hitbox2, info, i));
							}
						}
					}
				}

				//find previous collisions of entity1's hitboxes with entity2
				fgl::ArrayList<HitboxCollision> prevHitboxCollisions1;
				for(auto& prevCollisionPair : previousCollisionPairs)
				{
					if(prevCollisionPair.getHittingEntity()==entity1 && prevCollisionPair.getHitEntity()==entity2)
					{
						prevHitboxCollisions1 = prevCollisionPair.getHitboxCollisions();
						break;
					}
				}
				//find previous collisions of entity2's hitboxes with entity1
				fgl::ArrayList<HitboxCollision> prevHitboxCollisions2;
				for(auto& prevCollisionPair : previousCollisionPairs)
				{
					if(prevCollisionPair.getHittingEntity()==entity2 && prevCollisionPair.getHitEntity()==entity1)
					{
						prevHitboxCollisions2 = prevCollisionPair.getHitboxCollisions();
						break;
					}
				}

				//add priorities of previous collisions to entity1's priority
				for(auto& prevHitboxCollision : prevHitboxCollisions1)
				{
					priority1 += prevHitboxCollision.hitboxInfo.getPriority();
				}
				//add priorities of previous collisions to entity2's priority
				for(auto& prevHitboxCollision : prevHitboxCollisions2)
				{
					priority2 += prevHitboxCollision.hitboxInfo.getPriority();
				}

				if(priority1 > priority2)
				{
					addHitboxCollisionEvents(entity1, entity2, hitboxCollisions1, prevHitboxCollisions1, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
					addHitboxCollisionEvents(entity2, entity1, hitboxCollisions2, prevHitboxCollisions2, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
				}
				else if(priority2 > priority1)
				{
					addHitboxCollisionEvents(entity2, entity1, hitboxCollisions2, prevHitboxCollisions2, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
					addHitboxCollisionEvents(entity1, entity2, hitboxCollisions1, prevHitboxCollisions1, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
				}
				else
				{
					double randomFirst = fgl::Math::random();
					if(randomFirst < 0.5)
					{
						addHitboxCollisionEvents(entity1, entity2, hitboxCollisions1, prevHitboxCollisions1, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
						addHitboxCollisionEvents(entity2, entity1, hitboxCollisions2, prevHitboxCollisions2, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
					}
					else
					{
						addHitboxCollisionEvents(entity2, entity1, hitboxCollisions2, prevHitboxCollisions2, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
						addHitboxCollisionEvents(entity1, entity2, hitboxCollisions1, prevHitboxCollisions1, &onHitboxCollisionCalls, &onHitboxCollisionFinishCalls);
					}
				}

				if(hitboxCollisions1.size() > 0)
				{
					collisionPairs.add(HitboxCollisionPair(entity1, entity2, hitboxCollisions1));
				}
				if(hitboxCollisions2.size() > 0)
				{
					collisionPairs.add(HitboxCollisionPair(entity2, entity1, hitboxCollisions2));
				}
			}
		}
		
		previousClashPairs = clashPairs;
		previousCollisionPairs = collisionPairs;
		
		//call onHitboxClashFinish events
		for(auto& onHitboxClashFinish : onHitboxClashFinishCalls)
		{
			onHitboxClashFinish();
		}

		//call onHitboxCollisionFinish events
		for(auto& onHitboxCollisionFinish : onHitboxCollisionFinishCalls)
		{
			onHitboxCollisionFinish();
		}
		
		//call onHitboxClash events
		for(auto& onHitboxClash : onHitboxClashCalls)
		{
			onHitboxClash();
		}

		//call onHitboxCollision events
		for(auto& onHitboxCollision : onHitboxCollisionCalls)
		{
			onHitboxCollision();
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

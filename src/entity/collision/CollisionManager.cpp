
#include <fightlib/entity/collision/CollisionManager.hpp>

namespace fl
{
	CollisionManager::CollisionManager()
	{
		//
	}

	void CollisionManager::addCollidable(Collidable* entity)
	{
		collidables.add(entity);
	}

	void CollisionManager::removeCollidable(Collidable* entity)
	{
		size_t index = collidables.indexOf(entity);
		if(index!=(size_t)-1)
		{
			collidables.remove(index);
		}
	}

	CollisionSide CollisionManager::getCollisionSide(const fgl::Vector2d& shiftAmount) const
	{
		if(shiftAmount.x < 0)
		{
			return COLLISIONSIDE_LEFT;
		}
		else if(shiftAmount.x > 0)
		{
			return COLLISIONSIDE_RIGHT;
		}
		else if(shiftAmount.y < 0)
		{
			return COLLISIONSIDE_TOP;
		}
		else if(shiftAmount.y > 0)
		{
			return COLLISIONSIDE_BOTTOM;
		}
		throw fgl::IllegalArgumentException("shiftAmount", "cannot be 0,0");
	}
	
	CollisionSide CollisionManager::getOppositeCollisionSide(CollisionSide side)
	{
		switch(side)
		{
			case COLLISIONSIDE_LEFT:
				return COLLISIONSIDE_RIGHT;
				
			case COLLISIONSIDE_RIGHT:
				return COLLISIONSIDE_LEFT;
				
			case COLLISIONSIDE_TOP:
				return COLLISIONSIDE_BOTTOM;
				
			case COLLISIONSIDE_BOTTOM:
				return COLLISIONSIDE_TOP;
		}
		throw fgl::IllegalArgumentException("side", "invalid CollisionSide enum value");
	}

	void CollisionManager::update(const fgl::ApplicationData& appData)
	{
		fgl::ArrayList<CollisionPair> pairs = getCollisionPairs();
		previousCollisions.clear();
		
		fgl::ArrayList<std::function<void()>> onCollisionCalls;
		fgl::ArrayList<std::function<void()>> onCollisionFinishCalls;

		//handle collisions
		#ifdef DOUBLECHECK_COLLISIONS
		for(size_t i=0; i<2; i++)
		#endif
		//checking all of the collisions twice fixed collision jerking
		{
			for(auto& pair : pairs)
			{
				Collidable* collidable1 = pair.collidable1;
				Collidable* collidable2 = pair.collidable2;
				CollisionPair newPair(collidable1, collidable2);
				#ifdef DOUBLECHECK_COLLISIONS
					size_t pairReplaceIndex = -1;
					if(i==1)
					{
						pairReplaceIndex = previousCollisions.indexOf(newPair);
						if(pairReplaceIndex!=-1)
						{
							newPair = previousCollisions[pairReplaceIndex];
						}
					}
				#endif
			
				if((collidable1->isStaticCollisionBody() && !collidable2->isStaticCollisionBody())
					|| (!collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody()))
					//TODO allow two non-static collision bodies to collide
				{
					fgl::ArrayList<CollisionRect*> rects1 = collidable1->getCollisionRects();
					fgl::ArrayList<CollisionRect*> rects2 = collidable2->getCollisionRects();
				
					fgl::ArrayList<CollisionRectPair> rectPairs = pair.getCollisionRectPairs(rects1, rects2);
					//check each CollisionRect for a collision
					for(auto& rectPair : rectPairs)
					{
						fgl::Vector2d shiftAmount = CollisionRect::getCollisionOffset(rectPair.first, rectPair.second);
						if(!(shiftAmount.x==0 && shiftAmount.y==0))
						{
							CollisionSide collisionSide1 = getCollisionSide(shiftAmount);
							CollisionRectTagPair rectTagPair = CollisionRectTagPair(rectPair.first->getTag(), rectPair.second->getTag());
							//make sure that this collision wasn't previously ignored
							if(pair.shouldIgnoreCollision(rectPair.first, rectPair.second))
							{
								//since the collision was previously ignored, ignore it again
								if(!newPair.ignoredCollisions.contains(rectTagPair))
								{
									newPair.ignoredCollisions.add(rectTagPair);
								}
							}
							else
							{
								//check if we should ignore this collision
								bool ignore = false;
								if(!collidable1->respondsToCollision(collidable2, collisionSide1))
								{
									ignore = true;
								}
								else if(!collidable2->respondsToCollision(collidable1, getOppositeCollisionSide(collisionSide1)))
								{
									ignore = true;
								}

								if(!ignore)
								{
									//decide how to shift the collidables
									if(collidable1->isStaticCollisionBody())
									{
										collidable2->shift(shiftAmount);
										for(auto& rect : rects2)
										{
											rect->shift(shiftAmount);
										}
									}
									else if(collidable2->isStaticCollisionBody())
									{
										collidable1->shift(-shiftAmount);
										for(auto& rect : rects1)
										{
											rect->shift(-shiftAmount);
										}
									}
									else
									{
										//TODO make a case here for two non-static bodies colliding
									}

									//add collision side to previous collision sides if not already added
									if(!newPair.previousCollisionSides.contains(collisionSide1))
									{
										newPair.previousCollisionSides.add(collisionSide1);
									}
								}

								//add the rect pair to the priority rects, so that it will be checked first on the next frame
								if(!newPair.priorityRects.contains(rectTagPair))
								{
									newPair.priorityRects.add(rectTagPair);
								}
							}
						}
					}

					//add new collision pair to previous collisions
					#ifdef DOUBLECHECK_COLLISIONS
					if(pairReplaceIndex==-1)
					{
					#endif
						if(newPair.priorityRects.size() > 0)
						{
							previousCollisions.add(newPair);
						}
					#ifdef DOUBLECHECK_COLLISIONS
					}
					else
					{
						previousCollisions[pairReplaceIndex] = newPair;
					}
					#endif
				}
			
				#ifdef DOUBLECHECK_COLLISIONS
				if(i==1)
				#endif
				{
					//check for new/updated collision calls
					for(auto collisionSide : newPair.previousCollisionSides)
					{
						auto collisionEvent1 = CollisionEvent(collidable2, collisionSide);
						auto collisionEvent2 = CollisionEvent(collidable1, getOppositeCollisionSide(collisionSide));
						if(!pair.previousCollisionSides.contains(collisionSide))
						{
							//the previous collision pair doesn't have this collision side, so it is a new collision
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable2->onCollision(collisionEvent2);
									collidable1->onCollision(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable1->onCollision(collisionEvent1);
									collidable2->onCollision(collisionEvent2);
								});
							}
							else
							{
								//TODO make a case here for two non-static bodies colliding
							}
						}
						else
						{
							//the previous collision pair has this collision side, so it's an updated collision
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable2->onCollisionUpdate(collisionEvent2);
									collidable1->onCollisionUpdate(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable1->onCollisionUpdate(collisionEvent1);
									collidable2->onCollisionUpdate(collisionEvent2);
								});
							}
							else
							{
								//TODO make a case here for two non-static bodies colliding
							}
						}
					}

					//check for finished collision calls
					for(auto prevCollisionSide : pair.previousCollisionSides)
					{
						if(!newPair.previousCollisionSides.contains(prevCollisionSide))
						{
							auto collisionEvent1 = CollisionEvent(collidable2, prevCollisionSide);
							auto collisionEvent2 = CollisionEvent(collidable1, getOppositeCollisionSide(prevCollisionSide));
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionFinishCalls.add([=]{
									collidable2->onCollisionFinish(collisionEvent2);
									collidable1->onCollisionFinish(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionFinishCalls.add([=]{
									collidable1->onCollisionFinish(collisionEvent1);
									collidable2->onCollisionFinish(collisionEvent2);
								});
							}
							else
							{
								//TODO make a case here for two non-static bodies colliding
							}
						}
					}
				}
			}
		}
		
		//call finished collisions
		for(auto& onCollisionFinish : onCollisionFinishCalls)
		{
			onCollisionFinish();
		}
		
		//call current collisions
		for(auto& onCollision : onCollisionCalls)
		{
			onCollision();
		}
		
		//set the previous positions of the collidables
		for(auto collidable : collidables)
		{
			collidable->previousPosition = collidable->getPosition();
		}
		
		//tell updated collidables that their collision updates have finished
		for(auto collidable : collidables)
		{
			collidable->onFinishCollisionUpdates();
		}
	}

	fgl::ArrayList<CollisionPair> CollisionManager::getCollisionPairs() const
	{
		size_t pair_count = 0;
		size_t entities_size=collidables.size();
		for(size_t i=0; i<entities_size; i++)
		{
			for(size_t j=(i+1); j<entities_size; j++)
			{
				pair_count++;
			}
		}
		fgl::ArrayList<CollisionPair> pairs;
		pairs.reserve(pair_count);
		pairs.addAll(previousCollisions);
		for(size_t i=0; i<entities_size; i++)
		{
			for(size_t j=(i+1); j<entities_size; j++)
			{
				CollisionPair pair(collidables[i], collidables[j]);
				if(previousCollisions.indexOf(pair)==(size_t)-1)
				{
					pairs.add(pair);
				}
			}
		}
		return pairs;
	}
}


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
		//TODO what do I do about deleted collidables after they're removed? Queue deletion?
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
		for(auto& pair : pairs)
		{
			Collidable* collidable1 = pair.collidable1;
			Collidable* collidable2 = pair.collidable2;
			CollisionPair newPair(collidable1, collidable2);
			
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
					fgl::Vector2d shiftAmount = CollisionRect::checkCollision(rectPair.first, rectPair.second);
					if(!(shiftAmount.x==0 && shiftAmount.y==0))
					{
						CollisionSide collisionSide1 = getCollisionSide(shiftAmount);
						CollisionRectTagPair rectTagPair = CollisionRectTagPair(rectPair.first->getTag(), rectPair.second->getTag());
						//make sure that this collision wasn't previously ignored
						if(pair.shouldIgnoreCollision(rectPair.first, rectPair.second))
						{
							//since the collision was previously ignored, ignore it again
							newPair.ignoredCollisions.add(rectTagPair);
						}
						else
						{
							//check if we should ignore this collision
							bool ignore = false;
							if(collidable1->respondsToCollision(collidable2, collisionSide1))
							{
								ignore = true;
							}
							else if(collidable2->respondsToCollision(collidable1, getOppositeCollisionSide(collisionSide1)))
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
							newPair.priorityRects.add(rectTagPair);
						}
					}
				}
				
				//check for new/updated collision calls
				for(auto collisionSide : newPair.previousCollisionSides)
				{
					if(!pair.previousCollisionSides.contains(collisionSide))
					{
						//the previous collision pair doesn't have this collision side, so it is a new collision
						if(collidable1->isStaticCollisionBody())
						{
							onCollisionCalls.add([=]{
								collidable2->onCollision(collidable1, getOppositeCollisionSide(collisionSide));
								collidable1->onCollision(collidable2, collisionSide);
							});
						}
						else if(collidable2->isStaticCollisionBody())
						{
							onCollisionCalls.add([=]{
								collidable1->onCollision(collidable2, collisionSide);
								collidable2->onCollision(collidable1, getOppositeCollisionSide(collisionSide));
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
							onCollisionCalls.add([=]{
								collidable2->onCollisionUpdate(collidable1, getOppositeCollisionSide(collisionSide));
								collidable1->onCollisionUpdate(collidable2, collisionSide);
							});
						}
						else if(collidable2->isStaticCollisionBody())
						{
							onCollisionCalls.add([=]{
								collidable1->onCollisionUpdate(collidable2, collisionSide);
								collidable2->onCollisionUpdate(collidable1, getOppositeCollisionSide(collisionSide));
							});
						}
						else
						{
							//TODO make a case here for two non-static bodies colliding
						}
					}
				}

				//add new collision pair to previous collisions
				if(newPair.priorityRects.size() > 0)
				{
					previousCollisions.add(newPair);
				}
			}
			
			//check for finished collision calls
			for(auto prevCollisionSide : pair.previousCollisionSides)
			{
				if(!newPair.previousCollisionSides.contains(prevCollisionSide))
				{
					if(collidable1->isStaticCollisionBody())
					{
						onCollisionFinishCalls.add([=]{
							collidable2->onCollisionFinish(collidable1, getOppositeCollisionSide(prevCollisionSide));
							collidable1->onCollisionFinish(collidable2, prevCollisionSide);
						});
					}
					else if(collidable2->isStaticCollisionBody())
					{
						onCollisionFinishCalls.add([=]{
							collidable1->onCollisionFinish(collidable2, prevCollisionSide);
							collidable2->onCollisionFinish(collidable1, getOppositeCollisionSide(prevCollisionSide));
						});
					}
					else
					{
						//TODO make a case here for two non-static bodies colliding
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
		
		//tell updated collidables that their collision updates have finished
		for(auto collidable : collidables) //updatedCollidables)
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

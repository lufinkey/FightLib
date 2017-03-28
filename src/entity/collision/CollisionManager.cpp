
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
		for(size_t i=(previousCollisions.size()-1); i!=(size_t)-1; i--)
		{
			const CollisionPair& pair = previousCollisions[i];
			if(pair.collidable1==entity || pair.collidable2==entity)
			{
				previousCollisions.remove(i);
			}
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
		fgl::ArrayList<Collidable*> updatedCollidables;

		//handle collisions
		for(auto& pair : pairs)
		{
			Collidable* collidable1 = pair.collidable1;
			Collidable* collidable2 = pair.collidable2;
			if((collidable1->isStaticCollisionBody() && !collidable2->isStaticCollisionBody())
				|| (!collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody()))
			{
				fgl::ArrayList<CollisionRect*> rects1 = collidable1->getCollisionRects();
				fgl::ArrayList<CollisionRect*> rects2 = collidable2->getCollisionRects();

				CollisionPair newPair(collidable1, collidable2);
				
				fgl::ArrayList<CollisionRectPair> rectPairs = pair.getCollisionRectPairs(rects1, rects2);
				for(auto& rectPair : rectPairs)
				{
					fgl::Vector2d shiftAmount = CollisionRect::checkCollision(rectPair.first, rectPair.second);
					if(!(shiftAmount.x==0 && shiftAmount.y==0))
					{
						if(collidable1->isStaticCollisionBody())
						{
							collidable2->shift(shiftAmount);
							for(auto& rect : rects2)
							{
								rect->shift(shiftAmount);
							}
							
							CollisionSide collisionSide = getCollisionSide(-shiftAmount);
							if(!newPair.previousCollisionSides.contains(collisionSide))
							{
								newPair.previousCollisionSides.add(collisionSide);
							}

							CollisionPair::PriorityRect priorityRect = { .rectTag1=rectPair.first->getTag(), .rectTag2=rectPair.second->getTag() };
							newPair.priorityRects.add(priorityRect);
						}
						else if(collidable2->isStaticCollisionBody())
						{
							shiftAmount = -shiftAmount;
							collidable1->shift(shiftAmount);
							for(auto& rect : rects1)
							{
								rect->shift(shiftAmount);
							}
							
							CollisionSide collisionSide = getCollisionSide(shiftAmount);
							if(!newPair.previousCollisionSides.contains(collisionSide))
							{
								newPair.previousCollisionSides.add(collisionSide);
							}

							CollisionPair::PriorityRect priorityRect = { .rectTag1=rectPair.first->getTag(), .rectTag2=rectPair.second->getTag() };
							newPair.priorityRects.add(priorityRect);
						}
						else
						{
							//TODO make a case here for two non-static bodies colliding
						}
						
						//add collidables to the list of updated collidables
						if(!updatedCollidables.contains(collidable1))
						{
							updatedCollidables.add(collidable1);
						}
						if(!updatedCollidables.contains(collidable2))
						{
							updatedCollidables.add(collidable2);
						}
					}
				}
				
				//add to finished collision calls
				onCollisionFinishCalls.add([=]{
					for(auto prevCollisionSide : pair.previousCollisionSides)
					{
						if(!newPair.previousCollisionSides.contains(prevCollisionSide))
						{
							if(collidable1->isStaticCollisionBody())
							{
								collidable2->onCollisionFinish(collidable1, getOppositeCollisionSide(prevCollisionSide));
								collidable1->onCollisionFinish(collidable2, prevCollisionSide);
							}
							else if(collidable2->isStaticCollisionBody())
							{
								collidable1->onCollisionFinish(collidable2, prevCollisionSide);
								collidable2->onCollisionFinish(collidable1, getOppositeCollisionSide(prevCollisionSide));
							}
							else
							{
								//TODO make a case here for two non-static bodies colliding
							}
						}
					}
				});
				
				//add to collision calls
				onCollisionCalls.add([=]{
					for(auto collisionSide : newPair.previousCollisionSides)
					{
						if(collidable1->isStaticCollisionBody())
						{
							collidable2->onCollision(collidable1, getOppositeCollisionSide(collisionSide));
							collidable1->onCollision(collidable2, collisionSide);
						}
						else if(collidable2->isStaticCollisionBody())
						{
							collidable1->onCollision(collidable2, collisionSide);
							collidable2->onCollision(collidable1, getOppositeCollisionSide(collisionSide));
						}
						else
						{
							//TODO make a case here for two non-static bodies colliding
						}
					}
				});

				if(newPair.priorityRects.size() > 0)
				{
					previousCollisions.add(newPair);
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
		for(auto collidable : updatedCollidables)
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

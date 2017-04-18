
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

#define DOUBLECHECK_COLLISIONS

	void CollisionManager::update(const fgl::ApplicationData& appData)
	{
		fgl::ArrayList<CollisionPair> pairs = getCollisionPairs();
		fgl::ArrayList<CollisionPair> collisions;
		
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
						pairReplaceIndex = collisions.indexOf(newPair);
						if(pairReplaceIndex!=-1)
						{
							newPair = collisions[pairReplaceIndex];
						}
					}
				#endif
			
				if(!(collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody()))
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
							CollisionSide collisionSide2 = getOppositeCollisionSide(collisionSide1);
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
								else if(!collidable2->respondsToCollision(collidable1, collisionSide2))
								{
									ignore = true;
								}

								if(ignore)
								{
									//ignore collision
									if(!newPair.ignoredCollisions.contains(rectTagPair))
									{
										newPair.ignoredCollisions.add(rectTagPair);
									}
								}
								else
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
										//find out if rect1 has collided with a static collision body
										fgl::ArrayList<CollisionPair> collisionsSoFar;
										collisionsSoFar.reserve(previousCollisions.size()+collisions.size());
										collisionsSoFar.addAll(previousCollisions);
										collisionsSoFar.addAll(collisions);
										bool staticOpposite1 = false;
										for(auto& collision : collisionsSoFar)
										{
											if(collision.collidable1==collidable1 && collision.collidable2->isStaticCollisionBody() && collision.sides.contains(collisionSide2))
											{
												staticOpposite1 = true;
												break;
											}
											else if(collision.collidable2==collidable1 && collision.collidable1->isStaticCollisionBody() && collision.sides.contains(collisionSide1))
											{
												staticOpposite1 = true;
												break;
											}
										}
										bool staticOpposite2 = false;
										for(auto& collision : collisionsSoFar)
										{
											if(collision.collidable1==collidable2 && collision.collidable2->isStaticCollisionBody() && collision.sides.contains(collisionSide1))
											{
												staticOpposite2 = true;
												break;
											}
											else if(collision.collidable2==collidable2 && collision.collidable1->isStaticCollisionBody() && collision.sides.contains(collisionSide2))
											{
												staticOpposite2 = true;
												break;
											}
										}

										double portion1 = 0;
										if((staticOpposite1 && staticOpposite2) || (!staticOpposite1 && !staticOpposite2))
										{
											//TODO make a BETTER case here for two non-static bodies colliding
											double mass1 = collidable1->getMass();
											double mass2 = collidable2->getMass();
											auto rect1 = rectPair.first->getRect();
											auto prevRect1 = rectPair.first->getPreviousRect();
											auto rect2 = rectPair.second->getRect();
											auto prevRect2 = rectPair.second->getPreviousRect();

											double velocity1 = 0;
											double velocity2 = 0;
											switch(collisionSide1)
											{
												case COLLISIONSIDE_LEFT:
												velocity1 = rect1.x - prevRect1.x;
												velocity2 = (rect2.x+rect2.width) - (prevRect2.x+prevRect2.width);
												break;

												case COLLISIONSIDE_RIGHT:
												velocity1 = (rect1.x+rect1.width) - (prevRect1.x+prevRect1.width);
												velocity2 = rect2.x - prevRect2.x;
												break;

												case COLLISIONSIDE_TOP:
												velocity1 = rect1.y - prevRect1.y;
												velocity2 = (rect2.y+rect2.height) - (prevRect2.y+prevRect2.height);
												break;

												case COLLISIONSIDE_BOTTOM:
												velocity1 = (rect1.y+rect1.height) - (prevRect1.y+prevRect1.height);
												velocity2 = rect2.y - prevRect2.y;
												break;
											}

											double force1 = velocity2*mass2;
											double force2 = velocity1*mass1;
											double negativeForce = 0;
											double positiveForce = 0;
											if(force1 < 0)
											{
												negativeForce += force1;
											}
											else
											{
												positiveForce += force1;
											}
											if(force2 < 0)
											{
												negativeForce += force2;
											}
											else
											{
												positiveForce += force2;
											}
											double netForce = force1 + force2;

											fgl::Vector2d moveAmount1;
											fgl::Vector2d moveAmount2;
											if(netForce < 0)
											{
												double portion = 0;
												if(negativeForce!=0)
												{
													portion = netForce/negativeForce;
												}
												switch(collisionSide1)
												{
													case COLLISIONSIDE_LEFT:
													case COLLISIONSIDE_TOP:
													portion1 = 1.0-portion;
													break;

													case COLLISIONSIDE_RIGHT:
													case COLLISIONSIDE_BOTTOM:
													portion1 = portion;
													break;
												}
											}
											else if(netForce!=0)
											{
												double portion = 0;
												if(positiveForce!=0)
												{
													portion = netForce/positiveForce;
												}
												switch(collisionSide1)
												{
													case COLLISIONSIDE_LEFT:
													case COLLISIONSIDE_TOP:
													portion1 = portion;
													break;

													case COLLISIONSIDE_RIGHT:
													case COLLISIONSIDE_BOTTOM:
													portion1 = 1.0-portion;
													break;
												}
											}
										}
										else if(staticOpposite1)
										{
											portion1 = 0;
										}
										else if(staticOpposite2)
										{
											portion1 = 1.0;
										}

										auto moveAmount1 = -shiftAmount*portion1;
										auto moveAmount2 = shiftAmount+moveAmount1;

										if(moveAmount1.x!=0 || moveAmount1.y!=0)
										{
											collidable1->shift(moveAmount1);
											for(auto& rect : rects1)
											{
												rect->shift(moveAmount1);
											}
										}
										if(moveAmount2.x!=0 || moveAmount2.y!=0)
										{
											collidable2->shift(moveAmount2);
											for(auto& rect : rects2)
											{
												rect->shift(moveAmount2);
											}
										}
									}

									//add collision side to previous collision sides if not already added
									if(!newPair.sides.contains(collisionSide1))
									{
										newPair.sides.add(collisionSide1);
									}
								}
							}
							
							//add the rect pair to the priority rects, so that it will be checked first on the next frame
							if(!newPair.priorityRects.contains(rectTagPair))
							{
								newPair.priorityRects.add(rectTagPair);
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
							collisions.add(newPair);
						}
					#ifdef DOUBLECHECK_COLLISIONS
					}
					else
					{
						collisions[pairReplaceIndex] = newPair;
					}
					#endif
				}
			
				#ifdef DOUBLECHECK_COLLISIONS
				if(i==1)
				#endif
				{
					//check for new/updated collision calls
					for(auto collisionSide : newPair.sides)
					{
						if(!pair.sides.contains(collisionSide))
						{
							//the previous collision pair doesn't have this collision side, so it is a new collision
							auto collisionEvent1 = CollisionEvent(collidable2, collisionSide, COLLISIONSTATE_NEW);
							auto collisionEvent2 = CollisionEvent(collidable1, getOppositeCollisionSide(collisionSide), COLLISIONSTATE_NEW);
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
								double randomFirst = fgl::Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollision(collisionEvent1);
										collidable2->onCollision(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollision(collisionEvent2);
										collidable1->onCollision(collisionEvent1);
									});
								}
							}
						}
						else
						{
							//the previous collision pair has this collision side, so it's an updated collision
							auto collisionEvent1 = CollisionEvent(collidable2, collisionSide, COLLISIONSTATE_UPDATED);
							auto collisionEvent2 = CollisionEvent(collidable1, getOppositeCollisionSide(collisionSide), COLLISIONSTATE_UPDATED);
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
								double randomFirst = fgl::Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollisionUpdate(collisionEvent1);
										collidable2->onCollisionUpdate(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollisionUpdate(collisionEvent2);
										collidable1->onCollisionUpdate(collisionEvent1);
									});
								}
							}
						}
					}

					//check for finished collision calls
					for(auto prevCollisionSide : pair.sides)
					{
						if(!newPair.sides.contains(prevCollisionSide))
						{
							auto collisionEvent1 = CollisionEvent(collidable2, prevCollisionSide, COLLISIONSTATE_FINISHED);
							auto collisionEvent2 = CollisionEvent(collidable1, getOppositeCollisionSide(prevCollisionSide), COLLISIONSTATE_FINISHED);
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
								double randomFirst = fgl::Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollisionFinish(collisionEvent1);
										collidable2->onCollisionFinish(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollisionFinish(collisionEvent2);
										collidable1->onCollisionFinish(collisionEvent1);
									});
								}
							}
						}
					}
				}
			}
		}
		
		previousCollisions = collisions;
		
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
		fgl::ArrayList<CollisionPair> pairs;
		fgl::ArrayList<CollisionPair> prevStaticCollisions;
		fgl::ArrayList<CollisionPair> prevNonstaticCollisions;
		for(auto& collisionPair : previousCollisions)
		{
			if(collisionPair.collidable1->isStaticCollisionBody() || collisionPair.collidable2->isStaticCollisionBody())
			{
				prevStaticCollisions.add(collisionPair);
			}
			else
			{
				prevNonstaticCollisions.add(collisionPair);
			}
		}
		fgl::ArrayList<CollisionPair> staticCollisions;
		fgl::ArrayList<CollisionPair> nonstaticCollisions;
		size_t collidables_size = collidables.size();
		for(size_t i=0; i<collidables_size; i++)
		{
			for(size_t j=(i+1); j<collidables_size; j++)
			{
				CollisionPair pair(collidables[i], collidables[j]);
				if(previousCollisions.indexOf(pair)==(size_t)-1)
				{
					if(pair.collidable1->isStaticCollisionBody() || pair.collidable2->isStaticCollisionBody())
					{
						staticCollisions.add(pair);
					}
					else
					{
						nonstaticCollisions.add(pair);
					}
				}
			}
		}
		pairs.reserve(prevStaticCollisions.size()+staticCollisions.size()+prevNonstaticCollisions.size()+nonstaticCollisions.size());
		pairs.addAll(prevStaticCollisions);
		pairs.addAll(staticCollisions);
		pairs.addAll(prevNonstaticCollisions);
		pairs.addAll(nonstaticCollisions);
		return pairs;
	}
}

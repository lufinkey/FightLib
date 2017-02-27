
#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/Entity.hpp>

namespace fl
{
	CollisionManager::CollisionManager()
	{
		//
	}

	void CollisionManager::addEntity(Entity* entity)
	{
		entities.add(entity);
	}

	void CollisionManager::removeEntity(Entity* entity)
	{
		size_t index = entities.indexOf(entity);
		if(index!=(size_t)-1)
		{
			entities.remove(index);
		}
		for(size_t i=(previousCollisions.size()-1); i!=(size_t)-1; i--)
		{
			const CollisionPair& pair = previousCollisions[i];
			if(pair.entity1==entity || pair.entity2==entity)
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

	void CollisionManager::update(fgl::ApplicationData appData)
	{
		fgl::ArrayList<CollisionPair> pairs = getCollisionPairs();
		previousCollisions.clear();
		fgl::BasicDictionary<Entity*, fgl::ArrayList<CollisionRect*>> collisionRects;
		collisionRects.reserve(entities.size());

		//handle collisions
		for(auto& pair : pairs)
		{
			Entity* entity1 = pair.entity1;
			Entity* entity2 = pair.entity2;
			if((entity1->isStaticCollisionBody() && !entity2->isStaticCollisionBody())
				|| (!entity1->isStaticCollisionBody() && entity2->isStaticCollisionBody()))
			{
				size_t rectsIndex1 = collisionRects.indexOfKey(entity1);
				if(rectsIndex1==(size_t)-1)
				{
					rectsIndex1 = collisionRects.size();
					collisionRects.set(entity1, entity1->createCollisionRects());
				}
				size_t rectsIndex2 = collisionRects.indexOfKey(entity2);
				if(rectsIndex2==(size_t)-1)
				{
					rectsIndex2 = collisionRects.size();
					collisionRects.set(entity2, entity2->createCollisionRects());
				}

				fgl::ArrayList<CollisionRect*>& rects1 = collisionRects.valueAt(rectsIndex1);
				fgl::ArrayList<CollisionRect*>& rects2 = collisionRects.valueAt(rectsIndex2);

				fgl::ArrayList<CollisionRectPair> rectPairs = pair.getCollisionRectPairs(rects1, rects2);
				for(auto& rectPair : rectPairs)
				{
					fgl::Vector2d shiftAmount = CollisionRect::checkCollision(rectPair.first, rectPair.second);
					if(!(shiftAmount.x==0 && shiftAmount.y==0))
					{
						if(entity1->isStaticCollisionBody())
						{
							entity2->shift(shiftAmount);
							for(auto& rect : rects2)
							{
								rect->shift(shiftAmount);
							}
							entity2->onCollision(entity1, getCollisionSide(shiftAmount));
							entity1->onCollision(entity2, getCollisionSide(-shiftAmount));
						}
						else if(entity2->isStaticCollisionBody())
						{
							shiftAmount = -shiftAmount;
							entity1->shift(shiftAmount);
							for(auto& rect : rects1)
							{
								rect->shift(shiftAmount);
							}
							entity1->onCollision(entity2, getCollisionSide(shiftAmount));
							entity2->onCollision(entity1, getCollisionSide(-shiftAmount));
						}
						else
						{
							//TODO make a case here for two non-static bodies colliding
						}
					}
				}
			}
		}

		//delete the collision rects
		auto& contents = collisionRects.getContents();
		for(auto& pair : contents)
		{
			auto& rectList = pair.second;
			for(auto rect : rectList)
			{
				delete rect;
			}
		}
	}

	fgl::ArrayList<CollisionPair> CollisionManager::getCollisionPairs() const
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
		fgl::ArrayList<CollisionPair> pairs;
		pairs.reserve(pair_count);
		pairs.addAll(previousCollisions);
		for(size_t i=0; i<entities_size; i++)
		{
			for(size_t j=(i+1); j<entities_size; j++)
			{
				CollisionPair pair(entities[i], entities[j]);
				if(previousCollisions.indexOf(pair)==(size_t)-1)
				{
					pairs.add(pair);
				}
			}
		}
		return pairs;
	}
}

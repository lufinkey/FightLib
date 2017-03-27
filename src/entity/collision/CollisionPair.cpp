
#include <fightlib/entity/collision/CollisionPair.hpp>

namespace fl
{
	CollisionPair::CollisionPair(Collidable* collidable1, Collidable* collidable2)
		: collidable1(collidable1), collidable2(collidable2)
	{
		//
	}

	bool CollisionPair::operator==(const CollisionPair& pair) const
	{
		if((collidable1==pair.collidable1 && collidable2==pair.collidable2) || (collidable1==pair.collidable2 && collidable2==pair.collidable1))
		{
			return true;
		}
		return false;
	}

	bool CollisionPair::operator!=(const CollisionPair& pair) const
	{
		return !operator==(pair);
	}

	fgl::ArrayList<CollisionRectPair> CollisionPair::getCollisionRectPairs(const fgl::ArrayList<CollisionRect*>& rects1, const fgl::ArrayList<CollisionRect*>& rects2) const
	{
		size_t pair_count = rects1.size()*rects2.size();
		fgl::ArrayList<CollisionRectPair> pairs;
		pairs.reserve(pair_count);
		for(auto& priorityRect : priorityRects)
		{
			size_t rectIndex1 = rects1.indexWhere([&priorityRect](CollisionRect* const & rect) -> bool {
				if(rect->getTag()==priorityRect.rectTag1)
				{
					return true;
				}
				return false;
			});
			size_t rectIndex2 = rects2.indexWhere([&priorityRect](CollisionRect* const & rect) -> bool {
				if(rect->getTag()==priorityRect.rectTag2)
				{
					return true;
				}
				return false;
			});
			if(rectIndex1!=-1 && rectIndex2!=-1)
			{
				pairs.add(CollisionRectPair(rects1[rectIndex1], rects2[rectIndex2]));
			}
		}
		fgl::ArrayList<CollisionRectPair> priorityPairs = pairs;
		for(auto& rect1 : rects1)
		{
			for(auto& rect2 : rects2)
			{
				CollisionRectPair pair(rect1, rect2);
				if(priorityPairs.indexOf(pair)==(size_t)-1)
				{
					pairs.add(pair);
				}
			}
		}
		return pairs;
	}
}

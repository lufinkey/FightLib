
#pragma once

#include "rects/CollisionRect.hpp"

namespace fl
{
	class Entity;

	typedef std::pair<CollisionRect*, CollisionRect*> CollisionRectPair;

	class CollisionPair
	{
	public:
		Entity* entity1;
		Entity* entity2;
		struct PriorityRect
		{
			fgl::String rectTag1;
			fgl::String rectTag2;
		};
		fgl::ArrayList<PriorityRect> priorityRects;

		CollisionPair(Entity* entity1, Entity* entity2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		fgl::ArrayList<CollisionRectPair> getCollisionRectPairs(const fgl::ArrayList<CollisionRect*>& rects1, const fgl::ArrayList<CollisionRect*>& rects2) const;
	};
}


#pragma once

#include "Collidable.hpp"
#include "rects/CollisionRect.hpp"

namespace fl
{
	class Collidable;

	typedef std::pair<CollisionRect*, CollisionRect*> CollisionRectPair;

	class CollisionPair
	{
	public:
		Collidable* collidable1;
		Collidable* collidable2;
		struct PriorityRect
		{
			fgl::String rectTag1;
			fgl::String rectTag2;
		};
		fgl::ArrayList<PriorityRect> priorityRects;
		fgl::ArrayList<CollisionSide> previousCollisionSides;

		CollisionPair(Collidable* collidable1, Collidable* collidable2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		fgl::ArrayList<CollisionRectPair> getCollisionRectPairs(const fgl::ArrayList<CollisionRect*>& rects1, const fgl::ArrayList<CollisionRect*>& rects2) const;
	};
}


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
		struct RectTagPair
		{
			fgl::String rectTag1;
			fgl::String rectTag2;
		};
		//! the rect pairs that had a collision in the previous frame
		fgl::ArrayList<RectTagPair> priorityRects;
		//! the sides of collidable1 that were collided on
		fgl::ArrayList<CollisionSide> previousCollisionSides;
		//! the rect pairs that should be ignored in the next frame if they have a collision
		fgl::ArrayList<RectTagPair> ignoredCollisions;

		CollisionPair(Collidable* collidable1, Collidable* collidable2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		bool shouldIgnoreCollision(CollisionRect* rect1, CollisionRect* rect2) const;

		fgl::ArrayList<CollisionRectPair> getCollisionRectPairs(const fgl::ArrayList<CollisionRect*>& rects1, const fgl::ArrayList<CollisionRect*>& rects2) const;
	};
}

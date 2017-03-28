
#include <fightlib/entity/collision/rects/CollisionRect.hpp>

namespace fl
{
	CollisionRect::CollisionRect(const fgl::String& tag)
		: tag(tag)
	{
		//
	}

	CollisionRect::~CollisionRect()
	{
		//
	}

	const fgl::String& CollisionRect::getTag() const
	{
		return tag;
	}

	fgl::Vector2d CollisionRect::getCenter() const
	{
		return getRect().getCenter();
	}

	fgl::Vector2d CollisionRect::checkCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		fgl::RectangleD rect1 = collisionRect1->getRect();
		fgl::RectangleD rect2 = collisionRect2->getRect();
		if(rect1.intersects(rect2))
		{
			bool filled1 = collisionRect1->isFilled();
			bool filled2 = collisionRect2->isFilled();
			if(filled1 && filled2)
			{
				return checkFilledCollision(collisionRect1, collisionRect2);
			}
			else if(filled1 || filled2)
			{
				CollisionRect* filledRect = nullptr;
				CollisionRect* pixelRect = nullptr;
				if(filled1)
				{
					filledRect = collisionRect1;
					pixelRect = collisionRect2;
				}
				else //if(filled2)
				{
					filledRect = collisionRect2;
					pixelRect = collisionRect1;
				}
				fgl::Vector2d offset = checkPixelOnFilledCollision(pixelRect, filledRect);
				if(filled1)
				{
					offset.x = -offset.x;
					offset.y = -offset.y;
				}
				return offset;
			}
			else
			{
				return checkPixelCollision(collisionRect1, collisionRect2);
			}
		}
		return fgl::Vector2d(0, 0);
	}

	typedef enum
	{
		DIR_NONE,
		DIR_UP,
		DIR_LEFT,
		DIR_DOWN,
		DIR_RIGHT
	} CollisionRectDirection;

	CollisionRectDirection CollisionRect_findRectCollisionDirection(const fgl::RectangleD& rect1, const fgl::Vector2d& velocity1, const fgl::RectangleD& rect2, const fgl::Vector2d& velocity2)
	{
		if(!rect1.intersects(rect2))
		{
			//no collision happening
			return DIR_NONE;
		}

		unsigned long multiplyNum = 1;
		do
		{
			fgl::Vector2d velocity = velocity1 - velocity2;
			velocity.x *= (double)multiplyNum;
			velocity.y *= (double)multiplyNum;
			if(velocity.x==0 && velocity.y==0)
			{
				fgl::RectangleD intersect1 = rect1.getIntersect(rect2);
				double intersect1_right = intersect1.x + intersect1.width;
				double intersect1_bottom = intersect1.y + intersect1.height;
				double rect1_right = rect1.x + rect1.width;
				double rect1_bottom = rect1.y + rect1.height;

				double difRight = rect1_right- intersect1_right;
				double difLeft = intersect1.x - rect1.x;
				double difBottom = rect1_bottom - intersect1_bottom;
				double difTop = intersect1.y - rect1.y;

				double difX = 0;
				double difY = 0;
				CollisionRectDirection dirX = DIR_NONE;
				CollisionRectDirection dirY = DIR_NONE;
				if(difLeft < difRight)
				{
					difX = difLeft;
					dirX = DIR_LEFT;
				}
				else if(difRight < difLeft)
				{
					difX = difRight;
					dirX = DIR_RIGHT;
				}
				
				if(difTop < difBottom)
				{
					difY = difTop;
					dirY = DIR_UP;
				}
				else if(difBottom < difTop)
				{
					difY = difBottom;
					dirY = DIR_DOWN;
				}

				if(dirX==DIR_NONE)
				{
					if(dirY==DIR_NONE)
					{
						//TODO have a fallback rather than just returning up
						return DIR_UP;
					}
					else
					{
						return dirY;
					}
				}
				else if(dirY==DIR_NONE)
				{
					return dirX;
				}
				else
				{
					if(difX < difY)
					{
						return dirX;
					}
					else if(difY < difX)
					{
						return dirY;
					}
					else
					{
						if(intersect1.width > intersect1.height)
						{
							return dirX;
						}
						else if(intersect1.height > intersect1.width)
						{
							return dirY;
						}
						return dirY;
					}
				}
			}

			double rect1_right = rect1.x + rect1.width;
			double rect1_bottom = rect1.y + rect1.height;

			fgl::RectangleD pastRect2 = rect2;
			pastRect2.x += velocity.x;
			pastRect2.y += velocity.y;
			double pastRect2_right = pastRect2.x+pastRect2.width;
			double pastRect2_bottom = pastRect2.y+pastRect2.height;

			//precision fix, before I figured out the real bug
			double pfix = 0; //0.0000001;

			if(pastRect2_bottom <= (rect1.y+pfix))
				//above frame1
			{
				if(pastRect2_right <= (rect1.x+pfix))
					//top left
				{
					fgl::LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocity.x, pastRect2.y-velocity.y);

					fgl::Vector2d box_TR(rect1_right, rect1.y);
					fgl::Vector2d box_TL(rect1.x, rect1.y);
					fgl::Vector2d box_BL(rect1.x, rect1_bottom);

					if(lineBR.segmentsIntersect(fgl::LineD(box_TL, box_TR)))
					{
						//push up
						return DIR_UP;
					}
					if(lineBR.segmentsIntersect(fgl::LineD(box_TL, box_BL)))
					{
						//push left
						return DIR_LEFT;
					}

					fgl::PolygonD polyRight;
					polyRight.addPoint(lineTR.point1);
					polyRight.addPoint(lineTR.point2);
					polyRight.addPoint(lineBR.point2);
					polyRight.addPoint(lineBR.point1);
					if(polyRight.contains(box_BL) || polyRight.contains(box_TL))
					{
						//push left
						return DIR_LEFT;
					}
					//push up
					return DIR_UP;
				}
				else if((pastRect2.x+pfix) >= rect1_right)
					//top right
				{
					fgl::LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocity.x, pastRect2.y-velocity.y);

					fgl::Vector2d box_TL(rect1.x, rect1.y);
					fgl::Vector2d box_TR(rect1_right, rect1.y);
					fgl::Vector2d box_BR(rect1_right, rect1_bottom);

					if(lineBL.segmentsIntersect(fgl::LineD(box_TL, box_TR)))
					{
						//push up
						return DIR_UP;
					}
					if(lineBL.segmentsIntersect(fgl::LineD(box_TR, box_BR)))
					{
						//push right
						return DIR_RIGHT;
					}

					fgl::PolygonD polyLeft;
					polyLeft.addPoint(lineTL.point1);
					polyLeft.addPoint(lineTL.point2);
					polyLeft.addPoint(lineBL.point2);
					polyLeft.addPoint(lineBL.point1);
					if(polyLeft.contains(box_BR) || polyLeft.contains(box_TR))
					{
						//push right
						return DIR_RIGHT;
					}
					//push up
					return DIR_UP;
				}
				else
					//top middle
				{
					//push up
					return DIR_UP;
				}
			}
			else if((pastRect2.y+pfix) >= rect1_bottom)
				//below frame1
			{
				if(pastRect2_right <= (rect1.x+pfix))
					//bottom left
				{
					fgl::LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocity.x, pastRect2.y-velocity.y);
					fgl::LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocity.x, pastRect2.y-velocity.y);

					fgl::Vector2d box_TL(rect1.x, rect1.y);
					fgl::Vector2d box_BL(rect1.x, rect1_bottom);
					fgl::Vector2d box_BR(rect1_right, rect1_bottom);

					if(lineTR.segmentsIntersect(fgl::LineD(box_BL, box_BR)))
					{
						//push down
						return DIR_DOWN;
					}
					if(lineTR.segmentsIntersect(fgl::LineD(box_TL, box_BL)))
					{
						//push left
						return DIR_LEFT;
					}

					fgl::PolygonD polyRight;
					polyRight.addPoint(lineTR.point1);
					polyRight.addPoint(lineTR.point2);
					polyRight.addPoint(lineBR.point2);
					polyRight.addPoint(lineBR.point1);
					if(polyRight.contains(box_TL) || polyRight.contains(box_BL))
					{
						//push left
						return DIR_LEFT;
					}
					//push down
					return DIR_DOWN;
				}
				else if((pastRect2.x+pfix) >= rect1_right)
					//bottom right
				{
					fgl::LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocity.x, pastRect2_bottom-velocity.y);
					fgl::LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocity.x, pastRect2.y-velocity.y);
					fgl::LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocity.x, pastRect2.y-velocity.y);

					fgl::Vector2d box_TR(rect1_right, rect1.y);
					fgl::Vector2d box_BR(rect1_right, rect1_bottom);
					fgl::Vector2d box_BL(rect1.x, rect1_bottom);

					if(lineTL.segmentsIntersect(fgl::LineD(box_BL, box_BR)))
					{
						//push down
						return DIR_DOWN;
					}
					if(lineTL.segmentsIntersect(fgl::LineD(box_TR, box_BR)))
					{
						//push right
						return DIR_RIGHT;
					}

					fgl::PolygonD polyLeft;
					polyLeft.addPoint(lineTL.point1);
					polyLeft.addPoint(lineTL.point2);
					polyLeft.addPoint(lineBL.point2);
					polyLeft.addPoint(lineBL.point1);
					if(polyLeft.contains(box_TR) || polyLeft.contains(box_BR))
					{
						//push right
						return DIR_RIGHT;
					}
					//push down
					return DIR_DOWN;
				}
				else
					//bottom middle
				{
					//push down
					return DIR_DOWN;
				}
			}
			else
				//within top and bottom of frame 1
			{
				if(pastRect2_right <= (rect1.x+pfix))
					//middle left
				{
					//push left
					return DIR_LEFT;
				}
				else if((pastRect2.x+pfix) >= rect1_right)
					//middle right
				{
					//push right
					return DIR_RIGHT;
				}
				else
				{
					//The rest is inside of the box, try a bigger velocity. Loop again.
				}
			}
			multiplyNum++;
		}
		while(true);
		return DIR_NONE;
	}

	fgl::Vector2d CollisionRect_getRectCollisionOffset(const fgl::RectangleD& rect1, const fgl::Vector2d& velocity1, const fgl::RectangleD& rect2, const fgl::Vector2d& velocity2)
	{
		CollisionRectDirection dir = CollisionRect_findRectCollisionDirection(rect1, velocity1, rect2, velocity2);
		switch(dir)
		{
			case DIR_NONE:
			return fgl::Vector2d(0, 0);

			case DIR_UP:
			return fgl::Vector2d(0, rect1.y-(rect2.y+rect2.height));

			case DIR_DOWN:
			return fgl::Vector2d(0, (rect1.y+rect1.height)-rect2.y);

			case DIR_LEFT:
			return fgl::Vector2d(rect1.x-(rect2.x+rect2.width), 0);

			case DIR_RIGHT:
			return fgl::Vector2d((rect1.x+rect1.width)-rect2.x, 0);
		}
		return fgl::Vector2d(0, 0);
	}

	fgl::Vector2d CollisionRect::checkFilledCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		return CollisionRect_getRectCollisionOffset(collisionRect1->getRect(), collisionRect1->getVelocity(), collisionRect2->getRect(), collisionRect2->getVelocity());
	}

	fgl::Vector2d CollisionRect::checkPixelOnFilledCollision(CollisionRect* pixelRect, CollisionRect* filledRect)
	{
		fgl::RectangleD rect1 = pixelRect->getRect();
		fgl::RectangleD rect2 = filledRect->getRect();
		fgl::RectangleD intersect = rect1.getIntersect(rect2);
		if(intersect.width==0 || intersect.height==0)
		{
			return fgl::Vector2d(0, 0);
		}
		fgl::Vector2d increment1 = pixelRect->getPreferredIncrement();
		fgl::Vector2d increment2 = filledRect->getPreferredIncrement();
		fgl::Vector2d increment = fgl::Vector2d(fgl::Math::min(increment1.x, increment2.x)*0.6, fgl::Math::min(increment1.x, increment2.x)*0.6);
		fgl::PixelIterator pixelIter1 = pixelRect->createPixelIterator(intersect, increment);
		bool foundOverlap = false;
		fgl::RectD overlapRect;
		while(pixelIter1.nextPixelIndex())
		{
			if(pixelRect->check(pixelIter1))
			{
				fgl::Vector2d currentPoint = pixelIter1.getCurrentPoint();
				if(!foundOverlap)
				{
					foundOverlap = true;
					overlapRect.left = currentPoint.x;
					overlapRect.top = currentPoint.y;
					overlapRect.right = currentPoint.x + fgl::Math::min(1.0, increment.x/2);
					overlapRect.bottom = currentPoint.y + fgl::Math::min(1.0, increment.y/2);
				}
				else
				{
					if(currentPoint.x < overlapRect.left)
					{
						overlapRect.left = currentPoint.x;
					}
					if(currentPoint.y < overlapRect.top)
					{
						overlapRect.top = currentPoint.y;
					}
					if(currentPoint.x > overlapRect.right)
					{
						overlapRect.right = currentPoint.x;
					}
					if(currentPoint.y > overlapRect.bottom)
					{
						overlapRect.bottom = currentPoint.y;
					}
				}
			}
		}
		if(!foundOverlap)
		{
			return fgl::Vector2d(0, 0);
		}
		overlapRect.left = rect1.x + (fgl::Math::floor((overlapRect.left-rect1.x)/increment1.x)*increment1.x);
		overlapRect.top = rect1.y + (fgl::Math::floor((overlapRect.top-rect1.y)/increment1.y)*increment1.y);
		overlapRect.right = rect1.x + (fgl::Math::ceil((overlapRect.right-rect1.x)/increment1.x)*increment1.x);
		overlapRect.bottom = rect1.y + (fgl::Math::ceil((overlapRect.bottom-rect1.y)/increment1.y)*increment1.y);
		return CollisionRect_getRectCollisionOffset(overlapRect.toRectangle(), pixelRect->getVelocity(), rect2, filledRect->getVelocity());
	}

	fgl::Vector2d CollisionRect::checkPixelCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		//TODO implement pixel on pixel collisions
		return fgl::Vector2d(0, 0);
	}
}

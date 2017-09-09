
#include <fightlib/entity/collision/CollisionRectManager.hpp>
#include <fightlib/entity/collision/rects/BoxCollisionRect.hpp>
#include <fightlib/entity/collision/rects/PixelCollisionRect.hpp>

namespace fl
{
	CollisionRectManager::CollisionRectManager()
		: collisionMethod(COLLISIONMETHOD_NONE)
	{
		//
	}
	
	CollisionRectManager::~CollisionRectManager()
	{
		for(auto collisionRect : collisionRects)
		{
			delete collisionRect;
		}
	}
	
	void CollisionRectManager::setCollisionMethod(CollisionMethod method)
	{
		collisionMethod = method;
	}
	
	CollisionMethod CollisionRectManager::getCollisionMethod() const
	{
		return collisionMethod;
	}
	
	const fgl::ArrayList<CollisionRect*>& CollisionRectManager::getCollisionRects() const
	{
		return collisionRects;
	}
	
	void CollisionRectManager::update(fl::Collidable* collidable)
	{
		auto previousRects = collisionRects;
		collisionRects = createCollisionRects(collidable);
		for(auto collisionRect : previousRects)
		{
			delete collisionRect;
		}
	}
	
	fgl::ArrayList<CollisionRect*> CollisionRectManager::createCollisionRects(fl::Collidable* collidable) const
	{
		switch(collisionMethod)
		{
			case COLLISIONMETHOD_NONE:
				return {};
				
			case COLLISIONMETHOD_FRAME:
			{
				auto size = collidable->getSize();
				auto origin = collidable->getOrigin();
				float rotation = 0;
				auto previousPosition = collidable->getPreviousPosition();
				auto position = collidable->getPosition(&rotation);
				auto positionDiff = position - previousPosition;
				auto scale = collidable->getDrawScale();
				auto rect = fgl::RectangleD(position.x-origin.x, position.y-origin.y, size.x, size.y);
				auto lastRect = rect;
				lastRect.x -= positionDiff.x;
				lastRect.y -= positionDiff.y;
				size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
					if(rect->getTag()=="all")
					{
						return true;
					}
					return false;
				});
				if(matchingRectIndex!=-1)
				{
					lastRect = collisionRects[matchingRectIndex]->getRect();
				}
				if(rotation!=0.0)
				{
					return {new BoxCollisionRect("all", rect, lastRect, rotation, origin, scale)};
				}
				return {new BoxCollisionRect("all", rect, lastRect, scale)};
			}
				
			case COLLISIONMETHOD_BOUNDS:
			{
				auto size = collidable->getSize();
				auto origin = collidable->getOrigin();
				float rotation = 0;
				auto previousPosition = collidable->getPreviousPosition();
				auto position = collidable->getPosition(&rotation);
				auto positionDiff = position - previousPosition;
				auto scale = collidable->getDrawScale();
				AnimationData* animData = collidable->getCurrentAnimationData();
				if(animData==nullptr)
				{
					return {};
				}
				size_t frameIndex = collidable->getCurrentAnimationFrameIndex();
				AnimationOrientation animationOrientation = collidable->getAnimationOrientation();
				fgl::ArrayList<TaggedBox> boundsList = animData->getBounds(frameIndex, animationOrientation);
				if(boundsList.size()==0)
				{
					return {};
				}
				fgl::ArrayList<CollisionRect*> newCollisionRects;
				newCollisionRects.reserve(boundsList.size());
				for(size_t i=0; i<boundsList.size(); i++)
				{
					auto& metaBounds = boundsList[i];
					auto rect = fgl::RectangleD((metaBounds.rect.x*scale.x), (metaBounds.rect.y*scale.y), (metaBounds.rect.width*scale.x), (metaBounds.rect.height*scale.y));
					auto boundOrigin = fgl::Vector2d(rect.x-origin.x, rect.y-origin.y);
					rect = fgl::RectangleD(position.x+boundOrigin.x, position.y+boundOrigin.y, rect.width, rect.height);
					fgl::String tag;
					if(metaBounds.tag!=-1)
					{
						tag = (fgl::String)"bounds:"+metaBounds.tag;
					}
					else
					{
						tag = (fgl::String)"bounds:index"+i;
					}
					auto lastRect = rect;
					lastRect.x -= positionDiff.x;
					lastRect.y -= positionDiff.y;
					size_t matchingRectIndex = collisionRects.indexWhere([&](CollisionRect* const & rect) -> bool {
						if(rect->getTag()==tag)
						{
							return true;
						}
						return false;
					});
					if(matchingRectIndex!=-1)
					{
						lastRect = collisionRects[matchingRectIndex]->getRect();
					}
					if(rotation!=0.0)
					{
						newCollisionRects.add(new BoxCollisionRect(tag, rect, lastRect, rotation, boundOrigin, scale));
					}
					newCollisionRects.add(new BoxCollisionRect(tag, rect, lastRect, scale));
				}
				return newCollisionRects;
			}
				
			case COLLISIONMETHOD_PIXEL:
			{
				auto size = collidable->getSize();
				auto origin = collidable->getOrigin();
				float rotation = 0;
				auto previousPosition = collidable->getPreviousPosition();
				auto position = collidable->getPosition(&rotation);
				auto positionDiff = position - previousPosition;
				AnimationData* animData = collidable->getCurrentAnimationData();
				if(animData==nullptr)
				{
					return {};
				}
				fgl::Animation* animation = animData->getAnimation();
				if(animation==nullptr)
				{
					return {};
				}
				size_t frameIndex = collidable->getCurrentAnimationFrameIndex();
				auto img = animation->getImage(frameIndex);
				auto srcRect = animation->getImageSourceRect(frameIndex);
				bool mirroredHorizontal = animData->isMirrored(collidable->getAnimationOrientation());
				auto rect = fgl::RectangleD(position.x-origin.x, position.y-origin.y, size.x, size.y);
				auto lastRect = rect;
				lastRect.x -= positionDiff.x;
				lastRect.y -= positionDiff.y;
				size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
					if(rect->getTag()=="all")
					{
						return true;
					}
					return false;
				});
				if(matchingRectIndex!=-1)
				{
					lastRect = collisionRects[matchingRectIndex]->getRect();
				}
				if(rotation!=0.0)
				{
					return {new PixelCollisionRect("all", rect, lastRect, srcRect, rotation, origin, img, mirroredHorizontal, false)};
				}
				return {new PixelCollisionRect("all", rect, lastRect, srcRect, img, mirroredHorizontal, false)};
			}
		}
		throw fgl::IllegalStateException("invalid collisionMethod enum value");
	}
}

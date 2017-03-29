
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
	
	void CollisionRectManager::update(const fgl::ApplicationData& appData, fl::Collidable* collidable, const fgl::Vector2d& velocity)
	{
		auto previousRects = collisionRects;
		collisionRects = createCollisionRects(appData, collidable, velocity);
		for(auto collisionRect : previousRects)
		{
			delete collisionRect;
		}
	}
	
	fgl::ArrayList<CollisionRect*> CollisionRectManager::createCollisionRects(const fgl::ApplicationData& appData, fl::Collidable* collidable, const fgl::Vector2d& velocity) const
	{
		switch(collisionMethod)
		{
			case COLLISIONMETHOD_NONE:
				return {};
				
			case COLLISIONMETHOD_FRAME:
			{
				fgl::Vector2d size = collidable->getSize();
				float rotation = 0;
				fgl::Vector2d position = collidable->getPosition(&rotation);
				double scale = (double)collidable->getDrawScale();
				fgl::RectangleD rect = fgl::RectangleD(position.x-(size.x/2), position.y-(size.y/2), size.x, size.y);
				fgl::Vector2d diff = velocity;
				size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
					if(rect->getTag()=="all")
					{
						return true;
					}
					return false;
				});
				if(matchingRectIndex!=-1)
				{
					diff += (rect.getCenter() - collisionRects[matchingRectIndex]->getCenter()) * 2.0;
				}
				if(rotation!=0.0)
				{
					return {new BoxCollisionRect("all", rect, diff, rotation, fgl::Vector2d(size.x/2, size.y/2), fgl::Vector2d((double)scale, (double)scale))};
				}
				return {new BoxCollisionRect("all", rect, diff, fgl::Vector2d((double)scale, (double)scale))};
			}
				
			case COLLISIONMETHOD_BOUNDS:
			{
				fgl::Vector2d size = collidable->getSize();
				float rotation = 0;
				fgl::Vector2d position = collidable->getPosition(&rotation);
				double scale = (double)collidable->getDrawScale();
				AnimationData* animData = collidable->getCurrentAnimationData();
				if(animData==nullptr)
				{
					return {};
				}
				size_t frameIndex = collidable->getCurrentAnimationFrameIndex();
				AnimationOrientation animationOrientation = collidable->getAnimationOrientation();
				fgl::ArrayList<AnimationData::MetaBounds> boundsList = animData->getBounds(frameIndex, animationOrientation);
				if(boundsList.size()==0)
				{
					return {};
				}
				fgl::ArrayList<CollisionRect*> collisionRects;
				collisionRects.reserve(boundsList.size());
				for(size_t i=0; i<boundsList.size(); i++)
				{
					auto& metaBounds = boundsList[i];
					fgl::RectangleD rect = fgl::RectangleD((metaBounds.rect.x*scale), (metaBounds.rect.y*scale), metaBounds.rect.width*scale, metaBounds.rect.height*scale);
					fgl::Vector2d origin = fgl::Vector2d(rect.x-(size.x/2), rect.y-(size.y/2));
					rect = fgl::RectangleD(position.x+origin.x, position.y+origin.y, rect.width, rect.height);
					fgl::String tag;
					if(metaBounds.tag!=-1)
					{
						tag = (fgl::String)"bounds:"+metaBounds.tag;
					}
					else
					{
						tag = (fgl::String)"bounds:index"+i;
					}
					fgl::Vector2d diff = velocity;
					size_t matchingRectIndex = collisionRects.indexWhere([&](CollisionRect* const & rect) -> bool {
						if(rect->getTag()==tag)
						{
							return true;
						}
						return false;
					});
					if(matchingRectIndex!=-1)
					{
						diff += (rect.getCenter() - collisionRects[matchingRectIndex]->getCenter()) * 2.0;
					}
					if(rotation!=0.0)
					{
						collisionRects.add(new BoxCollisionRect(tag, rect, diff, rotation, origin, fgl::Vector2d((double)scale, (double)scale)));
					}
					collisionRects.add(new BoxCollisionRect(tag, rect, diff, fgl::Vector2d((double)scale, (double)scale)));
				}
				return collisionRects;
			}
				
			case COLLISIONMETHOD_PIXEL:
			{
				fgl::Vector2d size = collidable->getSize();
				float rotation = 0;
				fgl::Vector2d position = collidable->getPosition(&rotation);
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
				fgl::TextureImage* img = animation->getImage(frameIndex);
				fgl::RectangleU srcRect = animation->getImageSourceRect(frameIndex);
				bool mirroredHorizontal = animData->isMirrored(collidable->getAnimationOrientation());
				auto rect = fgl::RectangleD(position.x-(size.x/2), position.y-(size.y/2), size.x, size.y);
				fgl::Vector2d diff = velocity;
				size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
					if(rect->getTag()=="all")
					{
						return true;
					}
					return false;
				});
				if(matchingRectIndex!=-1)
				{
					diff += (rect.getCenter() - collisionRects[matchingRectIndex]->getCenter()) * 2.0;
				}
				if(rotation!=0.0)
				{
					return {new PixelCollisionRect("all", rect, diff, srcRect, rotation, fgl::Vector2d(size.x/2, size.y/2), img, mirroredHorizontal, false)};
				}
				return {new PixelCollisionRect("all", rect, diff, srcRect, img, mirroredHorizontal, false)};
			}
		}
		throw fgl::IllegalStateException("invalid collisionMethod enum value");
	}
}

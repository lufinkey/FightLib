
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/collision/rects/BoxCollisionRect.hpp>
#include <fightlib/entity/collision/rects/PixelCollisionRect.hpp>

namespace fl
{
	#define setOptionalArg(arg, value) if(arg!=nullptr){ *arg = value; }

	Entity::Entity(const fgl::Vector2d& position, Entity::Orientation orientation)
		: offset(position),
		velocity(0, 0),
		scale(1.0f),
		orientation(orientation),
		collisionMethod(COLLISIONMETHOD_NONE),
		staticCollisionBody(false),
		animationChanged(false),
		currentAnimationName(),
		currentAnimationFrame(0),
		currentAnimationLastFrameTime(0),
		currentAnimationEventHandler(nullptr),
		parentEntity(nullptr)
	{
		//
	}

	Entity::~Entity()
	{
		for(size_t animations_size=animations.size(), i=0; i<animations_size; i++)
		{
			delete animations[i];
		}
	}

	void Entity::update(fgl::ApplicationData appData)
	{
		long long currentTimeMillis = appData.getTime().getMilliseconds();

		//handle animation
		do
		{
			if(animationChanged)
			{
				currentAnimationLastFrameTime = currentTimeMillis;
				animationChanged = false;
			}
			AnimationData* animData = getAnimationData(currentAnimationName);
			if(animData!=nullptr)
			{
				fgl::Animation* animation = animData->getAnimation();
				long double fps = (long double)animation->getFPS();
				long long frameTime = (long long)(1000.0L/fps);
				if(frameTime==0)
				{
					frameTime = 1;
				}
				long long timeDiff = currentTimeMillis - currentAnimationLastFrameTime;
				if(timeDiff>=frameTime)
				{
					currentAnimationFrame++;
					currentAnimationLastFrameTime = currentTimeMillis;
					if(currentAnimationFrame>=animation->getTotalFrames())
					{
						currentAnimationFrame = 0;
						if(currentAnimationEventHandler)
						{
							currentAnimationEventHandler(ANIMATIONEVENT_FINISHED);
							if(!animationChanged)
							{
								currentAnimationEventHandler(ANIMATIONEVENT_FRAMECHANGED);
							}
						}
					}
					else
					{
						if(currentAnimationEventHandler)
						{
							currentAnimationEventHandler(ANIMATIONEVENT_FRAMECHANGED);
						}
					}
				}
			}
		} while(animationChanged);

		//offset for velocity
		offset.x += (velocity.x*appData.getFrameSpeedMultiplier());
		offset.y += (velocity.y*appData.getFrameSpeedMultiplier());
	}

	void Entity::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		if(parentEntity==nullptr)
		{
			graphics.translate(offset.x, offset.y);
		}
		else
		{
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
			{
				graphics.translate(-offset.x, offset.y);
			}
			else
			{
				graphics.translate(offset.x, offset.y);
			}
		}

		struct AnchorDrawData
		{
			Entity* entity;
			fgl::Vector2d offset;
			float rotation;
			fgl::Vector2d rotationPoint;
			bool behind;
			bool visible;
		};
		fgl::ArrayList<AnchorDrawData> frontAnchorDatas;
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			AnchorDrawData anchorData;
			anchorData.entity = anchoredEntities[i].entity;
			bool drawEntity = anchorData.entity->getAnchorData(&anchorData.offset, &anchorData.rotation, &anchorData.rotationPoint, &anchorData.behind, &anchorData.visible);
			if(drawEntity && anchorData.visible)
			{
				if(anchorData.behind)
				{
					fgl::Graphics entityGraphics = graphics;
					entityGraphics.translate(anchorData.offset);
					if(anchorData.rotation!=0)
					{
						entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
					}
					anchorData.entity->draw(appData, entityGraphics);
				}
				else
				{
					frontAnchorDatas.add(anchorData);
				}
			}
		}

		fgl::Graphics selfGraphics = graphics;
		selfGraphics.scale(scale, scale);
		AnimationData* animData = getAnimationData(currentAnimationName);
		if(animData!=nullptr)
		{
			animData->drawFrame(currentAnimationFrame, selfGraphics, getAnimationOrientation(), true);
		}

		for(size_t i=0; i<frontAnchorDatas.size(); i++)
		{
			const AnchorDrawData& anchorData = frontAnchorDatas[i];
			fgl::Graphics entityGraphics = graphics;
			entityGraphics.translate(anchorData.offset);
			if(anchorData.rotation!=0)
			{
				entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
			}
			anchorData.entity->draw(appData, entityGraphics);
		}
	}

	fgl::Vector2d Entity::getSize() const
	{
		AnimationData* animData = getAnimationData(currentAnimationName);
		if(animData==nullptr)
		{
			return fgl::Vector2d(0, 0);
		}
		return animData->getSize(currentAnimationFrame, scale);
	}

	fgl::Vector2d Entity::getPosition(float* rotation) const
	{
		fgl::Vector2d childOffset = fgl::Vector2d(offset.x, offset.y);
		if(parentEntity!=nullptr)
		{
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
			{
				childOffset.x = -childOffset.x;
			}
		}

		fgl::Vector2d parentOffset;
		float anchorRotation;
		fgl::Vector2d anchorRotationPoint;
		getAnchorData(&parentOffset, &anchorRotation, &anchorRotationPoint, nullptr, nullptr);
		fgl::Vector2d fullOffset = parentOffset+childOffset;
		if(anchorRotation!=0)
		{
			fgl::TransformD rotationTransform;
			rotationTransform.rotate(anchorRotation, anchorRotationPoint);
			fullOffset = rotationTransform.transform(fullOffset);
		}
		if(parentEntity!=nullptr)
		{
			float parentRotation = 0;
			fgl::Vector2d parentPosition = parentEntity->getPosition(&parentRotation);
			setOptionalArg(rotation, parentRotation+anchorRotation)
			return parentPosition+fullOffset;
		}
		setOptionalArg(rotation, anchorRotation)
		return fullOffset;
	}

	float Entity::getScale() const
	{
		return scale;
	}

	void Entity::setScale(float scale_arg)
	{
		scale = scale_arg;
	}

	Entity::Orientation Entity::getOrientation() const
	{
		return orientation;
	}
	
	void Entity::setOrientation(Entity::Orientation orientation_arg)
	{
		orientation = orientation_arg;
	}

	Entity::CollisionMethod Entity::getCollisionMethod() const
	{
		return collisionMethod;
	}

	void Entity::setCollisionMethod(Entity::CollisionMethod collisionMethod_arg)
	{
		collisionMethod = collisionMethod_arg;
	}

	bool Entity::isStaticCollisionBody() const
	{
		return staticCollisionBody;
	}

	void Entity::setStaticCollisionBody(bool staticCollisionBody_arg)
	{
		staticCollisionBody = staticCollisionBody_arg;
	}

	bool Entity::loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error)
	{
		bool animationSuccess = assetManager->loadAnimationData(path, error);
		if(animationSuccess)
		{
			animations.add(assetManager->getAnimationData(path));
			return true;
		}
		return false;
	}

	void Entity::changeAnimation(const fgl::String& name, std::function<void(AnimationEventType)> onevent)
	{
		if(name.length()==0)
		{
			throw fgl::IllegalArgumentException("name", "cannot be an empty string");
		}
		AnimationData* animData = getAnimationData(name);
		if(animData==nullptr)
		{
			throw fgl::IllegalArgumentException("name", "no animation exists with the name "+name);
		}
		animationChanged = true;
		std::function<void(AnimationEventType)> prevAnimationEventHandler = currentAnimationEventHandler;
		currentAnimationName = name;
		currentAnimationFrame = 0;
		currentAnimationLastFrameTime = 0;
		currentAnimationEventHandler = onevent;
		if(prevAnimationEventHandler)
		{
			prevAnimationEventHandler(ANIMATIONEVENT_CHANGED);
		}
	}

	fgl::Animation* Entity::getAnimation(const fgl::String& name) const
	{
		AnimationData* animData = getAnimationData(name);
		if(animData!=nullptr)
		{
			return animData->getAnimation();
		}
		return nullptr;
	}

	fgl::Animation* Entity::getCurrentAnimation() const
	{
		return getAnimation(currentAnimationName);
	}

	fgl::ArrayList<CollisionRect*> Entity::createCollisionRects() const
	{
		switch(collisionMethod)
		{
			case COLLISIONMETHOD_NONE:
			return {};

			case COLLISIONMETHOD_FRAME:
			{
				fgl::Vector2d size = getSize();
				float rotation = 0;
				fgl::Vector2d position = getPosition(&rotation);
				fgl::RectangleD rect = fgl::RectangleD(position.x-(size.x/2), position.y-(size.y/2), size.x, size.y);
				if(rotation!=0.0)
				{
					return {new BoxCollisionRect("all", rect, velocity, rotation, fgl::Vector2d(size.x/2, size.y/2), fgl::Vector2d((double)scale, (double)scale))};
				}
				return {new BoxCollisionRect("all", rect, velocity, fgl::Vector2d((double)scale, (double)scale))};
			}

			case COLLISIONMETHOD_BOUNDS:
			{
				fgl::Vector2d size = getSize();
				float rotation = 0;
				fgl::Vector2d position = getPosition(&rotation);
				AnimationData* animData = getAnimationData(currentAnimationName);
				if(animData==nullptr)
				{
					return {};
				}
				fgl::ArrayList<AnimationData::MetaBounds> boundsList = animData->getBounds(currentAnimationFrame, getAnimationOrientation());
				if(boundsList.size()==0)
				{
					return {};
				}
				//TODO add a multi-box collision rect
				fgl::ArrayList<CollisionRect*> collisionRects;
				collisionRects.reserve(boundsList.size());
				for(size_t i=0; i<boundsList.size(); i++)
				{
					auto& metaBounds = boundsList[i];
					fgl::RectangleD bounds = fgl::RectangleD((metaBounds.rect.x*scale), (metaBounds.rect.y*scale), metaBounds.rect.width*scale, metaBounds.rect.height*scale);
					fgl::Vector2d origin = fgl::Vector2d(bounds.x-(size.x/2), bounds.y-(size.y/2));
					bounds = fgl::RectangleD(position.x+origin.x, position.y+origin.y, bounds.width, bounds.height);
					fgl::String tag;
					if(metaBounds.tag!=-1)
					{
						tag = (fgl::String)"bounds:"+metaBounds.tag;
					}
					if(rotation!=0.0)
					{
						collisionRects.add(new BoxCollisionRect(tag, bounds, velocity, rotation, origin, fgl::Vector2d((double)scale, (double)scale)));
					}
					collisionRects.add(new BoxCollisionRect(tag, bounds, velocity, fgl::Vector2d((double)scale, (double)scale)));
				}
				return collisionRects;
			}

			case COLLISIONMETHOD_PIXEL:
			{
				fgl::Vector2d size = getSize();
				float rotation = 0;
				fgl::Vector2d position = getPosition(&rotation);
				AnimationData* animData = getAnimationData(currentAnimationName);
				if(animData==nullptr)
				{
					return {};
				}
				fgl::Animation* animation = animData->getAnimation();
				if(animation==nullptr)
				{
					return {};
				}
				fgl::TextureImage* img = animation->getImage(currentAnimationFrame);
				fgl::RectangleU srcRect = animation->getImageSourceRect(currentAnimationFrame);
				bool mirroredHorizontal = animData->isMirrored(getAnimationOrientation());
				if(rotation!=0.0)
				{
					return {new PixelCollisionRect("all", fgl::RectangleD(position.x-(size.x/2), position.y-(size.y/2), size.x, size.y), velocity, srcRect, rotation, fgl::Vector2d(size.x/2, size.y/2), img, mirroredHorizontal, false)};
				}
				return {new PixelCollisionRect("all", fgl::RectangleD(position.x-(size.x/2), position.y-(size.y/2), size.x, size.y), velocity, srcRect, img, mirroredHorizontal, false)};
			}
		}
		throw fgl::IllegalStateException("invalid collisionMethod enum value");
	}

	void Entity::anchorChildEntity(Entity* child, AnimationMetaPoint::Type childPoint, size_t childPointIndex, AnimationMetaPoint::Type parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset)
	{
		if(child->parentEntity!=nullptr)
		{
			throw fgl::IllegalArgumentException("child", "already anchored to a parent");
		}
		//TODO check for another item attached at the same place? idk revisit this when I'm sober
		Anchor anchor;
		child->offset.x = childOffset.x;
		child->offset.y = childOffset.y;
		child->parentEntity = this;
		anchor.entity = child;
		anchor.childPoint = childPoint;
		anchor.childPointIndex = childPointIndex;
		anchor.parentPoint = parentPoint;
		anchor.parentPointIndex = parentPointIndex;
		anchoredEntities.add(anchor);
	}
	
	void Entity::removeAnchoredEntity(Entity* child)
	{
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			const Anchor& anchor = anchoredEntities[i];
			if(anchor.entity==child)
			{
				fgl::Vector2d position = child->getPosition();
				//TODO do something about rotation when it gets released? maybe pass a pointer as an optional parameter? idk
				child->offset.x = position.x;
				child->offset.y = position.y;
				child->parentEntity = nullptr;
				anchoredEntities.remove(i);
			}
		}
	}

	void Entity::shift(const fgl::Vector2d& shiftOffset)
	{
		if(parentEntity!=nullptr)
		{
			parentEntity->shift(shiftOffset);
			return;
		}
		offset.x += shiftOffset.x;
		offset.y += shiftOffset.y;

		offset.y = fgl::Math::round(offset.y, 12);
		offset.x = fgl::Math::round(offset.x, 12);
	}

	void Entity::setVelocity(const fgl::Vector2d& velocity_arg)
	{
		velocity = velocity_arg;
	}

	const fgl::Vector2d& Entity::getVelocity() const
	{
		return velocity;
	}

	void Entity::onCollision(Entity* entity, CollisionSide side)
	{
		//
	}

	AnimationData* Entity::getAnimationData(const fgl::String& name) const
	{
		for(size_t animations_size=animations.size(), i=0; i<animations_size; i++)
		{
			AnimationData* animData = animations[i];
			if(animData->getName()==name)
			{
				return animData;
			}
		}
		return nullptr;
	}

	Entity::Anchor Entity::getAnchor(const Entity* entity) const
	{
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			if(anchoredEntities[i].entity)
			{
				return anchoredEntities[i];
			}
		}
		throw fgl::IllegalArgumentException("entity", "no anchor exists for the given entity");
	}

	bool Entity::getAnchorData(fgl::Vector2d* posOffset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const
	{
		//TODO determine how offset should behave with orientation
		if(parentEntity!=nullptr)
		{
			Anchor anchor = parentEntity->getAnchor(this);
			AnimationData* parentAnimData = parentEntity->getAnimationData(parentEntity->currentAnimationName);
			AnimationData* childAnimData = getAnimationData(currentAnimationName);
			if(parentAnimData==nullptr || childAnimData==nullptr)
			{
				setOptionalArg(posOffset, fgl::Vector2d(0,0))
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			fgl::ArrayList<AnimationMetaPoint> parentMetaPoints = parentAnimData->getMetaPoints(parentEntity->currentAnimationFrame, anchor.parentPoint);
			fgl::ArrayList<AnimationMetaPoint> childMetaPoints = childAnimData->getMetaPoints(currentAnimationFrame, anchor.childPoint);
			if(anchor.parentPointIndex >= parentMetaPoints.size() || anchor.childPointIndex >= childMetaPoints.size())
			{
				setOptionalArg(posOffset, fgl::Vector2d(0, 0))
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			AnimationMetaPoint parentMetaPoint = parentMetaPoints[anchor.parentPointIndex];
			AnimationMetaPoint childMetaPoint = childMetaPoints[anchor.childPointIndex];
			float parentRotation = parentMetaPoint.rotation;
			float childRotation = childMetaPoint.rotation;

			fgl::Vector2d parentSize = parentEntity->getSize();
			fgl::Vector2d parentPointOffset = fgl::Vector2d(((double)parentMetaPoint.x*parentEntity->scale)-(parentSize.x/2), ((double)parentMetaPoint.y*parentEntity->scale)-(parentSize.y/2));
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			AnimationOrientation parentAnimationOrientation = parentAnimData->getOrientation();
			if(parentOrientation!=parentAnimationOrientation && parentAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				parentPointOffset.x = -parentPointOffset.x;
				parentRotation = -parentRotation;
			}

			fgl::Vector2d childSize = getSize();
			fgl::Vector2d childPointOffset = fgl::Vector2d(((double)childMetaPoint.x*scale)-(childSize.x/2), ((double)childMetaPoint.y*scale)-(childSize.y/2));
			AnimationOrientation childOrientation = getAnimationOrientation();
			AnimationOrientation childAnimationOrientation = childAnimData->getOrientation();
			if(childOrientation!=childAnimationOrientation && childAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				childPointOffset.x = -childPointOffset.x;
				childRotation = -childRotation;
			}

			fgl::Vector2d totalOffset = parentPointOffset - childPointOffset;
			
			setOptionalArg(posOffset, totalOffset)
			setOptionalArg(rotation, parentRotation+childRotation)
			setOptionalArg(rotationPoint, parentPointOffset)
			setOptionalArg(behind, parentMetaPoint.behind)
			setOptionalArg(visible, parentMetaPoint.visible)
			return true;
		}
		setOptionalArg(posOffset, fgl::Vector2d(0, 0))
		setOptionalArg(rotation, 0)
		setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
		setOptionalArg(behind, false)
		setOptionalArg(visible, true)
		return true;
	}

	AnimationOrientation Entity::getAnimationOrientation() const
	{
		if(orientation==Entity::ORIENTATION_LEFT)
		{
			return ANIMATIONORIENTATION_LEFT;
		}
		else if(orientation==Entity::ORIENTATION_RIGHT)
		{
			return ANIMATIONORIENTATION_RIGHT;
		}
		throw fgl::IllegalStateException("Entity::orientation has an invalid value");
	}
}


#include <fightlib/entity/Entity.h>

namespace fl
{
	#define setOptionalArg(arg, value) if(arg!=nullptr){ *arg = value; }

	Entity::Entity(double x, double y) : x(x), y(y), scale(3.0f),
		currentAnimationName(),
		currentAnimationFrame(0),
		currentAnimationLastFrameTime(0),
		currentAnimationEventHandler(nullptr),
		animationChanged(false),
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
	}

	void Entity::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		graphics.translate(x, y);

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
			animData->drawFrame(currentAnimationFrame, selfGraphics, true);
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

	fgl::Vector2d Entity::getPosition(float* rotation) const
	{
		fgl::Vector2d parentOffset;
		float anchorRotation;
		fgl::Vector2d anchorRotationPoint;
		getAnchorData(&parentOffset, &anchorRotation, &anchorRotationPoint, nullptr, nullptr);
		fgl::Vector2d offset = parentOffset;
		if(anchorRotation!=0)
		{
			fgl::TransformD rotationTransform;
			rotationTransform.rotate(anchorRotation, anchorRotationPoint);
			offset = rotationTransform.transform(offset);
		}
		if(parentEntity!=nullptr)
		{
			float parentRotation = 0;
			fgl::Vector2d parentPosition = parentEntity->getPosition(&parentRotation);
			setOptionalArg(rotation, parentRotation+anchorRotation)
			return parentPosition+offset;
		}
		setOptionalArg(rotation, anchorRotation)
		return offset;
	}

	float Entity::getScale() const
	{
		return scale;
	}

	bool Entity::loadAnimation(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error)
	{
		AnimationData* animData = new AnimationData();
		bool animationSuccess = animData->loadFromFile(path, assetManager, error);
		if(animationSuccess)
		{
			animations.add(animData);
			return true;
		}
		else
		{
			delete animData;
			return false;
		}
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

	void Entity::anchorChildEntity(Entity* child, AnimationMetaPoint::PointType childPoint, size_t childPointIndex, AnimationMetaPoint::PointType parentPoint, size_t parentPointIndex, const fgl::Vector2d& offset)
	{
		if(child->parentEntity!=nullptr)
		{
			throw fgl::IllegalArgumentException("child", "already anchored to a parent");
		}
		//TODO check for another item attached at the same place? idk revisit this when I'm sober
		Anchor anchor;
		child->x = offset.x;
		child->y = offset.y;
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

	bool Entity::getAnchorData(fgl::Vector2d* offset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const
	{
		fgl::Vector2d childOffset = fgl::Vector2d(x, y);
		if(parentEntity!=nullptr)
		{
			Anchor anchor = parentEntity->getAnchor(this);
			AnimationData* parentAnimData = parentEntity->getAnimationData(parentEntity->currentAnimationName);
			AnimationData* childAnimData = getAnimationData(currentAnimationName);
			if(parentAnimData==nullptr || childAnimData==nullptr)
			{
				setOptionalArg(offset, childOffset)
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, childOffset)
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			fgl::ArrayList<AnimationMetaPoint> parentMetaPoints = parentAnimData->getMetaPoints(parentEntity->currentAnimationFrame, anchor.parentPoint);
			fgl::ArrayList<AnimationMetaPoint> childMetaPoints = childAnimData->getMetaPoints(currentAnimationFrame, anchor.childPoint);
			if(anchor.parentPointIndex >= parentMetaPoints.size() || anchor.childPointIndex >= childMetaPoints.size())
			{
				setOptionalArg(offset, childOffset)
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, childOffset)
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			AnimationMetaPoint parentMetaPoint = parentMetaPoints[anchor.parentPointIndex];
			AnimationMetaPoint childMetaPoint = childMetaPoints[anchor.childPointIndex];
			fgl::Vector2d parentSize = parentAnimData->getSize(parentEntity->currentAnimationFrame, parentEntity->scale);
			fgl::Vector2d parentPointOffset = fgl::Vector2d(((double)parentMetaPoint.x*parentEntity->scale)-(parentSize.x/2), ((double)parentMetaPoint.y*parentEntity->scale)-(parentSize.y/2));
			//TODO handle mirroring by making parentPointOffset.x negative
			fgl::Vector2d childSize = childAnimData->getSize(currentAnimationFrame, scale);
			fgl::Vector2d childPointOffset = fgl::Vector2d(((double)childMetaPoint.x*scale)-(childSize.x/2), ((double)childMetaPoint.y*scale)-(childSize.y/2));
			//TODO handle mirroring by making childPointOffset.x negative
			fgl::Vector2d totalOffset = parentPointOffset - childPointOffset;
			
			setOptionalArg(offset, totalOffset+childOffset)
			setOptionalArg(rotation, childMetaPoint.rotation)
			setOptionalArg(rotationPoint, parentPointOffset+childOffset)
			setOptionalArg(behind, parentMetaPoint.behind)
			setOptionalArg(visible, parentMetaPoint.visible)
			return true;
		}
		setOptionalArg(offset, childOffset)
		setOptionalArg(rotation, 0)
		setOptionalArg(rotationPoint, childOffset)
		setOptionalArg(behind, false)
		setOptionalArg(visible, true)
		return true;
	}
}

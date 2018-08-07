
#include <fightlib/entity/draw/Sprite.hpp>

namespace fl
{
	Sprite::Sprite(const fgl::Vector2d& position)
		: position(position),
		currentAnimationData(nullptr),
		currentAnimationFrame(0),
		currentAnimationLastFrameTime(0),
		currentAnimationEventHandler(nullptr),
		animationChanged(false)
	{
		//
	}

	Sprite::~Sprite()
	{
		//
	}
	
	bool Sprite::getFlag(const fgl::String& flag) const
	{
		if(flag=="Sprite")
		{
			return true;
		}
		return Drawable::getFlag(flag);
	}

	void Sprite::update(fgl::ApplicationData appData)
	{
		long long currentTimeMillis = appData.getTime().getMilliseconds();

		//handle animation
		bool frameChanged = false;
		do
		{
			frameChanged = false;
			if(animationChanged)
			{
				currentAnimationLastFrameTime = currentTimeMillis;
				animationChanged = false;
			}
			if(currentAnimationData!=nullptr)
			{
				fgl::Animation* animation = currentAnimationData->getAnimation();
				long double fps = (long double)animation->getFPS();
				long long frameTime = (long long)(1000.0L/fps);
				if(frameTime==0)
				{
					frameTime = 1;
				}
				long long nextFrameTime = currentAnimationLastFrameTime + frameTime;
				if(nextFrameTime<=currentTimeMillis)
				{
					frameChanged = true;
					currentAnimationFrame++;
					currentAnimationLastFrameTime = nextFrameTime;
					size_t totalFrames = animation->getFrameCount();
					if(currentAnimationFrame >= totalFrames)
					{
						currentAnimationFrame = 0;
						if(currentAnimationEventHandler)
						{
							currentAnimationEventHandler(ANIMATIONEVENT_FINISHED);
							if(!animationChanged && totalFrames > 1)
							{
								currentAnimationEventHandler(ANIMATIONEVENT_FRAMECHANGED);
							}
						}
						if(!animationChanged && totalFrames > 1)
						{
							onAnimationFrameChange();
						}
					}
					else
					{
						if(currentAnimationEventHandler)
						{
							currentAnimationEventHandler(ANIMATIONEVENT_FRAMECHANGED);
						}
						if(!animationChanged)
						{
							onAnimationFrameChange();
						}
					}
				}
			}
		} while(animationChanged || frameChanged);
	}

	fgl::TransformState Sprite::getDrawTransformState() const
	{
		return fgl::TransformState(position, 0);
	}

	fgl::Vector2d Sprite::getDrawScale() const
	{
		return fgl::Vector2d(1,1);
	}

	void Sprite::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		if(currentAnimationData!=nullptr)
		{
			auto drawTransform = getDrawTransformState();
			auto position = drawTransform.position;
			auto rotation = drawTransform.rotation;
			if(position.x!=0 || position.y!=0)
			{
				graphics.translate(position);
			}
			if(rotation!=0)
			{
				graphics.rotate(rotation);
			}
			auto scale = getDrawScale();
			graphics.scale(scale);
			
			//offset for origin
			auto origin = getOrigin()/scale;
			auto animSize = (fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame);
			auto originOffset = (animSize/2.0) - origin;
			graphics.translate(originOffset);
			
			currentAnimationData->drawFrame(currentAnimationFrame, graphics, getAnimationOrientation());
		}
	}
	
	fgl::Vector2d Sprite::getOrigin() const
	{
		return getSize()/2.0;
	}
	
	double Sprite::getRotation() const
	{
		return 0.0;
	}

	fgl::Vector2d Sprite::getPosition() const
	{
		return position;
	}

	void Sprite::setPosition(const fgl::Vector2d& position_arg)
	{
		position = position_arg;

		//TODO I may not need this anymore, but I should do some thorough tests before removing it
		position.y = fgl::Math::round(position.y, 12);
		position.x = fgl::Math::round(position.x, 12);
	}

	fgl::Vector2d Sprite::getSize() const
	{
		if(currentAnimationData==nullptr)
		{
			return fgl::Vector2d(0, 0);
		}
		return ((fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame)) * getDrawScale();
	}

	fgl::RectangleD Sprite::getFrame() const
	{
		auto size = getSize();
		auto origin = getOrigin();
		auto position = getPosition();
		auto rotation = getRotation();
		auto frame = fgl::RectangleD(position.x-origin.x, position.y-origin.y, size.x, size.y);
		if(rotation!=0)
		{
			fgl::TransformD transform;
			transform.rotate((double)rotation, position);
			frame = transform.transform(frame);
		}
		return frame;
	}

	bool Sprite::loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error)
	{
		bool animationSuccess = assetManager->loadAnimationData(path, error);
		if(animationSuccess)
		{
			animations.add(assetManager->getAnimationData(path));
			return true;
		}
		return false;
	}

	void Sprite::changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent)
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
		currentAnimationData = animData;
		currentAnimationFrame = 0;
		currentAnimationLastFrameTime = 0;
		currentAnimationEventHandler = onevent;
		if(prevAnimationEventHandler)
		{
			prevAnimationEventHandler(ANIMATIONEVENT_CHANGED);
		}
		onAnimationChange();
	}

	fgl::Animation* Sprite::getAnimation(const fgl::String& name) const
	{
		AnimationData* animData = getAnimationData(name);
		if(animData!=nullptr)
		{
			return animData->getAnimation();
		}
		return nullptr;
	}

	fgl::Animation* Sprite::getCurrentAnimation() const
	{
		if(currentAnimationData==nullptr)
		{
			return nullptr;
		}
		return currentAnimationData->getAnimation();
	}

	fgl::String Sprite::getCurrentAnimationName() const
	{
		if(currentAnimationData==nullptr)
		{
			return "";
		}
		return currentAnimationData->getName();
	}

	size_t Sprite::getCurrentAnimationFrameIndex() const
	{
		return currentAnimationFrame;
	}

	AnimationData* Sprite::getCurrentAnimationData() const
	{
		return currentAnimationData;
	}

	AnimationOrientation Sprite::getAnimationOrientation() const
	{
		return ANIMATIONORIENTATION_NEUTRAL;
	}

	AnimationData* Sprite::getAnimationData(const fgl::String& name) const
	{
		for(auto animData : animations)
		{
			if(animData->getName()==name)
			{
				return animData;
			}
		}
		return nullptr;
	}

	fgl::ArrayList<TaggedBox> Sprite::getMetaPointBoxes(MetaPointType metaPointType) const
	{
		if(currentAnimationData==nullptr || currentAnimationData->getAnimation()->getFrameCount()==0)
		{
			return {};
		}
		else
		{
			auto metaPoints = currentAnimationData->getMetaPoints(currentAnimationFrame, metaPointType);
			if(metaPoints.size()==0)
			{
				return {};
			}

			auto position = getPosition();
			auto rotation = getRotation();
			auto origin = getOrigin();
			auto size = getSize();
			auto topLeft = position - origin;

			auto animSize = (fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame);
			auto sizeScale = size/animSize;
			bool mirrored = currentAnimationData->isMirrored(getAnimationOrientation());

			fgl::TransformD transform;
			if(rotation!=0)
			{
				transform.rotate((double)rotation, position);
			}

			fgl::ArrayList<TaggedBox> boxes;
			boxes.reserve(metaPoints.size());
			for(auto& metaPoint : metaPoints)
			{
				fgl::Vector2d metaPointCenter = fgl::Vector2d((double)metaPoint.x, (double)metaPoint.y);
				if(mirrored)
				{
					metaPointCenter.x = animSize.x - metaPointCenter.x;
				}
				metaPointCenter = topLeft + (metaPointCenter*sizeScale);
				if(rotation!=0)
				{
					metaPointCenter = transform.transform(metaPointCenter);
				}
				fgl::Vector2d metaPointSize = fgl::Vector2d(metaPoint.radius*2, metaPoint.radius*2) * sizeScale;
				TaggedBox box;
				box.tag = metaPoint.tag;
				box.rect = fgl::RectangleD(metaPointCenter.x-(metaPointSize.x/2), metaPointCenter.y-(metaPointSize.y/2), metaPointSize.x, metaPointSize.y);
				boxes.add(box);
			}
			return boxes;
		}
	}
	
	void Sprite::onAnimationFrameChange()
	{
		//
	}
	
	void Sprite::onAnimationChange()
	{
		//
	}
}

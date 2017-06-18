
#include <fightlib/entity/draw/Animatable.hpp>

namespace fl
{
	Animatable::Animatable()
		: currentAnimationData(nullptr),
		currentAnimationFrame(0),
		currentAnimationLastFrameTime(0),
		currentAnimationEventHandler(nullptr),
		animationChanged(false)
	{
		//
	}

	Animatable::~Animatable()
	{
		//
	}

	void Animatable::update(fgl::ApplicationData appData)
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
		} while(animationChanged || frameChanged);
	}

	void Animatable::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		auto position = getDrawPosition();
		auto rotation = getDrawRotation();
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
		if(currentAnimationData!=nullptr)
		{
			currentAnimationData->drawFrame(currentAnimationFrame, graphics, getAnimationOrientation());
		}
	}

	fgl::Vector2d Animatable::getSize() const
	{
		if(currentAnimationData==nullptr)
		{
			return fgl::Vector2d(0, 0);
		}
		return ((fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame)) * getDrawScale();
	}

	bool Animatable::loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error)
	{
		bool animationSuccess = assetManager->loadAnimationData(path, error);
		if(animationSuccess)
		{
			animations.add(assetManager->getAnimationData(path));
			return true;
		}
		return false;
	}

	void Animatable::changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent)
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
	}

	fgl::Animation* Animatable::getAnimation(const fgl::String& name) const
	{
		AnimationData* animData = getAnimationData(name);
		if(animData!=nullptr)
		{
			return animData->getAnimation();
		}
		return nullptr;
	}

	fgl::Animation* Animatable::getCurrentAnimation() const
	{
		if(currentAnimationData==nullptr)
		{
			return nullptr;
		}
		return currentAnimationData->getAnimation();
	}

	fgl::String Animatable::getCurrentAnimationName() const
	{
		if(currentAnimationData==nullptr)
		{
			return "";
		}
		return currentAnimationData->getName();
	}

	size_t Animatable::getCurrentAnimationFrameIndex() const
	{
		return currentAnimationFrame;
	}

	AnimationData* Animatable::getCurrentAnimationData() const
	{
		return currentAnimationData;
	}

	AnimationOrientation Animatable::getAnimationOrientation() const
	{
		return ANIMATIONORIENTATION_NEUTRAL;
	}

	AnimationData* Animatable::getAnimationData(const fgl::String& name) const
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
	
	fgl::ArrayList<TaggedBox> Animatable::getMetaPointBoxes(MetaPointType metaPointType) const
	{
		if(currentAnimationData==nullptr || currentAnimationData->getAnimation()->getTotalFrames()==0)
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
			
			double rotation = getRotation();
			fgl::Vector2d position = getPosition();
			fgl::Vector2d size = getSize();
			fgl::Vector2d topLeft = position - (size/2.0);
			
			fgl::Vector2d animSize = (fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame);
			fgl::Vector2d sizeScale = size/animSize;
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
}

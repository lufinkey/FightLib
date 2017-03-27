
#include <fightlib/entity/draw/Animatable.hpp>

namespace fl
{
	Animatable::Animatable(const fgl::Vector2d& position)
		: position(position),
		currentAnimationData(nullptr),
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

	void Animatable::update(const fgl::ApplicationData& appData)
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
			if(currentAnimationData!=nullptr)
			{
				fgl::Animation* animation = currentAnimationData->getAnimation();
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

	void Animatable::draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const
	{
		graphics.translate(position.x, position.y);
		float scale = getScale();
		graphics.scale(scale, scale);
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
		return ((fgl::Vector2d)currentAnimationData->getSize(currentAnimationFrame)) * (double)getScale();
	}

	fgl::Vector2d Animatable::getPosition(float* rotation) const
	{
		if(rotation!=nullptr)
		{
			rotation = 0;
		}
		return position;
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

	AnimationData* Animatable::getCurrentAnimationData() const
	{
		return currentAnimationData;
	}

	float Animatable::getScale() const
	{
		return 1.0f;
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
}

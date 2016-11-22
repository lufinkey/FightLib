
#include <fightlib/entity/Entity.h>

namespace fl
{
	Entity::Entity(double x, double y) : x(x), y(y),
		currentAnimationName(),
		currentAnimationFrame(0),
		currentAnimationLastFrameTime(0),
		currentAnimationEventHandler(nullptr),
		animationChanged(false)
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
				long long frameTime = (long long)(1.0L/fps);
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
		AnimationData* animData = getAnimationData(currentAnimationName);
		if(animData!=nullptr)
		{
			animData->drawFrame(x, y, 1, currentAnimationFrame, graphics);
		}
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
}

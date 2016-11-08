
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
}

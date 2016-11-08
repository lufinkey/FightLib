
#include <fightlib/entity/action/AnimationData.h>

namespace fl
{
	Hitbox::Hitbox():
		index(0),
		x(0),
		y(0),
		radius(0)
	{
		//
	}

	AnimationMetaPoint::AnimationMetaPoint() :
		x(0),
		y(0),
		radius(0),
		rotation(0),
		type(POINTTYPE_HEAD),
		orientation(ORIENTATION_LEFT),
		behind(false),
		visible(false)
	{
		//
	}

	AnimationData::AnimationData() : animation(nullptr)
	{
		//
	}

	AnimationData::~AnimationData()
	{
		if(animation!=nullptr)
		{
			delete animation;
		}
	}

	bool AnimationData::loadFromFile(const fgl::String& path, fgl::String* error)
	{
		//TODO load from file
		if(error!=nullptr)
		{
			*error = "Loading AnimationData from file is not implemented yet!";
		}
		return false;
	}

	void AnimationData::drawFrame(double x, double y, double scale, size_t frameIndex, fgl::Graphics graphics)
	{
		if(animation!=nullptr)
		{
			graphics.translate(x, y);
			graphics.scale(scale, scale);
			animation->drawFrame(graphics, frameIndex);
		}
	}

	const fgl::String& AnimationData::getName() const
	{
		return name;
	}

	fgl::Animation* AnimationData::getAnimation() const
	{
		return animation;
	}
}

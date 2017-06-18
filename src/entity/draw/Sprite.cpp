
#include <fightlib/entity/draw/Sprite.hpp>

namespace fl
{
	Sprite::Sprite(const fgl::Vector2d& position)
		: position(position),
		scale(1.0, 1.0),
		rotation(0.0),
		animationOrientation(ANIMATIONORIENTATION_NEUTRAL)
	{
		//
	}

	fgl::Vector2d Sprite::getPosition() const
	{
		return position;
	}

	void Sprite::setPosition(const fgl::Vector2d& position_arg)
	{
		position = position_arg;
	}

	double Sprite::getRotation() const
	{
		return rotation;
	}

	void Sprite::setRotation(double rotation_arg)
	{
		rotation = rotation_arg;
	}

	fgl::Vector2d Sprite::getScale() const
	{
		return scale;
	}

	void Sprite::setScale(const fgl::Vector2d& scale_arg)
	{
		scale = scale_arg;
	}

	AnimationOrientation Sprite::getAnimationOrientation() const
	{
		return animationOrientation;
	}

	void Sprite::setAnimationOrientation(AnimationOrientation animationOrientation_arg)
	{
		animationOrientation = animationOrientation_arg;
	}

	fgl::Vector2d Sprite::getDrawPosition() const
	{
		return position;
	}

	double Sprite::getDrawRotation() const
	{
		return rotation;
	}

	fgl::Vector2d Sprite::getDrawScale() const
	{
		return scale;
	}
}

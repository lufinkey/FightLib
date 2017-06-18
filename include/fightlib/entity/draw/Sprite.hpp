
#pragma once

#include "Animatable.hpp"

namespace fl
{
	class Sprite : public Animatable
	{
	public:
		Sprite(const fgl::Vector2d& position);

		virtual fgl::Vector2d getPosition() const override;
		virtual void setPosition(const fgl::Vector2d& position) override;

		virtual double getRotation() const override;
		virtual void setRotation(double degrees) override;

		virtual fgl::Vector2d getScale() const;
		virtual void setScale(const fgl::Vector2d& scale);

		virtual AnimationOrientation getAnimationOrientation() const override;
		virtual void setAnimationOrientation(AnimationOrientation animationOrientation);

	protected:
		virtual fgl::Vector2d getDrawPosition() const override;
		virtual double getDrawRotation() const override;
		virtual fgl::Vector2d getDrawScale() const override;

	private:
		fgl::Vector2d position;
		fgl::Vector2d scale;
		double rotation;
		fl::AnimationOrientation animationOrientation;
	};
}

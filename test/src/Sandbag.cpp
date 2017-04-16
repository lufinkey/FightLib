
#include "Sandbag.hpp"

Sandbag::Sandbag(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation)
	: fl::Character(position, orientation)
{
	setScale(3.0);

	loadAnimation("assets/animations/sandbag/idle.plist", assetManager);
	loadAnimation("assets/animations/sandbag/hurt-front.plist", assetManager);
	changeAnimation("idle");
}

fgl::String Sandbag::getIdleAnimationName() const
{
	return "idle";
}

fgl::String Sandbag::getMoveAnimationName(double amount) const
{
	return "idle";
}

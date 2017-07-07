
#include "Sandbag.hpp"
#include "HurtAction.hpp"

Sandbag::Sandbag(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation)
	: fl::Character(position, orientation)
{
	setScale(3.0);

	loadAnimation("assets/animations/sandbag/idle.plist", assetManager);
	loadAnimation("assets/animations/sandbag/hit-front.plist", assetManager);
	changeAnimation("idle");

	addAction("hurt", new HurtAction());
}

fgl::String Sandbag::getDefaultAnimationName() const
{
	return "idle";
}

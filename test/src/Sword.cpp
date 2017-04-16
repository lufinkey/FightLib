
#include "Sword.hpp"

Sword::Sword(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation)
	: Item(position, orientation)
{
	loadAnimation("assets/animations/sword/sword.plist", assetManager);
	setScale(2.0);
	changeAnimation("sword");
}

fgl::ArrayList<fl::MetaPointType> Sword::getAnchorPoints() const
{
	return {fl::METAPOINT_LEFTHAND, fl::METAPOINT_RIGHTHAND};
}

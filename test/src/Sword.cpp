
#include "Sword.hpp"

Sword::Sword(const fgl::Vector2d& position, fl::Orientation orientation)
	: Item(position, orientation)
{
	//
}

fgl::ArrayList<fl::AnimationMetaPoint::Type> Sword::getAnchorPoints() const
{
	return {fl::AnimationMetaPoint::POINTTYPE_LEFTHAND, fl::AnimationMetaPoint::POINTTYPE_RIGHTHAND};
}

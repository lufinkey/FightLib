
#include "Sword.hpp"

Sword::Sword(const fgl::Vector2d& position, fl::Orientation orientation)
	: Item(position, orientation)
{
	//
}

fgl::ArrayList<fl::MetaPointType> Sword::getAnchorPoints() const
{
	return {fl::METAPOINT_LEFTHAND, fl::METAPOINT_RIGHTHAND};
}

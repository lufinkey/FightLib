
#pragma once

#include <fightlib/fightlib.hpp>

class Sandbag : public fl::Character
{
public:
	Sandbag(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation);

	virtual fgl::String getIdleAnimationName() const override;
	virtual fgl::String getMoveAnimationName(double amount) const override;
};


#pragma once

#include <fightlib/fightlib.hpp>

class Player : public fl::Character
{
public:
	Player(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation);

	virtual void update(fgl::ApplicationData appData) override;

	virtual fgl::String getDefaultAnimationName() const override;

	virtual fgl::ArrayList<fl::MetaPointType> getItemAnchorPoints() const override;
};

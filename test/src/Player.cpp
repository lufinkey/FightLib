
#include "Player.hpp"
#include "JumpAction.hpp"
#include "PickUpItemAction.hpp"

Player::Player(fl::AnimationAssetManager* assetManager, const fgl::Vector2d& position, fl::Orientation orientation)
	: Character(position, orientation)
{
	setScale(3.0);

	loadAnimation("assets/animations/player/idle.plist", assetManager);
	loadAnimation("assets/animations/player/walk.plist", assetManager);
	loadAnimation("assets/animations/player/jump.plist", assetManager);
	loadAnimation("assets/animations/player/jump2.plist", assetManager);
	loadAnimation("assets/animations/player/fall.plist", assetManager);
	loadAnimation("assets/animations/player/punch.plist", assetManager);
	changeAnimation("idle");
	
	addAction("jump", new JumpAction());
	addAction("pickUp", new PickUpItemAction());
}

void Player::update(const fgl::ApplicationData& appData)
{
	Character::update(appData);
}

fgl::String Player::getIdleAnimationName() const
{
	if(isOnGround())
	{
		return "idle";
	}
	return "fall";
}

fgl::String Player::getMoveAnimationName(double amount) const
{
	if(isOnGround())
	{
		return "walk";
	}
	return "fall";
}

fgl::ArrayList<fl::MetaPointType> Player::getItemAnchorPoints() const
{
	return {fl::METAPOINT_RIGHTHAND, fl::METAPOINT_HEAD};
}

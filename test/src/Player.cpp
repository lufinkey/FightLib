
#include "Player.hpp"
#include "JumpAction.hpp"
#include "PickUpItemAction.hpp"
#include "PunchAttack.hpp"

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
	addAction("punch", new PunchAttack());
}

void Player::update(fgl::ApplicationData appData)
{
	Character::update(appData);
}

fgl::String Player::getDefaultAnimationName() const
{
	if(isOnGround())
	{
		if(getDirection().x==0)
		{
			return "idle";
		}
		return "walk";
	}
	return "fall";
}

fgl::ArrayList<fl::MetaPointType> Player::getItemAnchorPoints() const
{
	return {fl::METAPOINT_RIGHTHAND, fl::METAPOINT_HEAD};
}

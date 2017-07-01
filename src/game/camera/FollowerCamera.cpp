
#include <fightlib/game/camera/FollowerCamera.hpp>

namespace fl
{
	FollowerCamera::FollowerCamera()
		: focus(nullptr),
		center(0,0),
		followRegion(0.25, 0.25, 0.75, 0.75)
	{
		//
	}

	void FollowerCamera::setFocus(Character* focus_arg)
	{
		focus = focus_arg;
	}

	Character* FollowerCamera::getFocus() const
	{
		return focus;
	}

	void FollowerCamera::update(fgl::ApplicationData appData)
	{
		Camera::update(appData);
		if(focus!=nullptr)
		{
			lastFocusPosition = focusPosition;
			focusPosition = focus->getPosition();
			auto velocity = focusPosition - lastFocusPosition;
			velocity.x = fgl::Math::abs(velocity.x);
			velocity.y = fgl::Math::abs(velocity.y);
			if(velocity.x < 200.0)
			{
				velocity.x = 200.0;
			}
			if(velocity.y < 200.0)
			{
				velocity.y = 200.0;
			}
			auto position = focus->getPosition();
			auto rect = getRect();
			if(position.x < (rect.x+(followRegion.left*rect.width)))
			{
				center.x -= velocity.x*appData.getFrameSpeedMultiplier();
			}
			if(position.x > (rect.x+(followRegion.right*rect.width)))
			{
				center.x += velocity.x*appData.getFrameSpeedMultiplier();
			}
			if(position.y < (rect.y+(followRegion.top*rect.height)))
			{
				center.y -= velocity.y*appData.getFrameSpeedMultiplier();
			}
			if(position.y > (rect.y+(followRegion.bottom*rect.height)))
			{
				center.y += velocity.y*appData.getFrameSpeedMultiplier();
			}
		}
	}

	void FollowerCamera::drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		graphics.translate(-center+(getFrameSize()/2.0));
		Camera::drawStage(appData, graphics);
	}

	fgl::RectangleD FollowerCamera::getRect() const
	{
		auto size = getFrameSize();
		return fgl::RectangleD(center.x-(size.x/2), center.y-(size.y/2), size.x, size.y);
	}
}

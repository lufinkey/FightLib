
#include <fightlib/game/camera/FollowerCamera.hpp>

namespace fl
{
	FollowerCamera::FollowerCamera(const fgl::Vector2d& center)
		: focus(nullptr),
		focusPosition(center),
		lastFocusPosition(center),
		center(center),
		followRegion(0.34, 0.34, 0.66, 0.66)
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
	
	void FollowerCamera::onBeginFight(Fight* fight)
	{
		if(focus!=nullptr)
		{
			focusPosition = focus->getPosition();
			lastFocusPosition = focusPosition;
			center = focusPosition;
		}
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
			auto rect = getRect();
			if(focusPosition.x < (rect.x+(followRegion.left*rect.width)))
			{
				center.x -= velocity.x*appData.getFrameSpeedMultiplier();
			}
			if(focusPosition.x > (rect.x+(followRegion.right*rect.width)))
			{
				center.x += velocity.x*appData.getFrameSpeedMultiplier();
			}
			if(focusPosition.y < (rect.y+(followRegion.top*rect.height)))
			{
				center.y -= velocity.y*appData.getFrameSpeedMultiplier();
			}
			if(focusPosition.y > (rect.y+(followRegion.bottom*rect.height)))
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


#include <fightlib/game/camera/FollowerCamera.hpp>

namespace fl
{
	FollowerCamera::FollowerCamera()
		: focus(nullptr)
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
	}

	void FollowerCamera::drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		if(focus!=nullptr)
		{
			auto frameSize = getFrameSize();
			graphics.translate(-focus->getPosition()+(frameSize/2.0));
		}
		Camera::drawStage(appData, graphics);
	}
}

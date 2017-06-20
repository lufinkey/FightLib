
#include <fightlib/game/camera/Camera.hpp>
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Camera::Camera()
		: fight(nullptr)
	{
		//
	}

	Camera::~Camera()
	{
		//
	}

	fgl::Vector2d Camera::getFrameSize() const
	{
		if(fight==nullptr)
		{
			return fgl::Vector2d(0, 0);
		}
		auto frame = fight->getFrame();
		return fgl::Vector2d(frame.width, frame.height);
	}

	void Camera::onBeginFight(Fight* fight)
	{
		//
	}

	void Camera::onFrameSizeChange(fgl::Vector2d oldSize, fgl::Vector2d newSize)
	{
		//
	}

	void Camera::update(fgl::ApplicationData appData)
	{
		//
	}

	void Camera::drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		fight->getStage()->draw(appData, graphics);
	}
}

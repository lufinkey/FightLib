
#include <fightlib/game/Camera.hpp>
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Camera::Camera()
		: fight(nullptr)
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

	void Camera::onBeginFight(Fight* fight)
	{
		//
	}
}

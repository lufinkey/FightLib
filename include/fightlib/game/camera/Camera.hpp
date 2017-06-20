
#pragma once

#include <fightlib/stage/Stage.hpp>

namespace fl
{
	class Fight;

	class Camera
	{
		friend class Fight;
	public:
		Camera();
		virtual ~Camera();

		fgl::Vector2d getFrameSize() const;

	protected:
		virtual void onBeginFight(Fight* fight);
		virtual void onFrameSizeChange(fgl::Vector2d oldSize, fgl::Vector2d newSize);

		virtual void update(fgl::ApplicationData appData);
		virtual void drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const;

	private:
		Fight* fight;
	};
}

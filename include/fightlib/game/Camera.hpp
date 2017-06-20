
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

	protected:
		virtual void update(fgl::ApplicationData appData);
		virtual void drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		virtual void onBeginFight(Fight* fight);

	private:
		Fight* fight;
	};
}

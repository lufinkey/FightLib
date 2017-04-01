
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Fight
	{
	public:
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

	private:
		fgl::RectangleD frame;
	};
}

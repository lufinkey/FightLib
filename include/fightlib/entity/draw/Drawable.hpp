
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Drawable
	{
	public:
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const = 0;
	};
}


#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Drawable
	{
	public:
		virtual ~Drawable() {}

		virtual void update(fgl::ApplicationData appData) {}
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const = 0;

		virtual bool getFlag(const fgl::String& flag) const { return false; }
	};
}

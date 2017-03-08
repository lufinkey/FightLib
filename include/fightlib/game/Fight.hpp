
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Fight
	{
	public:
		virtual void update(GameLibrary::ApplicationData appData);
		virtual void draw(GameLibrary::ApplicationData appData, GameLibrary::Graphics graphics) const;

	private:
		GameLibrary::RectangleD frame;
	};
}

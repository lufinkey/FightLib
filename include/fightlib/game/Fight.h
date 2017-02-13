
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Fight : public GameLibrary::UpdateDrawable
	{
	public:
		virtual void update(GameLibrary::ApplicationData appData) override;
		virtual void draw(GameLibrary::ApplicationData appData, GameLibrary::Graphics graphics) const override;

	private:
		GameLibrary::RectangleD frame;
	};
}

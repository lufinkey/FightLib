
#pragma once

#include "FightParams.hpp"

namespace fl
{
	class Fight
	{
	public:
		Fight(const FightParams& params);
		
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

	private:
		fgl::RectangleD frame;
		
		FightParams params;
	};
}

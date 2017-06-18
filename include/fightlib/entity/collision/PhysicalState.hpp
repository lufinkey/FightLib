
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class PhysicalState
	{
	public:
		PhysicalState();

		fgl::Vector2d position;
		fgl::Vector2d velocity;
		double rotation;
		double rotationalVelocity;
	};
}

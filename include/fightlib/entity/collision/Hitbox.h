
#pragma once

namespace fl
{
	class Hitbox
	{
	public:
		Hitbox() : index(0), x(0), y(0), radius(0) {}
		Hitbox(size_t index, double x, double y, double radius) : index(index), x(x), y(y), radius(radius) {}

		size_t index;
		double x;
		double y;
		double radius;
	};
}

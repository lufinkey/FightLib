
#pragma once

#include "Drawable.hpp"

namespace fl
{
	class DrawManager
	{
	public:
		void addDrawable(Drawable* drawable);
		void removeDrawable(Drawable* drawable);
		
		void update(const fgl::ApplicationData& appData);
		void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const;
		
	private:
		fgl::ArrayList<Drawable*> drawables;
	};
}

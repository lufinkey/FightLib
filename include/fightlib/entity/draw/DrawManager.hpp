
#pragma once

#include "Drawable.hpp"

namespace fl
{
	class DrawManager
	{
	public:
		void addDrawable(Drawable* drawable, float zLayer=0.5f);
		void removeDrawable(Drawable* drawable);

		float getDrawableZLayer(Drawable* drawable) const;
		void setDrawableZLayer(Drawable* drawable, float zLayer);

		fgl::ArrayList<Drawable*> getDrawables() const;
		
		void update(const fgl::ApplicationData& appData);
		void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const;
		
	private:
		struct DrawableData
		{
			Drawable* drawable;
			float zLayer;
		};

		fgl::ArrayList<DrawableData> drawables;
	};
}

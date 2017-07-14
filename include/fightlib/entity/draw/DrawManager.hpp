
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
		
		void addDrawFilter(const fgl::String& name, const std::function<void(Drawable*, fgl::Graphics&)>& filterFunc);
		void removeDrawFilter(const fgl::String& name);
		
	private:
		struct DrawableData
		{
			Drawable* drawable;
			float zLayer;
		};
		
		fgl::ArrayList<DrawableData> drawables;
		
		struct DrawFilter
		{
			fgl::String name;
			std::function<void(Drawable*, fgl::Graphics&)> filterFunc;
		};
		
		fgl::ArrayList<DrawFilter> drawFilters;
	};
}

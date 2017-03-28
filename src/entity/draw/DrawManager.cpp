
#include <fightlib/entity/draw/DrawManager.hpp>

namespace fl
{
	void DrawManager::addDrawable(Drawable* drawable)
	{
		drawables.add(drawable);
	}
	
	void DrawManager::removeDrawable(Drawable* drawable)
	{
		size_t drawableIndex = drawables.indexOf(drawable);
		if(drawableIndex!=-1)
		{
			drawables.remove(drawableIndex);
		}
	}
	
	void DrawManager::update(const fgl::ApplicationData& appData)
	{
		for(auto drawable : drawables)
		{
			drawable->update(appData);
		}
	}
	
	void DrawManager::draw(const fgl::ApplicationData& appData, fgl::Graphics graphics)
	{
		for(auto drawable : drawables)
		{
			drawable->draw(appData, graphics);
		}
	}
}

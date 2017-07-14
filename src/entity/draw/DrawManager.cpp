
#include <fightlib/entity/draw/DrawManager.hpp>

namespace fl
{
	void DrawManager::addDrawable(Drawable* drawable, float zLayer)
	{
		DrawableData drawableData;
		drawableData.drawable = drawable;
		drawableData.zLayer = zLayer;

		for(size_t drawables_size=drawables.size(), i=0; i<drawables_size; i++)
		{
			auto& cmp = drawables[i];
			if(zLayer < cmp.zLayer)
			{
				drawables.add(i, drawableData);
				return;
			}
		}
		
		drawables.add(drawableData);
	}
	
	void DrawManager::removeDrawable(Drawable* drawable)
	{
		size_t index = drawables.indexWhere([&](const DrawableData& cmp)->bool {
			if(cmp.drawable==drawable)
			{
				return true;
			}
			return false;
		});
		if(index!=-1)
		{
			drawables.remove(index);
		}
	}

	float DrawManager::getDrawableZLayer(Drawable* drawable) const
	{
		for(auto& drawableData : drawables)
		{
			if(drawableData.drawable == drawable)
			{
				return drawableData.zLayer;
			}
		}
		throw fgl::IllegalArgumentException("drawable", "does not exist in this DrawManager");
	}

	void DrawManager::setDrawableZLayer(Drawable* drawable, float zLayer)
	{
		size_t index = drawables.indexWhere([&](const DrawableData& cmp)->bool {
			if(cmp.drawable==drawable)
			{
				return true;
			}
			return false;
		});
		if(index==-1)
		{
			throw fgl::IllegalArgumentException("drawable", "does not exist in this DrawManager");
		}
		drawables.remove(index);
		addDrawable(drawable, zLayer);
	}

	fgl::ArrayList<Drawable*> DrawManager::getDrawables() const
	{
		fgl::ArrayList<Drawable*> drawablePtrs;
		drawablePtrs.reserve(drawables.size());
		for(auto& drawableData : drawables)
		{
			drawablePtrs.add(drawableData.drawable);
		}
		return drawablePtrs;
	}
	
	void DrawManager::update(const fgl::ApplicationData& appData)
	{
		for(auto& drawableData : drawables)
		{
			drawableData.drawable->update(appData);
		}
	}
	
	void DrawManager::draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const
	{
		for(auto& drawableData : drawables)
		{
			if(drawFilters.size()==0)
			{
				drawableData.drawable->draw(appData, graphics);
			}
			else
			{
				auto drawableGraphics = graphics;
				for(auto& drawFilter : drawFilters)
				{
					drawFilter.filterFunc(drawableData.drawable, drawableData.zLayer, drawableGraphics);
				}
				drawableData.drawable->draw(appData, drawableGraphics);
			}
		}
	}
	
	void DrawManager::addDrawFilter(const fgl::String& name, const std::function<void(Drawable*, float, fgl::Graphics&)>& filterFunc)
	{
		if(!filterFunc)
		{
			throw fgl::IllegalArgumentException("filterFunc", "cannot be an empty function");
		}
		for(auto& drawFilter : drawFilters)
		{
			if(drawFilter.name==name)
			{
				throw fgl::IllegalArgumentException("name", "a filter already exist with this name");
			}
		}
		
		DrawFilter drawFilter;
		drawFilter.name = name;
		drawFilter.filterFunc = filterFunc;
		drawFilters.add(drawFilter);
	}
	
	void DrawManager::removeDrawFilter(const fgl::String& name)
	{
		for(size_t i=0; i<drawFilters.size(); i++)
		{
			auto& drawFilter = drawFilters[i];
			if(drawFilter.name==name)
			{
				drawFilters.remove(i);
			}
		}
	}
}

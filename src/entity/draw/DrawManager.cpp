
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
			drawableData.drawable->draw(appData, graphics);
		}
	}
}

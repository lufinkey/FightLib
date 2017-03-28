
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/Platform.hpp>

namespace fl
{
	class Stage
	{
	public:
		Stage();
		virtual ~Stage();
		
		void update(const fgl::ApplicationData& appData);
		void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const;
		
	protected:
		void addPlatform(Platform* platform);
		void removePlatform(Platform* platform);
		
	private:
		CollisionManager collisionManager;
		DrawManager drawManager;
	};
}

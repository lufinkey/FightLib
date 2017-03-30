
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/Platform.hpp>

namespace fl
{
	class Stage
	{
	public:
		Stage();
		virtual ~Stage();
		
		void update(fgl::ApplicationData appData);
		void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;
		
	protected:
		void addPlatform(Platform* platform);
		void removePlatform(Platform* platform);
		
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);
		
	private:
		CollisionManager collisionManager;
		DrawManager drawManager;
	};
}

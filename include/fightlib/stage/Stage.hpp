
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
		
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;
		
		virtual double getGravity(Entity* entity) const;
		
	protected:
		void addPlatform(Platform* platform, double zLayer=0.5);
		void removePlatform(Platform* platform);
		
		void addEntity(Entity* entity, double zLayer=0.5);
		void removeEntity(Entity* entity);
		
	private:
		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<Platform*> platforms;
		
		CollisionManager collisionManager;
		DrawManager drawManager;
	};
}

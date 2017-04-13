
#pragma once

#include <fightlib/entity/Entity.hpp>

namespace fl
{
	class HitboxCollisionManager
	{
	public:
		HitboxCollisionManager();
		
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);
		
		void update(const fgl::ApplicationData& appData);
		
	private:
		fgl::ArrayList<Entity*> entities;
	};
}
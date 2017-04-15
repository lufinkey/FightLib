
#pragma once

#include <fightlib/entity/Entity.hpp>
#include "HitboxClashPair.hpp"
#include "HitboxCollisionPair.hpp"

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
		
		fgl::ArrayList<HitboxClashPair> previousClashPairs;
		fgl::ArrayList<HitboxCollisionPair> previousCollisionPairs;
		
		struct EntityPair
		{
			Entity* entity1;
			fgl::ArrayList<TaggedBox> hitboxes1;
			Entity* entity2;
			fgl::ArrayList<TaggedBox> hitboxes2;
		};
		fgl::ArrayList<EntityPair> getEntityPairs() const;

		void addHitboxCollisionEvents(Entity* hittingEntity, Entity* hitEntity,
			const fgl::ArrayList<HitboxCollision>& hitboxCollisions, const fgl::ArrayList<HitboxCollision>& previousHitboxCollisions,
			fgl::ArrayList<std::function<void()>>* onHitboxCollisionCalls, fgl::ArrayList<std::function<void()>>* onHitboxCollisionFinishCalls);
	};
}

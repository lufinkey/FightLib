
#pragma once

#include <fightlib/entity/collision/Collidable.hpp>
#include <fightlib/entity/collision/CollisionRectManager.hpp>

namespace fl
{
	class Entity;

	class Platform : public Collidable
	{
	public:
		Platform(const fgl::Vector2d& position);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;

		virtual fgl::Vector2d getFriction(Entity* entity, CollisionSide side) const;
		
		float getScale() const;
		void setScale(float scale);
		
		virtual bool isStaticCollisionBody() const override;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;
		
	protected:
		void setCollisionMethod(CollisionMethod collisionMethod);
		CollisionMethod getCollisionMethod() const;
		
	private:
		virtual float getDrawScale() const override;
		
		float scale;
		CollisionRectManager collisionRectManager;
	};
}

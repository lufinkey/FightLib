
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
		virtual void update(const fgl::ApplicationData& appData) override;
		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;

		virtual fgl::Vector2d getFriction(Entity* entity, CollisionSide side) const;
		
		float getScale() const;
		void setScale(float scale);
		
		virtual bool isStaticCollisionBody() const override;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;
		
		void setCollisionMethod(CollisionMethod collisionMethod);
		CollisionMethod getCollisionMethod() const;
		
	protected:
		virtual void shift(const fgl::Vector2d& offset) override;
		
	private:
		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;
		virtual float getDrawScale() const override;
		
		fgl::Vector2d position;
		float scale;
		CollisionRectManager collisionRectManager;
	};
}

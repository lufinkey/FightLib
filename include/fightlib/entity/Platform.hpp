
#pragma once

#include "collision/Collidable.hpp"
#include "collision/CollisionRectManager.hpp"

namespace fl
{
	class Platform : public Collidable
	{
	public:
		Platform(const fgl::Vector2d& position);
		
		virtual void update(const fgl::ApplicationData& appData) override;
		
		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;
		
		float getScale() const;
		void setScale(float scale);
		
		virtual bool isStaticCollisionBody() const override;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;
		
		void setCollisionMethod(CollisionMethod collisionMethod);
		CollisionMethod getCollisionMethod() const;
		
		void setVelocity(const fgl::Vector2d& velocity);
		const fgl::Vector2d& getVelocity() const;
		
	protected:
		virtual void shift(const fgl::Vector2d& offset) override;
		
	private:
		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;
		virtual float getDrawScale() const override;
		
		fgl::Vector2d position;
		fgl::Vector2d velocity;
		float scale;
		CollisionRectManager collisionRectManager;
	};
}


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
		
		virtual bool isStaticCollisionBody() const override;
		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;
		
		void setCollisionMethod(CollisionMethod collisionMethod);
		CollisionMethod getCollisionMethod() const;
		
	protected:
		virtual void shift(const fgl::Vector2d& offset) override;
		
	private:
		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;
		
		fgl::Vector2d position;
		CollisionRectManager collisionRectManager;
	};
}

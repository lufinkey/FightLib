
#pragma once

#include "collision/Collidable.hpp"
#include "collision/CollisionRectManager.hpp"

namespace fl
{
	class Stage;

	class StageObject : public Collidable
	{
		friend class Stage;
	public:
		StageObject(const fgl::Vector2d& position);

		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;

		virtual fgl::Vector2d getTerminalVelocity() const;
		virtual bool respondsToGravity() const;
		virtual bool respondsToAirResistance() const;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		Stage* getStage() const;

	protected:
        virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);

		CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(CollisionMethod method);

	private:
		Stage* stage;
		CollisionRectManager collisionRectManager;
	};
}

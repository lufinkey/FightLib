
#pragma once

#include "collision/CollidableSprite.hpp"

namespace fl
{
	typedef enum : fgl::Uint8
	{
		COLLISIONMETHOD_NONE,
		COLLISIONMETHOD_FRAME,
		COLLISIONMETHOD_PIXEL,
		COLLISIONMETHOD_BOUNDS
	} CollisionMethod;
	
	
	class Stage;

	class StageObject : public CollidableSprite
	{
		friend class Stage;
	public:
		StageObject(const fgl::Vector2d& position);
		virtual ~StageObject();

		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;

		virtual fgl::Vector2d getTerminalVelocity() const;
		virtual bool respondsToGravity() const;
		virtual bool respondsToAirResistance() const;

		virtual fgl::ArrayList<fgl::CollisionRect*> getCollisionRects() const override;
		
		Stage* getStage() const;

	protected:
        virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);
		
		virtual void onBeginCollisionUpdates() override;

		CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(CollisionMethod method);

	private:
		void updateCollisionRects();
	
		Stage* stage;
		fgl::ArrayList<fgl::CollisionRect*> collisionRects;
		CollisionMethod collisionMethod;
	};
}

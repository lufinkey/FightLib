
#pragma once

#include "../draw/Sprite.hpp"

namespace fl
{
	class CollidableSprite : public Sprite, public fgl::Collidable
	{
	public:
		CollidableSprite(const fgl::Vector2d& position);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;
		
		virtual fgl::TransformState getTransformState() const override;
		virtual void shift(const fgl::Vector2d& offset) override;
		
		fgl::Vector2d getVelocity() const;
		void setVelocity(const fgl::Vector2d& velocity);
		
		void applyForce(const fgl::Vector2d& force);
		
		static fgl::ArrayList<const fgl::CollisionRect*> createCollisionRectsFromAnimation(CollidableSprite* sprite, const fgl::ArrayList<const fgl::CollisionRect*>& prevRects);
		static fgl::ArrayList<const fgl::CollisionRect*> createCollisionRectsFromBoundsMetapoints(CollidableSprite* sprite, const fgl::ArrayList<const fgl::CollisionRect*>& prevRects);
		
	private:
		fgl::Vector2d velocity;
	};
}

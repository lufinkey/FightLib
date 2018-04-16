
#pragma once

#include "../draw/Sprite.hpp"

namespace fl
{
	class CollidableSprite : public Sprite, public fgl::Collidable
	{
	public:
		CollidableSprite(const fgl::Vector2d& position);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		
		virtual fgl::TransformState getTransformState() const override;
		virtual void shift(const fgl::Vector2d& offset) override;
		
		static fgl::ArrayList<fgl::CollisionRect*> createCollisionRectsFromAnimation(CollidableSprite* sprite, const fgl::ArrayList<fgl::CollisionRect*>& prevRects);
		static fgl::ArrayList<fgl::CollisionRect*> createCollisionRectsFromBoundsMetapoints(CollidableSprite* sprite, const fgl::ArrayList<fgl::CollisionRect*>& prevRects);
	};
}

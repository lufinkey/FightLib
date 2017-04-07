
#pragma once

#include "Entity.hpp"

namespace fl
{
	class Character;
	
	class Item : public Entity
	{
		friend class Character;
	public:
		Item(const fgl::Vector2d& position, Orientation orientation);

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		virtual fgl::ArrayList<MetaPointType> getAnchorPoints() const = 0;
		virtual bool isPickedUpByTouching() const;
		virtual bool isPowerUp() const;
		
		Character* getParentCharacter() const;

	protected:
		virtual void onCollision(Collidable* collided, CollisionSide side) override;
		virtual void onCollisionUpdate(Collidable* collided, CollisionSide side) override;
		
		virtual void onPickUp(Character* character);
		virtual void onDiscard(Character* character);
		
	private:
		Character* parentCharacter;
	};
}

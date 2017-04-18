
#pragma once

#include "ActionEntity.hpp"

namespace fl
{
	class Character;
	
	class Item : public ActionEntity
	{
		friend class Character;
	public:
		Item(const fgl::Vector2d& position, Orientation orientation);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const override;
		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		virtual fgl::ArrayList<MetaPointType> getAnchorPoints() const = 0;
		virtual bool isPickedUpByTouching() const;
		virtual bool isPowerUp() const;
		
		Character* getParentCharacter() const;

	protected:
		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;
		
		virtual bool respondsToCollision(Collidable* collided, CollisionSide side) const override;
		
		virtual void onPickUp(Character* character);
		virtual void onDiscard(Character* character);
		
	private:
		Character* parentCharacter;
	};
}

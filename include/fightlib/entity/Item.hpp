
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
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		virtual fgl::ArrayList<MetaPointType> getAnchorPoints() const = 0;
		virtual bool isPickedUpByTouching() const;

		bool isEquippable() const;
		
		Character* getParentCharacter() const;

	protected:
		virtual fgl::ArrayList<fgl::PolygonD> getCollisionPolygons() const override;
		virtual bool respondsToCollision(Collidable* collided, CollisionData data) const override;
		
		virtual void onPickUp();
		virtual void onDiscard();

		virtual void onEquip();
		virtual void onUnequip();
		
	private:
		Character* parentCharacter;
		bool beingUsed;
	};
}

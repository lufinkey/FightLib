
#pragma once

#include "ActionEntity.hpp"
#include "Item.hpp"

namespace fl
{
	class Character : public ActionEntity
	{
	public:
		Character(const fgl::Vector2d& position, Orientation orientation);
		
		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;

		virtual fgl::String getIdleAnimationName() const = 0;
		virtual fgl::String getMoveAnimationName(double amount) const = 0;
		virtual double getMoveSpeed(double amount) const;
		
		virtual fgl::ArrayList<MetaPointType> getItemAnchorPoints() const;
		fgl::ArrayList<MetaPointType> getAvailableItemAnchorPoints() const;
		
		bool pickUpItem(Item* item);
		void discardItem(Item* item);
		bool isCarryingItem(Item* item) const;
		fgl::ArrayList<Item*> getEquippedItems() const;
		fgl::ArrayList<Item*> getUnequippedItems() const;
		fgl::ArrayList<Item*> getItems() const;
		bool setItemEquipped(Item* item, bool equipped);
		bool isItemEquipped(Item* item) const;
		bool isItemEquippable(Item* item) const;

		void setDirection(const fgl::Vector2f& direction);
		const fgl::Vector2f& getDirection() const;

	protected:
		virtual void updateMovement(const fgl::ApplicationData& appData);
		virtual void updateMoveAnimation();
		
		virtual void onActionEnd(Action* action) override;

		virtual bool respondsToHitboxClash(Entity* clashedEntity) const override;
		virtual bool canCollideWithEntityHitbox(Entity* collidedEntity) const override;
		
		virtual void onFinishCollisionUpdates() override;
		
		virtual bool canPickUpItem(Item* item) const;
		virtual void onPickUpItem(Item* item);
		virtual void onDiscardItem(Item* item);
		virtual bool canEquipItem(Item* item) const;
		virtual void onEquipItem(Item* item);
		virtual void onUnequipItem(Item* item);

	private:
		struct EquipPoint
		{
			MetaPointType anchorPoint;
			size_t anchorPointIndex;
		};
		fgl::ArrayList<EquipPoint> getMatchingEquipPoints(Item* item) const;

		fgl::Vector2f direction;
		
		struct ItemContainer
		{
			Item* item;
			MetaPointType anchorPoint;
			size_t anchorPointIndex;
			bool equipped;
		};
		fgl::ArrayList<ItemContainer> itemContainers;
	};
}


#include "PickUpItemAction.hpp"

void PickUpItemAction::onPerform(fl::ActionParamsPtr params)
{
	auto character = (fl::Character*)getEntity();
	auto stage = character->getStage();
	if(stage!=nullptr)
	{
		auto availableItems = stage->getAccessibleItems(character);
		if(availableItems.size() > 0)
		{
			auto& item = availableItems[0];
			if(character->pickUpItem(item))
			{
				if(item->isEquippable())
				{
					character->setItemEquipped(item, true);
				}
			}
		}
	}
	
	end();
}

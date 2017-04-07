
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
			fgl::Console::writeLine("picking up item");
			character->pickUpItem(availableItems[0]);
		}
		else
		{
			fgl::Console::writeErrorLine("no items available");
		}
	}
	
	end();
}

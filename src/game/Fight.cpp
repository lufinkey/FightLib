
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Fight::Fight(const FightParams& params)
		: stage(params.getStage()),
		characterControllers(params.getCharacterControllers())
	{
		if(stage==nullptr)
		{
			throw fgl::IllegalArgumentException("params", "stage cannot be null");
		}
		else if(stage->fight != nullptr)
		{
			throw fgl::IllegalArgumentException("params", "stage cannot be added to multiple Fight objects");
		}
		
		stage->fight = this;

		for(auto character : params.getCharacters())
		{
			stage->addCharacter(character);
		}
	}

	Fight::~Fight()
	{
		for(auto characterController : characterControllers)
		{
			delete characterController;
		}
		
		stage->fight = nullptr;
		delete stage;
	}

	void Fight::update(fgl::ApplicationData appData)
	{
		appData.additionalData["fight"] = this;
		
		//automatically pick up items that can be picked up by touching them
		for(auto character : stage->getCharacters())
		{
			for(auto item : stage->getAccessibleItems(character))
			{
				if(item->isPickedUpByTouching())
				{
					character->pickUpItem(item);
				}
			}
		}
		
		//update controllers
		for(auto controller : characterControllers)
		{
			controller->update(appData);
		}
		
		//update stage
		stage->update(appData);
	}

	void Fight::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		appData.additionalData["fight"] = this;
		stage->draw(appData, graphics);
	}
	
	Stage* Fight::getStage() const
	{
		return stage;
	}
	
	const fgl::ArrayList<CharacterController*>& Fight::getCharacterControllers() const
	{
		return characterControllers;
	}
}

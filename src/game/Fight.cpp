
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Fight::Fight(const FightParams& params)
		: characterControllers(params.getCharacterControllers()),
		stage(params.getStage()),
		characters(params.getCharacters())
	{
		if(stage==nullptr)
		{
			throw fgl::IllegalArgumentException("params", "stage cannot be null");
		}

		for(auto character : characters)
		{
			stage->addEntity(character);
		}
	}

	Fight::~Fight()
	{
		for(auto characterController : characterControllers)
		{
			delete characterController;
		}

		delete stage;
	}

	void Fight::update(fgl::ApplicationData appData)
	{
		appData.additionalData["fight"] = this;
		
		for(auto controller : characterControllers)
		{
			controller->update(appData);
		}
		stage->update(appData);
	}

	void Fight::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		stage->draw(appData, graphics);
	}
	
	Stage* Fight::getStage() const
	{
		return stage;
	}
	
	const fgl::ArrayList<Character*>& Fight::getCharacters() const
	{
		return characters;
	}
	
	const fgl::ArrayList<CharacterController*>& Fight::getCharacterControllers() const
	{
		return characterControllers;
	}
}

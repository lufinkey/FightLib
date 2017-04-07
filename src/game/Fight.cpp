
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
	
	const fgl::ArrayList<CharacterController*>& Fight::getCharacterControllers() const
	{
		return characterControllers;
	}
}

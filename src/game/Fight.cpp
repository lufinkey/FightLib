
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Fight::Fight(const FightParams& params)
		: stage(params.getStage()),
		characters(params.getCharacters()),
		characterControllers(params.getCharacterControllers())
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
}

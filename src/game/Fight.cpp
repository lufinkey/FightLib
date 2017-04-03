
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
	}

	void Fight::update(fgl::ApplicationData appData)
	{
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

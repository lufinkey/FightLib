
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Fight::Fight(const FightParams& params)
		: stage(params.getStage()),
		camera(params.getCamera()),
		characterControllers(params.getCharacterControllers()),
		firstUpdate(true)
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

		if(camera==nullptr)
		{
			camera = new Camera();
		}
		camera->fight = this;
	}

	Fight::~Fight()
	{
		for(auto characterController : characterControllers)
		{
			delete characterController;
		}
		
		stage->fight = nullptr;
		delete stage;
		camera->fight = nullptr;
		delete camera;
	}

	void Fight::update(fgl::ApplicationData appData)
	{
		if(firstUpdate)
		{
			firstUpdate = false;
			camera->onBeginFight(this);
		}

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

		//update camera
		camera->update(appData);
	}

	void Fight::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		appData.additionalData["fight"] = this;
		camera->drawStage(appData, graphics);
	}
	
	Stage* Fight::getStage() const
	{
		return stage;
	}

	Camera* Fight::getCamera() const
	{
		return camera;
	}
	
	const fgl::ArrayList<CharacterController*>& Fight::getCharacterControllers() const
	{
		return characterControllers;
	}
}

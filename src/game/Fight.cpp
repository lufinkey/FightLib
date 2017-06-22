
#include <fightlib/game/Fight.hpp>

namespace fl
{
	Fight::Fight(const FightParams& params)
		: frame(params.getFrame()),
		stage(params.getStage()),
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
			time.start();
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
		graphics.translate(frame.x, frame.y);
		graphics.clip(fgl::RectangleD(0, 0, frame.width, frame.height));
		graphics.drawRect(0, 0, frame.width, frame.height);
		camera->drawStage(appData, graphics);
	}

	void Fight::setFrame(const fgl::RectangleD& frame_arg)
	{
		auto oldFrame = frame;
		frame = frame_arg;
		if(!firstUpdate)
		{
			if(frame.width!=oldFrame.width || frame.height!=oldFrame.height)
			{
				camera->onFrameSizeChange(fgl::Vector2d(oldFrame.width, oldFrame.height), fgl::Vector2d(frame.width, frame.height));
			}
		}
	}

	const fgl::RectangleD& Fight::getFrame() const
	{
		return frame;
	}

	fgl::TimeInterval Fight::getTime() const
	{
		return time;
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

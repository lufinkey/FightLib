
#include <fightlib/game/FightParams.hpp>

namespace fl
{
	FightParams::FightParams()
		: frame(0,0,640,480),
		stage(nullptr),
		camera(nullptr)
	{
		//
	}

	void FightParams::setFrame(const fgl::RectangleD& frame_arg)
	{
		frame = frame_arg;
	}

	const fgl::RectangleD& FightParams::getFrame() const
	{
		return frame;
	}
	
	void FightParams::setStage(Stage* stage_arg)
	{
		stage = stage_arg;
	}
	
	Stage* FightParams::getStage() const
	{
		return stage;
	}

	void FightParams::setCamera(Camera* camera_arg)
	{
		camera = camera_arg;
	}

	Camera* FightParams::getCamera() const
	{
		return camera;
	}
	
	void FightParams::addCharacter(Character* character)
	{
		characters.add(character);
	}
	
	void FightParams::setCharacters(const fgl::ArrayList<Character*>& characters_arg)
	{
		characters = characters_arg;
	}
	
	const fgl::ArrayList<Character*>& FightParams::getCharacters() const
	{
		return characters;
	}
	
	void FightParams::addCharacterController(CharacterController* controller)
	{
		characterControllers.add(controller);
	}
	
	void FightParams::setCharacterControllers(const fgl::ArrayList<CharacterController*>& controllers)
	{
		characterControllers = controllers;
	}
	
	const fgl::ArrayList<CharacterController*>& FightParams::getCharacterControllers() const
	{
		return characterControllers;
	}
}

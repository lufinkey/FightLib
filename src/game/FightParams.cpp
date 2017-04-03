
#include <fightlib/game/FightParams.hpp>

namespace fl
{
	FightParams::FightParams()
		: stage(nullptr),
		memoryCleanupEnabled(false)
	{
		//
	}
	
	void FightParams::setMemoryCleanupEnabled(bool enabled)
	{
		memoryCleanupEnabled = enabled;
	}
	
	bool FightParams::isMemoryCleanupEnabled() const
	{
		return memoryCleanupEnabled;
	}
	
	void FightParams::setStage(Stage* stage_arg)
	{
		stage = stage_arg;
	}
	
	Stage* FightParams::getStage() const
	{
		return stage;
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

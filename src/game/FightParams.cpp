
#include <fightlib/game/FightParams.hpp>

namespace fl
{
	FightParams::FightParams()
		: stage(nullptr),
		postFightCleanupEnabled(false)
	{
		//
	}
	
	void FightParams::setPostFightCleanupEnabled(bool enabled)
	{
		postFightCleanupEnabled = enabled;
	}
	
	bool FightParams::isPostFightCleanupEnabled() const
	{
		return postFightCleanupEnabled;
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
	
	void FightParams::addCharacter(fl::Character* character, const fgl::Vector2d& spawnPosition)
	{
		characters.add(character);
		characterSpawnPositions[character] = spawnPosition;
	}
	
	void FightParams::setCharacters(const fgl::ArrayList<Character*>& characters_arg, const fgl::ArrayList<fgl::Vector2d>& spawnPositions)
	{
		characters = characters_arg;
		characterSpawnPositions.clear();
		for(size_t i=0; i<spawnPositions.size() && i<characters.size(); i++)
		{
			characterSpawnPositions[characters[i]] = spawnPositions[i];
		}
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

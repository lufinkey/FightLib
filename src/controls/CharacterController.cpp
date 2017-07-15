
#include <fightlib/controls/CharacterController.hpp>

namespace fl
{
	CharacterController::CharacterController(Character* character)
		: character(character)
	{
		//
	}
	
	CharacterController::~CharacterController()
	{
		//
	}
	
	bool CharacterController::getFlag(const fgl::String& flag) const
	{
		return false;
	}

	Character* CharacterController::getCharacter() const
	{
		return character;
	}
}

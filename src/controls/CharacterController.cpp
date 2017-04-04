
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

	Character* CharacterController::getCharacter() const
	{
		return character;
	}
}

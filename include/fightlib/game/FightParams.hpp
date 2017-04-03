
#pragma once

#include <fightlib/controls/CharacterController.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/stage/Stage.hpp>

namespace fl
{
	class FightParams
	{
	public:
		FightParams();
		
		void setStage(Stage* stage);
		Stage* getStage() const;

		void addCharacter(Character* character);
		void setCharacters(const fgl::ArrayList<Character*>& characters);
		const fgl::ArrayList<Character*>& getCharacters() const;
		
		void addCharacterController(CharacterController* controller);
		void setCharacterControllers(const fgl::ArrayList<CharacterController*>& controllers);
		const fgl::ArrayList<CharacterController*>& getCharacterControllers() const;

	private:
		Stage* stage;
		fgl::ArrayList<Character*> characters;
		fgl::ArrayList<CharacterController*> characterControllers;
	};
}

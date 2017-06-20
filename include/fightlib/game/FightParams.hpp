
#pragma once

#include <fightlib/controls/CharacterController.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/stage/Stage.hpp>
#include "camera/Camera.hpp"

namespace fl
{
	class FightParams
	{
	public:
		FightParams();

		void setFrame(const fgl::RectangleD& frame);
		const fgl::RectangleD& getFrame() const;
		
		void setStage(Stage* stage);
		Stage* getStage() const;

		void setCamera(Camera* camera);
		Camera* getCamera() const;

		void addCharacter(Character* character);
		void setCharacters(const fgl::ArrayList<Character*>& characters);
		const fgl::ArrayList<Character*>& getCharacters() const;
		
		void addCharacterController(CharacterController* controller);
		void setCharacterControllers(const fgl::ArrayList<CharacterController*>& controllers);
		const fgl::ArrayList<CharacterController*>& getCharacterControllers() const;

	private:
		fgl::RectangleD frame;
		Stage* stage;
		Camera* camera;
		fgl::ArrayList<Character*> characters;
		fgl::ArrayList<CharacterController*> characterControllers;
	};
}

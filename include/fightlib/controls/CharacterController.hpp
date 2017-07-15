
#pragma once

#include <fightlib/entity/Character.hpp>

namespace fl
{
	class CharacterController
	{
	public:
		CharacterController(Character* character);
		virtual ~CharacterController();

		virtual bool getFlag(const fgl::String& flag) const;
		virtual void update(fgl::ApplicationData appData) = 0;

		Character* getCharacter() const;

	private:
		Character* character;
	};
}

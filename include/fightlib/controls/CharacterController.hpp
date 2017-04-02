
#pragma once

#include <fightlib/entity/Character.hpp>

namespace fl
{
	class CharacterController
	{
	public:
		CharacterController(Character* character);

		virtual void update(const fgl::ApplicationData& appData) = 0;

		Character* getCharacter() const;

	private:
		Character* character;
	};
}

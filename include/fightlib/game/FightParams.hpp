
#pragma once

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

		void setCharacters(const fgl::ArrayList<Character*>& characters, const fgl::ArrayList<fgl::Vector2d>& spawnPositions={});
		const fgl::ArrayList<Character*>& getCharacters() const;

	private:
		fgl::ArrayList<Character*> characters;
		fgl::ArrayList<fgl::Vector2d> spawnPositions;
	};
}

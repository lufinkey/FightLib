
#pragma once

#include "FightParams.hpp"

namespace fl
{
	class Fight
	{
	public:
		Fight(const FightParams& params);
		virtual ~Fight();
		
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;
		
		Stage* getStage() const;
		const fgl::ArrayList<Character*>& getCharacters() const;
		const fgl::ArrayList<CharacterController*>& getCharacterControllers() const;

	private:
		fgl::RectangleD frame;
		
		fgl::ArrayList<CharacterController*> characterControllers;
		
		Stage* stage;
		fgl::ArrayList<Character*> characters;
		fgl::ArrayList<Item*> items;
	};
}

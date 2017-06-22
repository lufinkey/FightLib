
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

		void setFrame(const fgl::RectangleD& frame);
		const fgl::RectangleD& getFrame() const;

		fgl::TimeInterval getTime() const;
		
		Stage* getStage() const;
		Camera* getCamera() const;
		const fgl::ArrayList<CharacterController*>& getCharacterControllers() const;

	private:
		fgl::RectangleD frame;

		fgl::TimeInterval time;
		
		Stage* stage;
		Camera* camera;
		fgl::ArrayList<CharacterController*> characterControllers;

		bool firstUpdate;
	};
}

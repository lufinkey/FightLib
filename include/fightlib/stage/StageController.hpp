
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Stage;

	class StageController
	{
		friend class Stage;
	public:
		StageController();
		virtual ~StageController();

		virtual void update(fgl::ApplicationData appData);

		Stage* getStage() const;

	protected:
		virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);

	private:
		Stage* stage;
	};
}


#include <fightlib/stage/StageController.hpp>

namespace fl
{
	StageController::StageController()
		: stage(nullptr)
	{
		//
	}

	StageController::~StageController()
	{
		//
	}

	void StageController::update(fgl::ApplicationData appData)
	{
		//
	}

	Stage* StageController::getStage() const
	{
		return stage;
	}

	void StageController::onAddToStage(Stage* stage)
	{
		//
	}

	void StageController::onRemoveFromStage(Stage* stage)
	{
		//
	}
}


#include <fightlib/stage/StageSection.hpp>

namespace fl
{
	StageSection::StageSection()
		: stage(nullptr),
		loaded(false)
	{
		//
	}

	fl::Stage* StageSection::getStage() const
	{
		return stage;
	}

	bool StageSection::isLoaded() const
	{
		return loaded;
	}

	void StageSection::update(const fgl::ApplicationData& appData)
	{
		//
	}
}

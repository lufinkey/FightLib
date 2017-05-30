
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	class Stage;

	class StageSection
	{
		friend class Stage;
	public:
		StageSection();
		virtual ~StageSection() = default;

		fl::Stage* getStage() const;
		bool isLoaded() const;

		virtual fgl::RectangleD getRect() const = 0;

	protected:
		virtual void onLoad(const fgl::Dictionary& state) = 0;
		virtual fgl::Dictionary onUnload() = 0;

		virtual void update(const fgl::ApplicationData& appData);

	private:
		fl::Stage* stage;
		bool loaded;
	};
}


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
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		Stage* getStage() const;
		
		void easeValue(const fgl::String& name, long long duration, const std::function<void(double progress)>& onprogress);
		void easeValue(long long duration, const std::function<void(double progress)>& onprogress);
		double stopEasedValue(const fgl::String& name, bool finishEase=false);

		void createTimer(const fgl::String& name, long long duration, const std::function<void()>& oncompletion);
		void createTimer(long long duration, const std::function<void()>& oncompletion);
		void destroyTimer(const fgl::String& name, bool callTimer=false);

	protected:
		virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);

		fgl::TimeInterval getTime() const;

	private:
		Stage* stage;

		struct EasedValue
		{
			size_t id;
			fgl::String name;
			long long startTime;
			long long duration;
			std::function<void(double)> onprogress;
		};
		size_t EasedValue_nextID;
		size_t EasedValue_getNextID();
		void stopEasedValue(size_t id);
		fgl::ArrayList<EasedValue> easedValues;

		struct Timer
		{
			size_t id;
			fgl::String name;
			long long fireTime;
			std::function<void()> oncompletion;
		};
		size_t Timer_nextID;
		size_t Timer_getNextID();
		void destroyTimer(size_t id);
		fgl::ArrayList<Timer> timers;
	};
}

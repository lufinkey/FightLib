
#include <fightlib/stage/StageController.hpp>
#include <fightlib/stage/Stage.hpp>
#include <fightlib/game/Fight.hpp>

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
		long long currentTimeMillis = appData.getTime().getMilliseconds();

		//update eased values
		for(size_t i=0; i<easedValues.size(); i++)
		{
			auto value = easedValues[i];
			long long elapsed = currentTimeMillis - value.startTime;
			if(elapsed >= value.duration)
			{
				//ease is finished. remove it.
				easedValues.remove(i);
				i--;
				//call the final callback
				if(value.onprogress)
				{
					value.onprogress(1.0);
				}
			}
			else
			{
				//progress the value callback
				if(value.onprogress)
				{
					value.onprogress((double)((long double)elapsed/(long double)value.duration));
				}
			}
		}

		//update timers
		for(size_t i=0; i<timers.size(); i++)
		{
			auto timer = timers[i];
			if(currentTimeMillis >= timer.fireTime)
			{
				//timer is finished. remove it
				timers.remove(i);
				i--;
				//call the timer callback
				if(timer.oncompletion)
				{
					timer.oncompletion();
				}
			}
		}
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

	fgl::TimeInterval StageController::getTime() const
	{
		if(auto stage = getStage())
		{
			if(auto fight = stage->getFight())
			{
				return fight->getTime();
			}
		}
		return fgl::TimeInterval(0);
	}

	void StageController::easeValue(const fgl::String& name, long long duration, const std::function<void(double progress)>& onprogress)
	{
		//make sure another eased value with the same name doesn't already exist
		if(name.length()>0)
		{
			size_t index = easedValues.indexWhere([&](const EasedValue& value) -> bool {
				if(value.name==name)
				{
					return true;
				}
				return false;
			});
			if(index != -1)
			{
				throw fgl::IllegalArgumentException("name", "eased value already exists with the name \""+name+"\"");
			}
		}

		//add the eased value
		EasedValue value;
		value.name = name;
		value.startTime = getTime().getMilliseconds();
		value.duration = duration;
		value.onprogress = onprogress;
		easedValues.add(value);

		//set initial state for eased value
		if(onprogress)
		{
			onprogress(0.0);
		}
	}

	void StageController::easeValue(long long duration, const std::function<void(double progress)>& onprogress)
	{
		easeValue("", duration, onprogress);
	}

	void StageController::stopEasedValue(const fgl::String& name)
	{
		//eased values with no name can't get removed
		if(name.length()==0)
		{
			return;
		}

		easedValues.removeFirstWhere([&](const EasedValue& value) -> bool {
			if(value.name==name)
			{
				return true;
			}
			return false;
		});
	}

	void StageController::createTimer(const fgl::String& name, long long duration, const std::function<void()>& oncompletion)
	{
		//make sure another time with the same name doesn't already exist
		if(name.length()>0)
		{
			size_t index = timers.indexWhere([&](const Timer& timer) -> bool {
				if(timer.name==name)
				{
					return true;
				}
				return false;
			});
			if(index != -1)
			{
				throw fgl::IllegalArgumentException("name", "timer already exists with the name \""+name+"\"");
			}
		}

		//add the timer
		Timer timer;
		timer.name = name;
		timer.fireTime = getTime().getMilliseconds()+duration;
		timer.oncompletion = oncompletion;
		timers.add(timer);
	}

	void StageController::createTimer(long long duration, const std::function<void()>& oncompletion)
	{
		createTimer("", duration, oncompletion);
	}

	void StageController::destroyTimer(const fgl::String& name)
	{
		//timers with no name can't get removed
		if(name.length()==0)
		{
			return;
		}

		timers.removeFirstWhere([&](const Timer& timer) -> bool {
			if(timer.name==name)
			{
				return true;
			}
			return false;
		});
	}
}

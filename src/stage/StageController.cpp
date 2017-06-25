
#include <fightlib/stage/StageController.hpp>
#include <fightlib/stage/Stage.hpp>
#include <fightlib/game/Fight.hpp>

namespace fl
{
	StageController::StageController()
		: stage(nullptr),
		EasedValue_nextID(0),
		Timer_nextID(0)
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
		auto easedValues_tmp = easedValues;
		for(auto& value : easedValues_tmp)
		{
			long long elapsed = currentTimeMillis - value.startTime;
			if(elapsed >= value.duration)
			{
				//ease is finished. remove it.
				stopEasedValue(value.id);
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
		auto timers_tmp = timers;
		for(auto& timer : timers_tmp)
		{
			if(currentTimeMillis >= timer.fireTime)
			{
				//timer is finished. remove it
				destroyTimer(timer.id);
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

	size_t StageController::EasedValue_getNextID()
	{
		auto nextID = EasedValue_nextID;

		//increment EasedValue_nextID and make sure there's no duplicate ID already in existance
		bool foundDuplicate = false;
		do
		{
			EasedValue_nextID++;
			foundDuplicate = false;
			for(auto& value : easedValues)
			{
				if(value.id == EasedValue_nextID)
				{
					foundDuplicate = true;
					break;
				}
			}
		} while(foundDuplicate);

		return nextID;
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

		if(duration<=0)
		{
			if(onprogress)
			{
				onprogress(1.0);
			}
			return;
		}

		//add the eased value
		EasedValue value;
		value.id = EasedValue_getNextID();
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

	double StageController::stopEasedValue(const fgl::String& name, bool finishEase)
	{
		//eased values with no name can't get removed
		if(name.length()==0)
		{
			return 0;
		}

		std::function<void(double)> easeFunc;
		double progress = 0;
		easedValues.removeFirstWhere([&](const EasedValue& value) -> bool {
			if(value.name==name)
			{
				auto time = getTime().getMilliseconds();
				auto elapsed = time - value.startTime;
				if(elapsed < value.duration)
				{
					progress = (double)((long double)elapsed/(long double)value.duration);
				}
				if(finishEase)
				{
					easeFunc = value.onprogress;
				}
				return true;
			}
			return false;
		});
		if(easeFunc)
		{
			easeFunc(1.0);
		}
		return progress;
	}

	void StageController::stopEasedValue(size_t id)
	{
		easedValues.removeFirstWhere([&](const EasedValue& value) -> bool {
			if(value.id==id)
			{
				return true;
			}
			return false;
		});
	}

	size_t StageController::Timer_getNextID()
	{
		auto nextID = Timer_nextID;

		//increment Timer_nextID and make sure there's no duplicate ID already in existance
		bool foundDuplicate = false;
		do
		{
			Timer_nextID++;
			foundDuplicate = false;
			for(auto& timer : timers)
			{
				if(timer.id == Timer_nextID)
				{
					foundDuplicate = true;
					break;
				}
			}
		} while(foundDuplicate);

		return nextID;
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

		if(duration<=0)
		{
			if(oncompletion)
			{
				oncompletion();
			}
			return;
		}

		//add the timer
		Timer timer;
		timer.id = Timer_getNextID();
		timer.name = name;
		timer.fireTime = getTime().getMilliseconds()+duration;
		timer.oncompletion = oncompletion;
		timers.add(timer);
	}

	void StageController::createTimer(long long duration, const std::function<void()>& oncompletion)
	{
		createTimer("", duration, oncompletion);
	}

	void StageController::destroyTimer(const fgl::String& name, bool callTimer)
	{
		//timers with no name can't get removed
		if(name.length()==0)
		{
			return;
		}

		std::function<void()> timerFunc;
		timers.removeFirstWhere([&](const Timer& timer) -> bool {
			if(timer.name==name)
			{
				if(callTimer)
				{
					timerFunc = timer.oncompletion;
				}
				return true;
			}
			return false;
		});
		if(timerFunc)
		{
			timerFunc();
		}
	}

	void StageController::destroyTimer(size_t id)
	{
		timers.removeFirstWhere([&](const Timer& timer) -> bool {
			if(timer.id==id)
			{
				return true;
			}
			return false;
		});
	}
}

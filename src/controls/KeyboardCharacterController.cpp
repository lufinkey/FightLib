
#include <fightlib/controls/KeyboardCharacterController.hpp>

namespace fl
{
	KeyboardCharacterController::KeyboardCharacterController(Character* character)
		: CharacterController(character),
		upKey(fgl::Keyboard::UPARROW),
		downKey(fgl::Keyboard::DOWNARROW),
		leftKey(fgl::Keyboard::LEFTARROW),
		rightKey(fgl::Keyboard::RIGHTARROW)
	{
		//
	}
	
	void KeyboardCharacterController::update(fgl::ApplicationData appData)
	{
		auto character = getCharacter();
		
		fgl::Vector2f direction;
		if(fgl::Keyboard::isKeyPressed(upKey))
		{
			direction.y -= 1.0f;
		}
		if(fgl::Keyboard::isKeyPressed(downKey))
		{
			direction.y += 1.0f;
		}
		if(fgl::Keyboard::isKeyPressed(leftKey))
		{
			direction.x -= 1.0f;
		}
		if(fgl::Keyboard::isKeyPressed(rightKey))
		{
			direction.x += 1.0f;
		}
		
		character->setDirection(direction);
		
		for(auto& keyActionPair : keyUpActions)
		{
			if(!fgl::Keyboard::isKeyPressed(keyActionPair.first) && fgl::Keyboard::wasKeyPressed(keyActionPair.first))
			{
				character->performAction(keyActionPair.second.name, keyActionPair.second.params);
			}
		}
		
		for(auto& keyActionPair : keyDownActions)
		{
			if(fgl::Keyboard::isKeyPressed(keyActionPair.first) && !fgl::Keyboard::wasKeyPressed(keyActionPair.first))
			{
				character->performAction(keyActionPair.second.name, keyActionPair.second.params);
			}
		}
	}
	
	void KeyboardCharacterController::setUpKey(fgl::Keyboard::Key key)
	{
		upKey = key;
	}
	
	fgl::Keyboard::Key KeyboardCharacterController::getUpKey() const
	{
		return upKey;
	}
	
	void KeyboardCharacterController::setDownKey(fgl::Keyboard::Key key)
	{
		downKey = key;
	}
	
	fgl::Keyboard::Key KeyboardCharacterController::getDownKey() const
	{
		return downKey;
	}
	
	void KeyboardCharacterController::setLeftKey(fgl::Keyboard::Key key)
	{
		leftKey = key;
	}
	
	fgl::Keyboard::Key KeyboardCharacterController::getLeftKey() const
	{
		return leftKey;
	}
	
	void KeyboardCharacterController::setRightKey(fgl::Keyboard::Key key)
	{
		rightKey = key;
	}
	
	fgl::Keyboard::Key KeyboardCharacterController::getRightKey() const
	{
		return rightKey;
	}
	
	void KeyboardCharacterController::setKeyDownAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params)
	{
		KeyAction action;
		action.name = actionName;
		action.params = params;
		keyDownActions[key] = action;
	}
	
	fgl::String KeyboardCharacterController::getKeyDownActionName(fgl::Keyboard::Key key) const
	{
		try
		{
			return keyDownActions.get(key).name;
		}
		catch(const fgl::DictionaryKeyNotFoundException&)
		{
			return "";
		}
	}
	
	ActionParamsPtr KeyboardCharacterController::getKeyDownActionParams(fgl::Keyboard::Key key) const
	{
		try
		{
			return keyDownActions.get(key).params;
		}
		catch (const fgl::DictionaryKeyNotFoundException&)
		{
			return ActionParamsPtr(nullptr);
		}
	}
	
	void KeyboardCharacterController::removeKeyDownAction(fgl::Keyboard::Key key)
	{
		keyDownActions.remove(key);
	}
	
	void KeyboardCharacterController::setKeyUpAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params)
	{
		KeyAction action;
		action.name = actionName;
		action.params = params;
		keyUpActions[key] = action;
	}
	
	fgl::String KeyboardCharacterController::getKeyUpActionName(fgl::Keyboard::Key key) const
	{
		try
		{
			return keyUpActions.get(key).name;
		}
		catch(const fgl::DictionaryKeyNotFoundException&)
		{
			return "";
		}
	}
	
	ActionParamsPtr KeyboardCharacterController::getKeyUpActionParams(fgl::Keyboard::Key key) const
	{
		try
		{
			return keyUpActions.get(key).params;
		}
		catch (const fgl::DictionaryKeyNotFoundException&)
		{
			return ActionParamsPtr(nullptr);
		}
	}
	
	void KeyboardCharacterController::removeKeyUpAction(fgl::Keyboard::Key key)
	{
		keyUpActions.remove(key);
	}
}

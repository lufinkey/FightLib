
#include <fightlib/controls/CharacterKeyboardController.hpp>

namespace fl
{
	CharacterKeyboardController::CharacterKeyboardController(Character* character)
		: CharacterController(character),
		upKey(fgl::Keyboard::UPARROW),
		downKey(fgl::Keyboard::DOWNARROW),
		leftKey(fgl::Keyboard::LEFTARROW),
		rightKey(fgl::Keyboard::RIGHTARROW)
	{
		//
	}
	
	void CharacterKeyboardController::update(fgl::ApplicationData appData)
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
	}
	
	void CharacterKeyboardController::setUpKey(fgl::Keyboard::Key key)
	{
		upKey = key;
	}
	
	fgl::Keyboard::Key CharacterKeyboardController::getUpKey() const
	{
		return upKey;
	}
	
	void CharacterKeyboardController::setDownKey(fgl::Keyboard::Key key)
	{
		downKey = key;
	}
	
	fgl::Keyboard::Key CharacterKeyboardController::getDownKey() const
	{
		return downKey;
	}
	
	void CharacterKeyboardController::setLeftKey(fgl::Keyboard::Key key)
	{
		leftKey = key;
	}
	
	fgl::Keyboard::Key CharacterKeyboardController::getLeftKey() const
	{
		return leftKey;
	}
	
	void CharacterKeyboardController::setRightKey(fgl::Keyboard::Key key)
	{
		rightKey = key;
	}
	
	fgl::Keyboard::Key CharacterKeyboardController::getRightKey() const
	{
		return rightKey;
	}
	
	void CharacterKeyboardController::setKeyDownAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params)
	{
		KeyAction action;
		action.name = actionName;
		action.params = params;
		keyDownActions[key] = action;
	}
	
	fgl::String CharacterKeyboardController::getKeyDownActionName(fgl::Keyboard::Key key) const
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
	
	ActionParamsPtr CharacterKeyboardController::getKeyDownActionParams(fgl::Keyboard::Key key) const
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
	
	void CharacterKeyboardController::removeKeyDownAction(fgl::Keyboard::Key key)
	{
		keyDownActions.remove(key);
	}
	
	void CharacterKeyboardController::setKeyUpAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params)
	{
		KeyAction action;
		action.name = actionName;
		action.params = params;
		keyUpActions[key] = action;
	}
	
	fgl::String CharacterKeyboardController::getKeyUpActionName(fgl::Keyboard::Key key) const
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
	
	ActionParamsPtr CharacterKeyboardController::getKeyUpActionParams(fgl::Keyboard::Key key) const
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
	
	void CharacterKeyboardController::removeKeyUpAction(fgl::Keyboard::Key key)
	{
		keyUpActions.remove(key);
	}
}

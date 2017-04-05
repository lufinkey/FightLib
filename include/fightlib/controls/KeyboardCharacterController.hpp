
#pragma once

#include "CharacterController.hpp"

namespace fl
{
	class KeyboardCharacterController : public CharacterController
	{
	public:
		KeyboardCharacterController(Character* character);
		
		virtual void update(fgl::ApplicationData appData) override;
		
		void setUpKey(fgl::Keyboard::Key key);
		void setDownKey(fgl::Keyboard::Key key);
		void setLeftKey(fgl::Keyboard::Key key);
		void setRightKey(fgl::Keyboard::Key key);
		
		fgl::Keyboard::Key getUpKey() const;
		fgl::Keyboard::Key getDownKey() const;
		fgl::Keyboard::Key getLeftKey() const;
		fgl::Keyboard::Key getRightKey() const;
		
		void setKeyDownAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params = ActionParamsPtr(new ActionParams()));
		fgl::String getKeyDownActionName(fgl::Keyboard::Key key) const;
		ActionParamsPtr getKeyDownActionParams(fgl::Keyboard::Key key) const;
		void removeKeyDownAction(fgl::Keyboard::Key key);
		
		void setKeyUpAction(fgl::Keyboard::Key key, const fgl::String& actionName, ActionParamsPtr params = ActionParamsPtr(new ActionParams()));
		fgl::String getKeyUpActionName(fgl::Keyboard::Key key) const;
		ActionParamsPtr getKeyUpActionParams(fgl::Keyboard::Key key) const;
		void removeKeyUpAction(fgl::Keyboard::Key key);
		
	private:
		fgl::Keyboard::Key upKey;
		fgl::Keyboard::Key downKey;
		fgl::Keyboard::Key leftKey;
		fgl::Keyboard::Key rightKey;
		
		struct KeyAction
		{
			fgl::String name;
			ActionParamsPtr params;
		};
		
		fgl::BasicDictionary<fgl::Keyboard::Key, KeyAction> keyDownActions;
		fgl::BasicDictionary<fgl::Keyboard::Key, KeyAction> keyUpActions;
	};
}

#pragma once

#include <Ogre.h>
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "Base.h"
#include "State.h"
#include "StateManager.h"

namespace Game
{
	class InputManager: public OIS::KeyListener, public OIS::MouseListener
	{
		public:
			static InputManager& Instance();
			~InputManager();

			void Init(Ogre::RenderWindow& win);
			void Update(float t);
			void Terminate();

			bool keyPressed(const OIS::KeyEvent& e);
			bool keyReleased(const OIS::KeyEvent& e);

			bool mouseMoved(const OIS::MouseEvent& e);
			bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID b);
			bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID b);

			OIS::Mouse* m_mouse;
			OIS::Keyboard* m_keyboard;

		private:
			InputManager();

			OIS::InputManager* m_input_manager;
	};
}
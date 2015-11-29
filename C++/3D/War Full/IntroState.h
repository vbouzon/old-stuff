#pragma once

#include "Base.h"
#include "State.h"
#include "InputManager.h"

namespace Game
{
	class IntroState : public Game::State
	{

	public:
		IntroState();
		~IntroState();

		void Init();
		void Update(float t);

		bool keyPressed(const OIS::KeyEvent& e){return true;}
		bool keyReleased(const OIS::KeyEvent& e){return true;}

		bool mouseMoved(const OIS::MouseEvent& e){return true;}
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID b){return true;}
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID b){return true;}

	private:

	};
}

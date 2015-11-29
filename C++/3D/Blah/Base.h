#pragma once

#include <windows.h>
#include <Ogre.h>

#include "InputManager.h"
#include "StateManager.h"

class Base
{
		Base();

		Ogre::Root* m_root;
		Ogre::RenderWindow* m_window;
		Ogre::Overlay* mDebugOverlay;

		Ogre::Timer time;
		float t;
		bool m_run;

	public:
		static Base& Instance();
		~Base();

		bool Init();
		void InitResources();
		bool Configure();	

		bool Run();
		bool Render();
		bool Update(float t);
	
		void RequestShutdown();
		void Shutdown();

		void ShowStats();
		
		Ogre::RenderWindow* GetWindow();
		Ogre::Root* GetRoot();
};

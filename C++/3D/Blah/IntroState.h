#pragma once

#include "Base.h"
#include "State.h"
#include "InputManager.h"

#include <OgreAL/OgreAL.h>
#include <NxOgre/NxOgre.h>

namespace Game
{
	class IntroState : public Game::State
	{
		Ogre::SceneManager* m_scene_manager;
		Ogre::Camera* m_camera;
		Ogre::Viewport* m_viewport;

		NxOgre::World* m_world;
		NxOgre::Scene* m_scene;

		OgreAL::SoundManager* m_sound_manager;

	public:
		IntroState();
		~IntroState();

		void Update(float t);
		void Start();
		bool keyPressed(const OIS::KeyEvent& e);

	};
}

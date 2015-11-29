#include "IntroState.h"



namespace Game
{
	IntroState::IntroState()
	{

	}

	IntroState::~IntroState()
	{
	}

	void IntroState::Update(float t)
	{

	}


	void IntroState::Start()
	{
		m_scene_manager = Base::Instance().GetRoot()->createSceneManager(Ogre::ST_GENERIC);

		m_camera = m_scene_manager->createCamera("MenuCamera");

		m_camera->setPosition(0.0,2000.0,0.0);

		m_camera->lookAt(0.0,0.0, 0.0);
		m_camera->setNearClipDistance(1.0);

		m_scene_manager->setSkyBox(true,"Examples/MorningSkyBox");

		m_viewport = Base::Instance().GetWindow()->addViewport(m_camera);

		m_world = new NxOgre::World();

		m_scene = m_world->createScene("MaScene",m_scene_manager,"gravity: yes, floor: yes");

		NxOgre::Body* b = m_scene->createBody("sphere.mesh", new NxOgre::SphereShape(1.0), Ogre::Vector3(-150.0,5.0,0.0));
		
		b->setLinearVelocity(Ogre::Vector3(150.0,0.0,0.0));
	
		b = m_scene->createBody("sphere.mesh", new NxOgre::SphereShape(1.0), Ogre::Vector3(150.0,10.0,0.0));

		b->setLinearVelocity(Ogre::Vector3(-150.0,0.0,0.0));

	
		m_scene->createActor("Salle",new NxOgre::CubeShape(2.0), Ogre::Vector3(0.0,0.0,0.0),"static: yes");

	}

	bool IntroState::keyPressed(const OIS::KeyEvent& e)
	{
		if(e.key == OIS::KC_A)
		{
			m_sound_manager->getSound("Miaou")->play();
		}

		return true;
	}
}
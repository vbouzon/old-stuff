#include "InputManager.h"


namespace Game
{

	InputManager &InputManager::Instance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager::InputManager()
		:m_input_manager(0),m_keyboard(0),m_mouse(0)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::Init(Ogre::RenderWindow& win)
	{
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		win.getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		m_input_manager = OIS::InputManager::createInputSystem(pl);

		m_keyboard = (OIS::Keyboard*)(m_input_manager->createInputObject(OIS::OISKeyboard, true));
		m_mouse = (OIS::Mouse*)(m_input_manager->createInputObject(OIS::OISMouse, true));

		m_mouse->setEventCallback(this);
		m_keyboard->setEventCallback(this);
	}

	void InputManager::Terminate()
	{
		m_input_manager->destroyInputObject(m_keyboard);
		m_input_manager->destroyInputObject(m_mouse);

		OIS::InputManager::destroyInputSystem(m_input_manager);
	}

	bool InputManager::keyPressed(const OIS::KeyEvent& e)
	{
		return StateManager::Instance().GetCurrentState()->keyPressed(e);
	}

	bool InputManager::keyReleased(const OIS::KeyEvent& e)
	{
		return StateManager::Instance().GetCurrentState()->keyReleased(e);
	}

	bool InputManager::mouseMoved(const OIS::MouseEvent& e)
	{
		return StateManager::Instance().GetCurrentState()->mouseMoved(e);
	}

	bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID b)
	{
		return StateManager::Instance().GetCurrentState()->mousePressed(e, b);
	}

	bool InputManager::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID b)
	{
		return StateManager::Instance().GetCurrentState()->mouseReleased(e, b);
	}

}
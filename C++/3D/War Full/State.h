#ifndef STATE_H
#define STATE_H

#include <Ogre.h>
#include <OIS/OIS.h>

namespace Game
{
	class State
	{
	public:

		State(){};
		~State(){};

		virtual void Init() = 0;
		virtual void Update(float t) = 0;

		virtual bool keyPressed(const OIS::KeyEvent& e) = 0;
		virtual bool keyReleased(const OIS::KeyEvent& e) = 0;

		virtual bool mouseMoved(const OIS::MouseEvent& e);
		virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID b) = 0;
		virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID b) = 0;
	};


}

#endif
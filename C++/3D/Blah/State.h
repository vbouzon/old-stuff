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

		virtual void Init() {}
		virtual void Start() {}
		virtual void Pause() {}
		virtual void Update(float t) {}

		virtual bool keyPressed(const OIS::KeyEvent& e) { return true; }
		virtual bool keyReleased(const OIS::KeyEvent& e) { return true; }

		virtual bool mouseMoved(const OIS::MouseEvent& e) { return true; }
		virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID b) { return true; }
		virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID b) { return true; }
	};


}

#endif
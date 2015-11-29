#pragma once

#include <cassert>
#include <map>
#include <string>

#include "State.h"
#include "IntroState.h"

namespace Game
{
	typedef std::map <std::string, State*> MapState;

	class StateManager
	{
	public:
		static StateManager& Instance();
		~StateManager(); 

		void Init();
		void SetState(const std::string& StateName);
		void Update(float t);
		void Terminate();

		State *GetState(const std::string& StateName);
		State *GetCurrentState();

	private:

		StateManager();

		MapState m_StateList;
		State* m_currentState;
	};

}

#include "StateManager.h"

namespace Game
{
	
	StateManager& StateManager::Instance()
	{
		static StateManager instance;
		return instance;
	}

	StateManager::StateManager()
	{
		m_currentState = NULL;
	}

	StateManager::~StateManager()
	{
	}

	void StateManager::Init()
	{
		//A deporter dans l etat ...
		//Base::Instance().GetSceneManager()->setFog( FOG_LINEAR, ColourValue(0,0,0), 0.001);
		//Base::Instance().GetSceneManager()->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
		//OverlayObjectManager::Instance().Init();


		m_StateList.insert(MapState::value_type("INTRO",new IntroState));

		MapState::const_iterator iter;
		for(iter = m_StateList.begin(); iter != m_StateList.end(); ++iter)
		{
			iter->second->Init();
		}

		SetState("INTRO");
	}

	void StateManager::SetState(const std::string& StateName)
	{
		if( m_currentState != NULL ) 
			m_currentState->Pause();

		assert(m_StateList[StateName] != NULL);
		m_currentState = m_StateList[StateName];

		m_currentState->Start();
	}

	void StateManager::Update(float t)
	{
		m_currentState->Update(t);
	}

	void StateManager::Terminate()
	{
		MapState::const_iterator iter;
		for(iter = m_StateList.begin(); iter != m_StateList.end(); ++iter)
		{
			delete iter->second;
		}

		m_StateList.clear();
	}

	State* StateManager::GetState(const std::string& StateName)
	{
		assert(m_StateList[StateName] != NULL);
		return m_StateList[StateName];
	}

	State* StateManager::GetCurrentState()
	{
		return m_currentState;
	}
}
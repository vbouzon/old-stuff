#include "CStateManager.h"

void CStateManager::SetState(CState *NewState)
{
	CurrentState = NewState;
	CurrentState->Init();
}
CState* CStateManager::GetCurrentState()
{
	return CurrentState;
}
void CStateManager::Update(float time)
{
	CurrentState->UpdateInput(time);
	CurrentState->Update(time);
}

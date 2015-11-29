#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

#include "../CSingleton.h"
#include "CState.h"

class CStateManager : public CSingleton<CStateManager>
{
	CState *CurrentState;

public:
	void SetState(CState *NewState);
	CState *GetCurrentState();
	void Update(float time);
};

#endif


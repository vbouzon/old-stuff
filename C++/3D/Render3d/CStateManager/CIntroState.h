#ifndef CINTROSTATE_H
#define CINTROSTATE_H

#include "CState.h"
#include "../CForme/CTriangle.h"
#include "../CForme/CNoeud.h"
#include "../CEventManager/CEventManager.h"

class CIntroState : public CState
{
	CTriangle *Tri;
	CNoeud *n;
public:

	void Init();
	void Update(float time);
	void UpdateInput(float time);
};

#endif

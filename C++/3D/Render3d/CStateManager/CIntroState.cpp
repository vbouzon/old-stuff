#include "CIntroState.h"

void CIntroState::Init()
{
	CVertex v0(  0.0f, 0.0f,  0.0 );
    CVertex v1(  1.0f, 0.0f,  0.0 );
    CVertex v2(  1.0f,  1.0f,  0.0 );

	Tri = new CTriangle(v0,v1,v2);

	n = new CNoeud(0.0,0.0,0.0);

	n->SetEntity(Tri);
	n->Translate(0,0,-5);

}
void CIntroState::Update(float time)
{
	n->Render();
}
void CIntroState::UpdateInput(float time)
{
	if(CEventManager::GetInstance()->GetKeyDown(SDLK_a))
	{
		n->Rotate(0,180*time/1000,0);
	}
}


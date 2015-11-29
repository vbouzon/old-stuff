#include "CEventManager.h"

bool CEventManager::GetKeyDown(int key)
{
	KeyboardState = SDL_GetKeyState(NULL);
    return KeyboardState[key];
}
void CEventManager::ManageEvent()
{
	while( SDL_PollEvent( &event ) ){};
}

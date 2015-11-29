#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include "../CSingleton.h"
#include <SDL/SDL.h>

class CEventManager : public CSingleton<CEventManager>
{
	Uint8 *KeyboardState;
	SDL_Event event;

public:
    void ManageEvent();
	bool GetKeyDown(int key);
      
};


#endif

#include <iostream>
#include "CRenderer/CRenderer.h"
#include "CEventManager/CEventManager.h"
#include "CStateManager/CStateManager.h"
#include "CStateManager/CIntroState.h"

int main(int argc, char *argv[])
{
    Uint32 last_time,time_frame;

	
	CRenderer Render;
    CEventManager *EventManager;
	CStateManager *StateManager;

	float x = 0;

    EventManager = CEventManager::GetInstance();
	StateManager = CStateManager::GetInstance();

    if(!Render.Init(1024,768))return -1;
    Render.Config();

	CIntroState intro;

	StateManager->SetState(&intro);

	last_time = SDL_GetTicks();

	while(!EventManager->GetKeyDown(SDLK_ESCAPE))
	{
		
		time_frame = SDL_GetTicks() - last_time;
		last_time +=  time_frame;

		EventManager->ManageEvent();

		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		StateManager->Update(time_frame);

		SDL_GL_SwapBuffers();
	}
    
	return EXIT_SUCCESS;
}

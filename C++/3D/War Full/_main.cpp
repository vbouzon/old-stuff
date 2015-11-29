#include <windows.h>
#include <Ogre.h>

#include "Base.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	try
	{
		Base::Instance().Run();
	}
	catch( Ogre::Exception& e )
	{
		MessageBox( NULL, e.getFullDescription().c_str(), "Petit Problème", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}

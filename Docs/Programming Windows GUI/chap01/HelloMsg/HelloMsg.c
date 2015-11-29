/*----------------------------------------------------------------
   HelloMsg.C -- Affiche "Hello, Windows 98 !" dans une boîte de message
                 (c) Charles Petzold, 1998
------------------------------------------------------------------*/

#include <windows.h>


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)

{
	MessageBox (NULL, TEXT ("Hello, Windows 98 !"), TEXT ("HelloMsg"), 0) ;

return 0 ;

}

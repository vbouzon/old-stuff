/*-----------------------------------------------------
   SCRNSIZE.C -- Affiche la taille de l'écran dans une boîte de message
                 (c) Charles Petzold, 1998
  -----------------------------------------------------*/

#include <windows.h>
#include <tchar.h>     
#include <stdio.h>     

int CDECL MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...)
{
     TCHAR   szBuffer [1024] ;
     va_list pArgList ;

          // La macro va_start (définie dans STDARG.H) est habituellement équivalente à:
          // pArgList = (char *) &szFormat + sizeof (szFormat) ;

     va_start (pArgList, szFormat) ;

          // Le dernier argument de wvsprintf pointe sur les arguments

     _vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
                  szFormat, pArgList) ;

          // La macro va_end juste des zéros de pArgList pour aucune bonne raison

     va_end (pArgList) ;

     return MessageBox (NULL, szBuffer, szCaption, 0) ;
}
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow) 
{
     int cxScreen, cyScreen ;

     cxScreen = GetSystemMetrics (SM_CXSCREEN) ;
     cyScreen = GetSystemMetrics (SM_CYSCREEN) ;

     MessageBoxPrintf (TEXT ("Taille de l'écran"), 
                       TEXT ("L'écran fait %i pixels de large sur %i pixels de haut."),
                       cxScreen, cyScreen) ;
     return 0 ;
}
